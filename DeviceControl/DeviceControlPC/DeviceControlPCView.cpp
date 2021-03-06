// GSM_TopUpView.cpp : implementation of the CSmartSimTopUpView class
//
#include "stdafx.h"
#include "DeviceControlPC.h"
#include <string.h>
#include <stdio.h>
#include "DeviceControlPCDoc.h"
#include "DeviceControlPCView.h"
#include "MainFrm.h"

#define START       0
#define DEVICE_ID   1
#define CMD         2
#define DATA        3
#define CRC         4
#define END         5

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(DeviceControlPCView, CFormView)

BEGIN_MESSAGE_MAP(DeviceControlPCView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_OPENGSMPORT, &DeviceControlPCView::OnOpenGSMPort)
	ON_BN_CLICKED(IDC_BUTTONSCANCOMPORT, &DeviceControlPCView::OnScanComport)
	ON_BN_CLICKED(IDC_BUTTON_RELAY1, &DeviceControlPCView::OnBnClickedButtonRelay1)
	ON_BN_CLICKED(IDC_BUTTON_RELAY2, &DeviceControlPCView::OnBnClickedButtonRelay2)
	ON_BN_CLICKED(IDC_BUTTON_RELAY3, &DeviceControlPCView::OnBnClickedButtonRelay3)
	ON_BN_CLICKED(IDC_BUTTON_RELAY4, &DeviceControlPCView::OnBnClickedButtonRelay4)
	ON_BN_CLICKED(IDC_BUTTON_RELAY5, &DeviceControlPCView::OnBnClickedButtonRelay5)
	ON_BN_CLICKED(IDC_BUTTON_RELAY6, &DeviceControlPCView::OnBnClickedButtonRelay6)
	ON_BN_CLICKED(IDC_BUTTON_RELAY7, &DeviceControlPCView::OnBnClickedButtonRelay7)
	ON_BN_CLICKED(IDC_BUTTON_RELAY8, &DeviceControlPCView::OnBnClickedButtonRelay8)
	ON_BN_CLICKED(IDC_BUTTON_SETID, &DeviceControlPCView::OnBnClickedButtonSetid)
	ON_BN_CLICKED(IDC_BUTTON_GETID, &DeviceControlPCView::OnBnClickedButtonGetid)
	ON_BN_CLICKED(IDC_BT_CLOSEPORT, &DeviceControlPCView::OnBnClickedBtCloseport)
END_MESSAGE_MAP()


DeviceControlPCView::DeviceControlPCView()
: CFormView(DeviceControlPCView::IDD)
, m_szComPortList(_T(""))
, m_nDeviceID(0)
, m_nNewID(0)
, m_nCurrentID(0)
{
	m_isRelay1On = false;
	m_isRelay2On = false;
	m_isRelay3On = false;
	m_isRelay4On = false;
	m_isRelay5On = false;
	m_isRelay6On = false;
	m_isRelay7On = false;
	m_isRelay8On = false;
	m_commandType = RELAY_ON;
	m_data = "";
}

DeviceControlPCView::~DeviceControlPCView()
{
	
}

void DeviceControlPCView::GetPath(char* path, const char* filename)
{
	char apppath[MAX_PATH] = {0};
	char* pch;
	GetModuleFileNameA(NULL, path, MAX_PATH);

	char *slice = NULL;
	slice = strrchr(path, '\\');
	if(slice)
		*slice = 0;
}

BOOL DeviceControlPCView::isFolderExists(CString strFolderName)
{       
	return (GetFileAttributes(strFolderName) != INVALID_FILE_ATTRIBUTES);   
}

void DeviceControlPCView::WriteLog(const char* log)
{
	wchar_t filepath[MAX_PATH] = {0};
	wchar_t *slice = NULL;

	GetModuleFileName(0, filepath, MAX_PATH);
	slice = wcsrchr(filepath, '\\');

	if(slice)
	{
		*slice =0;
		slice = NULL;
		free(slice);
	}

	CString cLogPath = L"";
	cLogPath.Format(L"%s\\LOG", filepath);

	CString cCmdLogFile = L"";
	cCmdLogFile.Format(L"\\log");

	DWORD dwErr = 0;
	CTime theTime = CTime::GetCurrentTime();

	if(!isFolderExists(cLogPath))
	{
		if(!CreateDirectory(cLogPath, NULL))
		{
			dwErr = GetLastError();
			AfxMessageBox(_T("Khong tao duoc LOG folder!"));
			return;
		}

		cLogPath.Append(cCmdLogFile);
		cLogPath.Append(theTime.Format("%Y-%m-%d"));
		cLogPath.Append(_T(".txt"));

		HANDLE hFile = CreateFile(cLogPath,
			GENERIC_WRITE, FILE_SHARE_READ,
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			dwErr = GetLastError();
			AfxMessageBox(_T("Khong tao duoc file log!"));
		}
		else
		{
			try {
				CFile myFile(hFile);			
				myFile.Write(log, strlen(log));
				myFile.Close();
			}
			catch (CMemoryException* e)
			{
				e->ReportError();
				e->Delete();
			}
			catch (CFileException* e)
			{
				e->ReportError();
				e->Delete();
			}
			catch (CException* e)
			{
				e->ReportError();
				e->Delete();
			}
		}
	}
	else
	{
		cLogPath.Append(cCmdLogFile);
		cLogPath.Append(theTime.Format("%Y-%m-%d"));
		cLogPath.Append(_T(".txt"));

		if(!isFolderExists(cLogPath))
		{
			HANDLE hFile = CreateFile(cLogPath,
				GENERIC_WRITE, FILE_SHARE_READ,
				NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile == INVALID_HANDLE_VALUE)
			{
				dwErr = GetLastError();
				AfxMessageBox(_T("Khong tao duoc file log!"));
			}
			else
			{
				try {
					CFile myFile(hFile);			
					myFile.Write(log, strlen(log));
					myFile.Close();
				}
				catch (CMemoryException* e)
				{
					e->ReportError();
					e->Delete();
				}
				catch (CFileException* e)
				{
					e->ReportError();
					e->Delete();
				}
				catch (CException* e)
				{
					e->ReportError();
					e->Delete();
				}
			}
		}
		else
		{
			try
			{
				CFile myFile(cLogPath, CFile::modeReadWrite);
				myFile.Seek(0, CFile::end);
				myFile.Write(log, strlen(log));
				myFile.Close();
			}
			catch (CMemoryException* e)
			{
				e->ReportError();
				e->Delete();
			}
			catch (CFileException* e)
			{
				e->ReportError();
				e->Delete();
			}
			catch (CException* e)
			{
				e->ReportError();
				e->Delete();
			}
		}
	}
}

int DeviceControlPCView::GetComportNumber( char* comPortName )
{
	if(comPortName)
	{
		int len = strlen(comPortName);
		char n[5] = {0};
		int j = 0;
		if (len)
		{
			for(int i = 0; i < len; i++)
			{
				if ((comPortName[i] >= '0') && (comPortName[i] <= '9'))
				{
					n[j] = comPortName[i];
					j++;
				}
			}
			if (j > 0)
				return atoi(n);
		}
		return -1;
	}
	return -1;
}

void DeviceControlPCView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_cbComport);
	DDX_CBString(pDX, IDC_COMBO_PORT, m_szComPortList);
	DDX_Text(pDX, IDC_EDIT_DEVICEID, m_nDeviceID);
	DDX_Text(pDX, IDC_EDIT_SETID, m_nNewID);
	DDX_Text(pDX, IDC_EDIT_GETID, m_nCurrentID);
}

BOOL DeviceControlPCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CFormView::PreCreateWindow(cs);
}

void DeviceControlPCView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	UpdateData(TRUE);

	SetDlgItemText(IDC_ST_STATUSPORT,_T("Closed"));
	GetDlgItem(IDC_BT_CLOSEPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPENGSMPORT)->EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_OPENGSMPORT, L"Open Port");

	GetDlgItem(IDC_BUTTON_RELAY1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RELAY2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RELAY3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RELAY4)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RELAY5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RELAY6)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RELAY7)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RELAY8)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SETID)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_GETID)->EnableWindow(FALSE);

	OnScanComport();
	UpdateData(FALSE);
}

#ifdef _DEBUG
void DeviceControlPCView::AssertValid() const
{
	CFormView::AssertValid();
}
void DeviceControlPCView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

DeviceControlPCDoc* DeviceControlPCView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(DeviceControlPCDoc)));
	return (DeviceControlPCDoc*)m_pDocument;
}
#endif //_DEBUG

void DeviceControlPCView::OnOpenGSMPort()
{
	UpdateData(TRUE);
	m_cbComport.GetCurSel();

	char *portName = GetAString(m_szComPortList.GetBuffer());
	int comNumber = GetComportNumber(portName);
	if (comNumber >= 10)
	{
		CString port = L"";
		port.Format(L"\\\\.\\COM%d", comNumber); 			
		portName = GetAString(port);
	}
	OpenPort(portName, CBR_9600);
	delete portName;
}

void DeviceControlPCView::scanComport()
{
	char port_name[20] = {0};
	int n = 0;

	m_szComPortList.Empty();
	UpdateData(TRUE);
	m_cbComport.Clear();
	m_cbComport.ResetContent();

	for (int port_number = 1; port_number < 128; ++port_number)
	{
		port_number < 10 ? sprintf_s(port_name, sizeof( port_name ), "COM%d", port_number) :
			sprintf_s(port_name, sizeof( port_name ), "\\\\.\\COM%d", port_number);

		HANDLE hFile = ::CreateFileA(port_name,
			GENERIC_READ | GENERIC_WRITE, 0, NULL,
			OPEN_EXISTING, 0, NULL);

		if(hFile != INVALID_HANDLE_VALUE)
		{
			UpdateData(TRUE);
			port_number < 10 ? (m_szComPortList = port_name) : (m_szComPortList = port_name + 4);
			n = m_cbComport.GetCount();
			m_cbComport.InsertString(n, m_szComPortList);
			UpdateData(FALSE);
		}

		CloseHandle(hFile);
	}
	UpdateData(FALSE);
}

void DeviceControlPCView::OnScanComport()
{
	scanComport();
	if(m_cbComport.GetCount() > 0)
	{
		GetDlgItem(IDC_BUTTON_OPENGSMPORT)->EnableWindow(TRUE);
		m_cbComport.SetCurSel(0);
		OnOpenGSMPort();
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OPENGSMPORT)->EnableWindow(FALSE);
	}
}

void DeviceControlPCView::LogView( const char* log )
{
	CString outPut = L"";
	CTime currentTime = CTime::GetCurrentTime();
	CString currentTimeFormat = currentTime.Format("%Y-%m-%d %H:%M:%S");
	outPut.Format(L"\r\n%s: %s\r\n", currentTimeFormat, (CString)log);

	CMainFrame *frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	frm->m_pBottomRichTextView->Addtext(outPut, true);

	int nTextLen =::GetWindowTextLength(frm->m_pBottomRichTextView->m_hWnd);
	if(nTextLen >= 1024)
		frm->m_pBottomRichTextView->ClearText();

	char *cLog = GetAString(outPut.GetBuffer());
	WriteLog(cLog);

	delete cLog;
}

void DeviceControlPCView::OnEventOpen( bool bSuccess )
{
	if (bSuccess)
	{
		LogView("Port is opened");
		SetDlgItemText(IDC_ST_STATUSPORT,_T("Opened"));
		GetDlgItem(IDC_BT_CLOSEPORT)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_RELAY1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RELAY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RELAY3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RELAY4)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RELAY5)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RELAY6)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RELAY7)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RELAY8)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SETID)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_GETID)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_RELAY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY5)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY6)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY7)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY8)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SETID)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_GETID)->EnableWindow(FALSE);

		LogView("Open port error!");
	}
}

void DeviceControlPCView::OnEventClose( bool bSuccess )
{
	if (bSuccess)
	{
		LogView("Port is closed");
		SetDlgItemText(IDC_BUTTON_OPENGSMPORT, L"Open Port");
		SetDlgItemText(IDC_ST_STATUSPORT, L"Closed");

		GetDlgItem(IDC_BUTTON_RELAY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY5)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY6)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY7)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RELAY8)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SETID)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_GETID)->EnableWindow(FALSE);
	}
}

void DeviceControlPCView::OnEventRead( char *inPacket, int inLength )
{
	m_data.append(inPacket);
	
	if (m_data.length() >= 6)
	{
		CString outPut = L"";
		outPut.Format(L"\r\nWRITE:%d %d %d %d %d %d", m_data[0],m_data[1],m_data[2],m_data[3],m_data[4],m_data[5]);

		CMainFrame *frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
		frm->m_pBottomRichTextView->Addtext(outPut, true);

		int nTextLen =::GetWindowTextLength(frm->m_pBottomRichTextView->m_hWnd);
		if(nTextLen >= 2000)
			frm->m_pBottomRichTextView->ClearText();

		if (m_data[START] == -96 && m_data[END] == -80)
		{
			if(m_data[CMD] == GET_ID)
			{
				SetDlgItemInt(IDC_EDIT_GETID, m_data[DEVICE_ID]);
				SetDlgItemInt(IDC_EDIT_DEVICEID, m_data[DEVICE_ID]);
			}
			else if(m_data[CMD] == SET_ID)
			{
				SetDlgItemInt(IDC_EDIT_GETID, m_data[DEVICE_ID]);
				SetDlgItemInt(IDC_EDIT_DEVICEID, m_data[DEVICE_ID]);
			}
		}
		m_data = "";
	}
}

void DeviceControlPCView::OnEventWrite( int nWritten )
{

}

void DeviceControlPCView::OnBnClickedButtonRelay1()
{
	UpdateData(TRUE);

	if (m_isRelay1On)
	{
		m_isRelay1On = false;
		SetDlgItemText(IDC_BUTTON_RELAY1, L"RELAY1_ON");
		createCommand(m_nDeviceID, RELAY_OFF, '0');
	}
	else
	{
		m_isRelay1On = true;
		SetDlgItemText(IDC_BUTTON_RELAY1, L"RELAY1_OFF");
		createCommand(m_nDeviceID, RELAY_ON, '0');
	}

	UpdateData(FALSE);
}

void DeviceControlPCView::OnBnClickedButtonRelay2()
{
	UpdateData(TRUE);

	if (m_isRelay2On)
	{
		m_isRelay2On = false;
		SetDlgItemText(IDC_BUTTON_RELAY2, L"RELAY2_ON");
		createCommand(m_nDeviceID, RELAY_OFF, '1');
	}
	else
	{
		m_isRelay2On = true;
		SetDlgItemText(IDC_BUTTON_RELAY2, L"RELAY2_OFF");
		createCommand(m_nDeviceID, RELAY_ON, '1');
	}

	UpdateData(FALSE);
}

void DeviceControlPCView::OnBnClickedButtonRelay3()
{
	UpdateData(TRUE);

	if (m_isRelay3On)
	{
		m_isRelay3On = false;
		SetDlgItemText(IDC_BUTTON_RELAY3, L"RELAY3_ON");
		createCommand(m_nDeviceID, RELAY_OFF, '2');
	}
	else
	{
		m_isRelay3On = true;
		SetDlgItemText(IDC_BUTTON_RELAY3, L"RELAY3_OFF");
		createCommand(m_nDeviceID, RELAY_ON, '2');
	}

	UpdateData(FALSE);
}

void DeviceControlPCView::OnBnClickedButtonRelay4()
{
	UpdateData(TRUE);

	if (m_isRelay4On)
	{
		m_isRelay4On = false;
		SetDlgItemText(IDC_BUTTON_RELAY4, L"RELAY4_ON");
		createCommand(m_nDeviceID, RELAY_OFF, '3');
	}
	else
	{
		m_isRelay4On = true;
		SetDlgItemText(IDC_BUTTON_RELAY4, L"RELAY4_OFF");
		createCommand(m_nDeviceID, RELAY_ON, '3');
	}

	UpdateData(FALSE);
}

void DeviceControlPCView::OnBnClickedButtonRelay5()
{
	UpdateData(TRUE);

	if (m_isRelay5On)
	{
		m_isRelay5On = false;
		SetDlgItemText(IDC_BUTTON_RELAY5, L"RELAY5_ON");
		createCommand(m_nDeviceID, RELAY_OFF, '4');
	}
	else
	{
		m_isRelay5On = true;
		SetDlgItemText(IDC_BUTTON_RELAY5, L"RELAY5_OFF");
		createCommand(m_nDeviceID, RELAY_ON, '4');
	}

	UpdateData(FALSE);
}

void DeviceControlPCView::OnBnClickedButtonRelay6()
{
	UpdateData(TRUE);

	if (m_isRelay6On)
	{
		m_isRelay6On = false;
		SetDlgItemText(IDC_BUTTON_RELAY6, L"RELAY6_ON");
		createCommand(m_nDeviceID, RELAY_OFF, '5');
	}
	else
	{
		m_isRelay6On = true;
		SetDlgItemText(IDC_BUTTON_RELAY6, L"RELAY6_OFF");
		createCommand(m_nDeviceID, RELAY_ON, '5');
	}

	UpdateData(FALSE);
}

void DeviceControlPCView::OnBnClickedButtonRelay7()
{
	UpdateData(TRUE);

	if (m_isRelay7On)
	{
		m_isRelay7On = false;
		SetDlgItemText(IDC_BUTTON_RELAY7, L"RELAY7_ON");
		createCommand(m_nDeviceID, RELAY_OFF, '6');
	}
	else
	{
		m_isRelay7On = true;
		SetDlgItemText(IDC_BUTTON_RELAY7, L"RELAY7_OFF");
		createCommand(m_nDeviceID, RELAY_ON, '6');
	}

	UpdateData(FALSE);
}

void DeviceControlPCView::OnBnClickedButtonRelay8()
{
	UpdateData(TRUE);

	if (m_isRelay8On)
	{
		m_isRelay8On = false;
		SetDlgItemText(IDC_BUTTON_RELAY8, L"RELAY8_ON");
		createCommand(m_nDeviceID, RELAY_OFF, '7');
	}
	else
	{
		m_isRelay8On = true;
		SetDlgItemText(IDC_BUTTON_RELAY8, L"RELAY8_OFF");
		createCommand(m_nDeviceID, RELAY_ON, '7');
	}

	UpdateData(FALSE);
}

void DeviceControlPCView::OnBnClickedButtonSetid()
{
	UpdateData(TRUE);
	createCommand(0, SET_ID, m_nNewID);
	UpdateData(FALSE);
}

void DeviceControlPCView::OnBnClickedButtonGetid()
{
	m_commandType = GET_ID;
	createCommand(0, GET_ID, 0);
}

void DeviceControlPCView::createCommand( int deviceID, CommandType type, int data )
{
	char cmd[10] = {0};
	int header = 0xA0;
	int crc = 0xA0 | deviceID | type | data;
	int end = 0xB0;

	sprintf_s(cmd, sizeof(cmd), "%c%c%c%c%c%c", header, deviceID, type, data, crc, end);
	CString outPut = L"";
	outPut.Format(L"\r\nWRITE:%d %d %d %d %d %d", cmd[0],cmd[1],cmd[2],cmd[3],cmd[4],cmd[5]);

	CMainFrame *frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	frm->m_pBottomRichTextView->Addtext(outPut, false);

	int nTextLen =::GetWindowTextLength(frm->m_pBottomRichTextView->m_hWnd);
	if(nTextLen >= 2000)
		frm->m_pBottomRichTextView->ClearText();

	//WriteLog(cmd);

	Write(cmd, 6);
}

void DeviceControlPCView::OnBnClickedBtCloseport()
{
	ClosePort();
}
