// GSM_TopUp.h : main header file for the GSM_TopUp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSmartSimTopUpApp:
// See GSM_TopUp.cpp for the implementation of this class
//

class DeviceControlPCApp : public CWinApp
{
public:
	DeviceControlPCApp();
	void SetText(CString cWindowname);


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern DeviceControlPCApp theApp;