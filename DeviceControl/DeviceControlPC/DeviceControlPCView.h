// GSM_TopUpView.h : interface of the CSmartSimTopUpView class
//

#pragma once
#include "stdafx.h"
#include "DeviceControlPCDoc.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "afxmt.h"
#include "DeviceControlPC.h"
#include "SerialCtrl.h"

enum CommandType {
	RELAY_ON,
	RELAY_OFF,
	GET_ID,
	SET_ID,
	ALL_RELAY_ON,
	ALL_RELAY_OFF
};

class DeviceControlPCView : public CFormView, public CSerialIO
{
protected: // create from serialization only
	DeviceControlPCView();
	DECLARE_DYNCREATE(DeviceControlPCView)

public:
	enum{ IDD = IDD_VTCTOPUPTELCO_FORM };

// Attributes
public:
	DeviceControlPCDoc* GetDocument() const;

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~DeviceControlPCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
public:
	virtual void OnEventOpen(bool bSuccess);
	virtual void OnEventClose(bool bSuccess);
	virtual void OnEventRead(char *inPacket,int inLength);
	virtual void OnEventWrite(int nWritten);

protected:
	DECLARE_MESSAGE_MAP()

private:
	void GetPath(char* path, const char* filename);
	void WriteLog(const char* cmdLog);
	int GetComportNumber( char* comPortName );
	BOOL isFolderExists(CString strFolderName);

    void LogView(const char* log);
	void scanComport();

	afx_msg void OnOpenGSMPort();
	afx_msg void OnScanComport();

	void createCommand(int deviceID, CommandType type, int data);

private:
	CString m_szComPortList;	
	CComboBox m_cbComport;

	bool m_isRelay1On;
	bool m_isRelay2On;
	bool m_isRelay3On;
	bool m_isRelay4On;
	bool m_isRelay5On;
	bool m_isRelay6On;
	bool m_isRelay7On;
	bool m_isRelay8On;


public:
	afx_msg void OnBnClickedButtonRelay1();
	afx_msg void OnBnClickedButtonRelay2();
	afx_msg void OnBnClickedButtonRelay3();
	afx_msg void OnBnClickedButtonRelay4();
	afx_msg void OnBnClickedButtonRelay5();
	afx_msg void OnBnClickedButtonRelay6();
	afx_msg void OnBnClickedButtonRelay7();
	afx_msg void OnBnClickedButtonRelay8();
	afx_msg void OnBnClickedButtonSetid();
	afx_msg void OnBnClickedButtonGetid();

private:
	int m_nDeviceID;
	int m_nNewID;
	int m_nCurrentID;

	CommandType m_commandType;
};

#ifndef _DEBUG 
inline DeviceControlPCDoc* DeviceControlPCView::GetDocument() const
   { return reinterpret_cast<DeviceControlPCDoc*>(m_pDocument); }
#endif

