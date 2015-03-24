#ifndef _SERIAL_CTRL_H
#define _SERIAL_CTRL_H

/**************************************************************************/
/* Title: CSerialIO: a useful and simple serial control                   */
/* Author: wufengkai                                                      */
/* Email:  tojine@hotmail.com                                             */

class SerialCtrl
{
private:
	bool m_portStatus;                    
	HANDLE m_portHandle;                  
	DCB m_portConfig;   

public:
	SerialCtrl(void);
	~SerialCtrl(void);             
	void SetPortStatus(bool bOnOff);
	bool GetPortStatus(void);
	HANDLE GetPortHandle(void);
	bool OpenPort(DCB dcb, const char * portName="COM1");
	bool OpenPort(const char * portName="COM1", int baudRate = CBR_115200);
	bool Read(char * inputData, const unsigned int & sizeBuffer, unsigned long & length);
	bool Write(const char *outputData, const unsigned int & sizeBuffer, unsigned long & length);
	bool ClosePort(void);
};


//////////////////////////////////////////////////////////
class CSerialIO;

class SerialThread : public CWinThread
{
public:
	// To enable objects of CObject-derived classes to be created dynamically at  run timeto enable objects of CObject-derived classes to be created 
	DECLARE_DYNCREATE(SerialThread)  
	SerialThread ();               // Constructor.
	virtual ~SerialThread();       // Destructor.

	void ClosePort();
	virtual BOOL InitInstance();   // virtual function that derive from base-class.
	virtual int Run();             // virtual function that derive from base-class.  
	void setOwner(CSerialIO* serialIO)
	{
		m_serialIO = serialIO;
	}   

	static SerialCtrl& serialCtrl() // Implement as the singleton funciton.
	{
		static SerialCtrl serial;
		return serial;
	}

private:
	CSerialIO*  m_serialIO;         // The pointer that pointer to object user data, such dialog,window
};

////////////////////////////////////////////////////////////
#define MAX_SEND_BUFFER 1024

class CSerialIO
{
public:
	CSerialIO();
	virtual ~CSerialIO();

    void OpenPort(const char* strPortName, int nBaudRate);//open serial port with parameter name and baud rate
	void ClosePort();//close serial port
	void Write(char *outPacket,int outLength);// write data directly 

	virtual void OnEventOpen(bool bSuccess);// handle the event whether the port is successfully opened 
	virtual void OnEventClose(bool bSuccess);//handle the event whether the port is successfully closed
	virtual void OnEventRead(char *inPacket,int inLength);// handle the received data from serial
	virtual void OnEventWrite(int nWritten); //return how many bytes has been written

	bool GetSendActivateValue(void); 
	void SetSendActivate(bool bSendActivate);
	bool GetPortActivateValue(void);
	void SetPortActivate(bool bPortActivate);
	bool GetProcessActivateValue(void);
	void SetProcessActivate(bool bProcessActivate);
	void SetClosePortValue(bool bVal);
	bool GetClosePortValue();

	DCB m_DCB;
	char m_sendBuffer[MAX_SEND_BUFFER];
	unsigned int m_sendSize;
	char m_strPortName[50];
	
private:
	bool Init();
	void UnInit();
	SerialThread* m_serialProcess;
	bool m_bProccessActivate;
	bool m_bPortActivate;
	bool m_bSendActivate;
	int m_nBaudRate;
	bool m_bClosePort;
};

#endif