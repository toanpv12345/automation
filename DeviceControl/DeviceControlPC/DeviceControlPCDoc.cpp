// GSM_TopUpDoc.cpp : implementation of the CSmartSimTopUpDoc class
//

#include "stdafx.h"
#include "DeviceControlPC.h"

#include "DeviceControlPCDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmartSimTopUpDoc

IMPLEMENT_DYNCREATE(DeviceControlPCDoc, CDocument)

BEGIN_MESSAGE_MAP(DeviceControlPCDoc, CDocument)
END_MESSAGE_MAP()


// CSmartSimTopUpDoc construction/destruction

DeviceControlPCDoc::DeviceControlPCDoc()
{
	// TODO: add one-time construction code here

}

DeviceControlPCDoc::~DeviceControlPCDoc()
{
}

BOOL DeviceControlPCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSmartSimTopUpDoc serialization

void DeviceControlPCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CSmartSimTopUpDoc diagnostics

#ifdef _DEBUG
void DeviceControlPCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void DeviceControlPCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSmartSimTopUpDoc commands
