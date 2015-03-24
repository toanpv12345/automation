// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "Splitter.h"
#include "RichTextView.h"
#include "DeviceControlPCView.h"

class CMainFrame : public CFrameWnd
{
	CSplitter m_sMainsp;
	//CSplitter m_sMiddle;
	CSplitter m_sBottom;
	
	DeviceControlPCView *m_pUp;
	bool m_bcreate;
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	CRichTextView *m_pBottomRichTextView;
	//CRichTextView *m_PReceiverlog;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
};