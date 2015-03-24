// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DeviceControlPC.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bcreate=false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CRect rc;
	GetClientRect(&rc);
	m_sMainsp.SetRowInfo(1, rc.Height()/3, 0);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_bcreate=true;
	
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	

	return 0;
}



BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;    // no title
    return CFrameWnd::PreCreateWindow(cs);

	//if( !CFrameWnd::PreCreateWindow(cs) )
	//	return FALSE;
	//// TODO: Modify the Window class or styles here by modifying
	////  the CREATESTRUCT cs

	//return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if(!m_sMainsp.CreateStatic(this,2,1))
	{
		TRACE0("Ko Splite duoc \n");
		return FALSE;
	}

	if(!m_sBottom.CreateStatic(&m_sMainsp, 1, 2,WS_VISIBLE|WS_CHILD|WS_BORDER,m_sMainsp.IdFromRowCol(1,0)))
	{
		TRACE0("Ko Tao duoc view\n");
		return FALSE;
	}

	m_sMainsp.CreateView(0,0,RUNTIME_CLASS(DeviceControlPCView),CSize(350,190),pContext);
	
	m_sBottom.CreateView(0,0,RUNTIME_CLASS(CRichTextView),CSize(0,0),pContext);
	m_sBottom.CreateView(0,1,RUNTIME_CLASS(CRichTextView),CSize(0,0),pContext);

	m_gWnd=(CWnd*)(DeviceControlPCView*)m_sMainsp.GetPane(0,0);
	m_pBottomRichTextView=(CRichTextView*)m_sBottom.GetPane(0, 1);
	return TRUE;
}
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	if(m_bcreate)
	{
		CRect rect;
		GetClientRect(rect);

		m_sMainsp.SetRowInfo(1, rect.Height()*3/7, 0);

		//m_sMiddle.SetColumnInfo(0,rect.Width()*3/12,0);

		//m_sMiddle.SetRowInfo(0,rect.Height()/2,0);
		m_sBottom.SetColumnInfo(1,rect.Width() *3/5,0); // 1-1 view, 0-2 view

		m_sMainsp.RecalcLayout();
		//m_sMiddle.RecalcLayout();
		m_sBottom.RecalcLayout();
	}
}

void CMainFrame::OnClose()
{
	TRACE0("Ko Splite duoc \n");
}

