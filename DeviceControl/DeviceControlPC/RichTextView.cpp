// RichTextView.cpp : implementation file
//

#include "stdafx.h"
#include "RichTextView.h"
#include ".\richtextview.h"


// CRichTextView

IMPLEMENT_DYNCREATE(CRichTextView, CRichEditView)

CRichTextView::CRichTextView()
{
	m_bColor=false;
}

CRichTextView::~CRichTextView()
{

}

BEGIN_MESSAGE_MAP(CRichTextView, CRichEditView)
END_MESSAGE_MAP()


// CRichTextView diagnostics

#ifdef _DEBUG
void CRichTextView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CRichTextView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif //_DEBUG


// CRichTextView message handlers


void CRichTextView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();
	HWND hwnw=GetRichEditCtrl().m_hWnd;
	int len=::GetWindowTextLength(hwnw);
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_FACE | CFM_BOLD | CFM_CHARSET;
	cf.bCharSet = DEFAULT_CHARSET;
	_tcscpy(cf.szFaceName, _T("Times New Roman")/*"Times New Roman"*/);
	cf.crTextColor	= RGB(0, 0, 255);
	GetRichEditCtrl().SetDefaultCharFormat(cf);
}

void CRichTextView::Addtext(CString szVal, bool isBlue)
{
	HWND hwnd=GetRichEditCtrl().m_hWnd;
	int len= ::GetWindowTextLength(hwnd);
	CHARFORMAT cf;
	CHARRANGE cr;
	cr.cpMin=len;
	cr.cpMax=len;
	cf.cbSize		= sizeof(CHARFORMAT);
	cf.dwMask		= CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects	= (unsigned long)~( CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	//if(!m_bColor)
	//{
	//	//_tcscpy(cf.szFaceName, _T("Courier New")/*"Times New Roman"*/);
	//	cf.crTextColor	= RGB(0, 0, 255);
	//}
	//else
	//{
	//	//_tcscpy(cf.szFaceName, _T("Times New Roman")/*"Times New Roman"*/);
	//	cf.crTextColor	= RGB(255, 0, 0);
	//}
	if(isBlue)
	{
		//_tcscpy(cf.szFaceName, _T("Courier New")/*"Times New Roman"*/);
		cf.crTextColor	= RGB(0, 0, 255);
	}
	else
	{
		//_tcscpy(cf.szFaceName, _T("Times New Roman")/*"Times New Roman"*/);
		cf.crTextColor	= RGB(255, 0, 0);
	}
	
	//m_bColor=!m_bColor;
	::SendMessage(hwnd, EM_EXSETSEL, 0, (LPARAM)&cr);
	::SendMessage(hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	::SendMessage(hwnd, EM_REPLACESEL, (WPARAM) FALSE, (LPARAM)szVal.GetBuffer());
}

void CRichTextView::ClearText()
{
	CHARRANGE cr;
	HWND hwnd=GetRichEditCtrl().m_hWnd;
	int len= ::GetWindowTextLength(hwnd);
	CHARFORMAT cf;
	cr.cpMin=0;
	cr.cpMax=len;
	cf.cbSize		= sizeof(CHARFORMAT);
	cf.dwMask		= CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects	= (unsigned long)~( CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);

	::SendMessage(hwnd, EM_EXSETSEL, 0, (LPARAM)&cr);
	::SendMessage(hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	::SendMessage(hwnd, EM_REPLACESEL, (WPARAM) FALSE, (LPARAM)"");
}