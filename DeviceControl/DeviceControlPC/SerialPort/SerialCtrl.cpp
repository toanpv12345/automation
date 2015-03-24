#include "../StdAfx.h"
#include "SerialCtrl.h"

#define MESSAGE_LENGTH				500

CRITICAL_SECTION CriticalSection; 

SerialCtrl::SerialCtrl(void):m_portStatus(false),m_portHandle(NULL)
{
	m_portConfig.ByteSize = 8;            // Byte of the Data.
	m_portConfig.StopBits = ONESTOPBIT;   // Use one bit for stopbit.
	m_portConfig.Parity = NOPARITY;       // No parity bit
	m_portConfig.BaudRate = CBR_115200;     // Buadrate 9600 bit/sec
}

SerialCtrl::~SerialCtrl(void)
{
	m_portHandle = NULL;
}

void SerialCtrl::SetPortStatus(bool bOnOff)
{
	m_portStatus = bOnOff;
}

bool SerialCtrl::GetPortStatus(void)
{
	return m_portStatus;
}

HANDLE SerialCtrl::GetPortHandle(void)
{
	return m_portHandle;
}

bool SerialCtrl::OpenPort(DCB dcb, const char * portName)
{
	if (m_portStatus == false)  // if port is opened already, not open port again.
	{
		m_portHandle = CreateFileA(portName,  // Specify port device: default "COM1"
			GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
			0,                                  // the devide isn't shared.
			NULL,                               // the object gets a default security.
			OPEN_EXISTING,                      // Specify which action to take on file. 
			0,                                  // default.
			NULL);                              // default.

		// Get current configuration of serial communication port.
		if (GetCommState(m_portHandle,&m_portConfig) == 0)
		{
			AfxMessageBox(L"Get configuration port has problem.");
			return false;
		}
		// Assign user parameter.
		m_portConfig.BaudRate = dcb.BaudRate;    // Specify buad rate of communicaiton.
		m_portConfig.StopBits = dcb.StopBits;    // Specify stopbit of communication.
		m_portConfig.Parity = dcb.Parity;        // Specify parity of communication.
		m_portConfig.ByteSize = dcb.ByteSize;    // Specify  byte of size of communication.

		// Set current configuration of serial communication port.
		if (SetCommState(m_portHandle,&m_portConfig) == 0)
		{
			AfxMessageBox(L"Set configuration port has problem.");
			return false;
		}

		// instance an object of COMMTIMEOUTS.
		COMMTIMEOUTS comTimeOut;                   
		comTimeOut.ReadIntervalTimeout = 3;
		comTimeOut.ReadTotalTimeoutMultiplier = 3;
		comTimeOut.ReadTotalTimeoutConstant = 2;
		comTimeOut.WriteTotalTimeoutMultiplier = 3;
		comTimeOut.WriteTotalTimeoutConstant = 2;
		SetCommTimeouts(m_portHandle,&comTimeOut);		// set the time-out parameter into device control.
		m_portStatus = true; 
		return true;      
	}
	return false;
}

bool SerialCtrl::OpenPort(const char * portName, int nBaudRate)
{
	DCB configSerial;
	configSerial.ByteSize = 8;
	configSerial.StopBits = ONESTOPBIT;
	configSerial.Parity = NOPARITY;
	configSerial.BaudRate = nBaudRate;
	return OpenPort(configSerial, portName);
}
bool SerialCtrl::Read(char * inputData, const unsigned int & sizeBuffer, unsigned long & length)
{
	if (ReadFile(m_portHandle,  // handle of file to read
		inputData,               // handle of file to read
		sizeBuffer,              // number of bytes to read
		&length,                 // pointer to number of bytes read
		NULL) == 0)              // pointer to structure for data
	{
		// AfxMessageBox("Reading of serial communication has problem.");
		return false;
	}
	if (length > 0)
	{
		inputData[length] = NULL; // Assign end flag of message.
		return true;  
	}  
	return true;
}

bool SerialCtrl::Write(const char * outputData, const unsigned int & sizeBuffer, unsigned long & length)
{
	if (sizeBuffer > 0)
	{
		if (WriteFile(m_portHandle, // handle to file to write to
			outputData,              // pointer to data to write to file
			sizeBuffer,              // number of bytes to write
			&length,NULL) == 0)      // pointer to number of bytes written
		{
			AfxMessageBox(L"Reading of serial communication has problem.");
			return false;
		}
		return true;
	}
	return false;
}

bool SerialCtrl::ClosePort(void)
{
	if (m_portStatus == true)               // Port need to be open before.
	{
		m_portStatus = false;                 // Update status
		if(CloseHandle(m_portHandle) == FALSE)    // Call this function to close port.
		{
			AfxMessageBox(L"Port Closeing isn't successed.");
			return false;
		}    
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(SerialThread,CWinThread)
SerialThread::SerialThread() :m_serialIO(NULL)
{

}
SerialThread::~SerialThread()
{
	m_serialIO = NULL;
}

BOOL SerialThread::InitInstance()
{
	return TRUE;
}

int SerialThread::Run()
{
	// Check signal controlling and status to open serial communication.
	while(1)
	{
		while(m_serialIO->GetProcessActivateValue() == true)
		{
			if ((serialCtrl().GetPortStatus()==false)&&m_serialIO->GetPortActivateValue()==true)
			{
				if(serialCtrl().OpenPort(m_serialIO->m_DCB, m_serialIO->m_strPortName)==true)
				{
					m_serialIO->OnEventOpen(true);
				}
				else
				{
					m_serialIO->OnEventOpen(false);
				    m_serialIO->SetPortActivate(false);
				}
				
			}
			else if (m_serialIO->GetPortActivateValue()==true)
			{
				char message[MESSAGE_LENGTH] = {0};
				unsigned int lenBuff = MESSAGE_LENGTH;
				unsigned long lenMessage;
				if(serialCtrl().Read(message,lenBuff,lenMessage)==true)
				{
					if(lenMessage>0)
					{
						m_serialIO->OnEventRead(message,lenMessage);
					}
				}
				else
				{
					//m_serialIO->SetProcessActivate(false);
				}
			}

			if (m_serialIO->GetSendActivateValue()==true)
			{
				unsigned long nWritten = 0;
				if (m_serialIO->m_sendSize > 0)
				{
					if(serialCtrl().Write(m_serialIO->m_sendBuffer, m_serialIO->m_sendSize, nWritten) == true)
					{
						m_serialIO->OnEventWrite(nWritten);
					}
					else
					{
						m_serialIO->OnEventWrite(-1);
					}
				}
				m_serialIO->SetSendActivate(false);
			}

			if (m_serialIO->GetClosePortValue()==true)
			{
				m_serialIO->SetClosePortValue(false);
				if (serialCtrl().ClosePort()==true)
				{
					m_serialIO->SetProcessActivate(false);
					m_serialIO->OnEventClose(true);
					DWORD nExitCode = 0;

					if(!GetExitCodeThread(m_hThread, &nExitCode))
						AfxMessageBox(L"Get Exit code Thread error");

					if(!TerminateThread(m_hThread, nExitCode))
						AfxMessageBox(L"Terminal Thread error");

					CloseHandle(m_hThread);
				}
				else
				{
					m_serialIO->OnEventClose(false);
				}
			}
		}
	}
	return 0;
}
void SerialThread::ClosePort()
{
	serialCtrl().ClosePort();
}

///////////////////////////////////////////////////////////
CSerialIO::CSerialIO():m_serialProcess(NULL), m_nBaudRate(CBR_115200), m_sendSize(0)
{
	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400) ) 
	{
		AfxMessageBox(L"Init critical error");
	}
	m_bProccessActivate=false;
	m_bPortActivate=false;
	m_bSendActivate=false;
	m_bClosePort=false;
	sprintf(m_strPortName, "%s", "COM1");
}
CSerialIO:: ~CSerialIO()
{
	UnInit();
}
bool CSerialIO::Init()
{
	m_DCB.ByteSize = 8;
	m_DCB.StopBits = ONESTOPBIT;
	m_DCB.Parity = NOPARITY;
	m_DCB.BaudRate = m_nBaudRate;

	EnterCriticalSection(&CriticalSection);
	m_bProccessActivate = true;
	LeaveCriticalSection(&CriticalSection);

	if (m_serialProcess != NULL)
	{
		delete m_serialProcess;
		m_serialProcess = NULL;
	}

	m_serialProcess = (SerialThread*)AfxBeginThread(RUNTIME_CLASS(SerialThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_serialProcess->setOwner(this);
	m_serialProcess->ResumeThread();

	return true;
}
void CSerialIO::UnInit()
{
	EnterCriticalSection(&CriticalSection);
	m_bPortActivate = false;
	LeaveCriticalSection(&CriticalSection);
	if(m_serialProcess)
	{
		m_serialProcess->ClosePort();
		m_serialProcess->SuspendThread();

		DWORD nExitCode = 0;

		if(!GetExitCodeThread(m_serialProcess->m_hThread, &nExitCode))
			AfxMessageBox(L"Get Exit code Thread error");

		if(!TerminateThread(m_serialProcess->m_hThread, nExitCode))
			AfxMessageBox(L"Terminal Thread error");

		delete m_serialProcess;
		m_serialProcess = NULL;
	}
	DeleteCriticalSection(&CriticalSection);
}
void CSerialIO::OpenPort(const char* strPortName, int nBaudRate)
{
	m_nBaudRate = nBaudRate;
	sprintf(m_strPortName, "%s", strPortName);

	EnterCriticalSection(&CriticalSection);
	m_bPortActivate = true;
	LeaveCriticalSection(&CriticalSection);
	Init();
}
void CSerialIO::OnEventOpen(bool bSuccess)
{
	if(bSuccess)
		printf("port open successfully\n");
	else
		printf("port open failed\n");
}

void CSerialIO::ClosePort()
{
	SetClosePortValue(true);
	SetPortActivate(false);
}

void CSerialIO::OnEventClose(bool bSuccess)
{
	if(bSuccess)
		printf("port close successfully\n");
	else
		printf("port close failed\n");
	
}

void CSerialIO::OnEventRead(char *inPacket, int inLength)
{
	return;
}

void CSerialIO::Write(char *outPacket,int outLength)
{
	if(outLength<=MAX_SEND_BUFFER)
	{
		memcpy(m_sendBuffer,outPacket, outLength);
		//sprintf(m_sendBuffer, "%s", outPacket);
		m_sendSize = outLength;
		SetSendActivate(true);
		
	}else
	{
		printf("buffer over flow");
		OnEventWrite(-1);
	}
	return ;
}

void CSerialIO::OnEventWrite(int nWritten)
{
	if(nWritten<0)
	{
		printf("write error\n");
	}
	else
	{
		printf("%d bytes send\n",nWritten);
	}
}

bool CSerialIO::GetSendActivateValue(void)
{
	EnterCriticalSection(&CriticalSection);
	bool bVal = m_bSendActivate;
	LeaveCriticalSection(&CriticalSection);
	return bVal;
}

void CSerialIO::SetSendActivate(bool bSendActivate)
{
	EnterCriticalSection(&CriticalSection);
	m_bSendActivate = bSendActivate;
	LeaveCriticalSection(&CriticalSection);
}

bool CSerialIO::GetPortActivateValue(void)
{
	EnterCriticalSection(&CriticalSection);
	bool bVal = m_bPortActivate;
	LeaveCriticalSection(&CriticalSection);
	return bVal;
}

void CSerialIO::SetPortActivate(bool bPortActivate)
{
	EnterCriticalSection(&CriticalSection);
	m_bPortActivate = bPortActivate;
	LeaveCriticalSection(&CriticalSection);
}

bool CSerialIO::GetProcessActivateValue(void)
{
	EnterCriticalSection(&CriticalSection);
	bool bVal = m_bProccessActivate;
	LeaveCriticalSection(&CriticalSection);
	return bVal;
}

void CSerialIO::SetProcessActivate(bool bProcessActivate)
{
	EnterCriticalSection(&CriticalSection);
	m_bProccessActivate = bProcessActivate;
	LeaveCriticalSection(&CriticalSection);
}

void CSerialIO::SetClosePortValue( bool bVal )
{
	EnterCriticalSection(&CriticalSection);
	m_bClosePort = bVal;
	LeaveCriticalSection(&CriticalSection);
}

bool CSerialIO::GetClosePortValue()
{
	EnterCriticalSection(&CriticalSection);
	bool bVal = m_bClosePort;
	LeaveCriticalSection(&CriticalSection);
	return bVal;
}
