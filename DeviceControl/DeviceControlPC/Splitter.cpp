// Splitter.cpp : implementation file
//

#include "stdafx.h"
#include "Splitter.h"
#include ".\splitter.h"
//#include "afximpl.h"
#include <afxpriv.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitter
extern enum HitTestValue
{
	noHit                   = 0,
	vSplitterBox            = 1,
	hSplitterBox            = 2,
	bothSplitterBox         = 3,        // just for keyboard
	vSplitterBar1           = 101,
	vSplitterBar15          = 115,
	hSplitterBar1           = 201,
	hSplitterBar15          = 215,
	splitterIntersection1   = 301,
	splitterIntersection225 = 525
};


CSplitter::CSplitter()
{
	m_cxSplitter = m_cySplitter = 3 + 1 + 1+2;
	m_cxBorderShare = m_cyBorderShare = 0;
	m_cxSplitterGap = m_cySplitterGap = 7;
	m_cxBorder = m_cyBorder = 1;
}

CSplitter::~CSplitter()
{
}


BEGIN_MESSAGE_MAP(CSplitter, CSplitterWnd)
	//{{AFX_MSG_MAP(CSplitter)
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSplitter message handlers

CWnd *CSplitter::SetPanelView(int row, int col, CRuntimeClass *pViewClass, SIZE sizeInit, CCreateContext *pContext)
{
#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(row >= 0 && row < m_nRows);
	ASSERT(col >= 0 && col < m_nCols);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	if (GetDlgItem(IdFromRowCol(row, col)) != NULL)
	{
		TRACE2("Error: CreateView - pane already exists for row %d, col %d.\n",
			row, col);
		ASSERT(FALSE);
		return FALSE;
	}
#endif

	// set the initial size for that pane
	m_pColInfo[col].nIdealSize = sizeInit.cx;
	m_pRowInfo[row].nIdealSize = sizeInit.cy;

	BOOL bSendInitialUpdate = FALSE;

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected
		//  client if possible
		CView* pOldView = (CView*)GetActivePane();
		if (pOldView != NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// set info about last pane
			ASSERT(contextT.m_pCurrentFrame == NULL);
			contextT.m_pLastView = pOldView;
			contextT.m_pCurrentDoc = pOldView->GetDocument();
			if (contextT.m_pCurrentDoc != NULL)
				contextT.m_pNewDocTemplate =
				  contextT.m_pCurrentDoc->GetDocTemplate();
		}
		pContext = &contextT;
		bSendInitialUpdate = TRUE;
	}

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a splitter pane.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created

/*	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	if (afxData.bWin4)
		dwStyle &= ~WS_BORDER;

	// Create with the right size (wrong position)
	CRect rect(CPoint(0,0), sizeInit);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, this, IdFromRowCol(row, col), pContext))
	{
		TRACE0("Warning: couldn't create client pane for splitter.\n");
			// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	ASSERT((int)_AfxGetDlgCtrlID(pWnd->m_hWnd) == IdFromRowCol(row, col));

	// send initial notification message
	if (bSendInitialUpdate)
		pWnd->SendMessage(WM_INITIALUPDATE);

*/	return NULL;
}

BOOL CSplitter::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pItem = (NMHDR*) lParam;
	if (CWnd::OnNotify(wParam, lParam, pResult))
		return TRUE;
	CWnd *p = GetParent();
//	if(p == NULL) p = GetOwner();
	pItem->hwndFrom = m_hWnd;
	//pItem->idFrom = GetDlgCtrlID();
	*pResult = p->SendMessage(WM_NOTIFY, wParam, lParam);
	return TRUE;
}

CWnd* CSplitter::GetActivePane(int* pRow, int* pCol)
{
	ASSERT_VALID(this);

	// attempt to use active view of frame window
	CWnd* pView = NULL;
	CFrameWnd* pFrameWnd = GetParentFrame();
	if(pFrameWnd)
		pView = pFrameWnd->GetActiveView();

	// failing that, use the current focus
	if (pView == NULL)
		pView = GetFocus();

	// make sure the pane is a child pane of the splitter
	if (pView != NULL && !IsChildPane(pView, pRow, pCol))
		pView = NULL;

	return pView;
}

BOOL CSplitter::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (CWnd::OnCommand(wParam, lParam))
		return TRUE;
	CFrameWnd* pFrameWnd = GetParentFrame();
/*	if(pFrameWnd)
		return pFrameWnd->SendMessage(WM_COMMAND, wParam, lParam);
*/	return GetParent()->SendMessage(WM_COMMAND, wParam, lParam);
}

void CSplitter::StartTracking(int ht)
{
	ASSERT_VALID(this);
	if (ht == noHit)
		return;

	// GetHitRect will restrict 'm_rectLimit' as appropriate
	GetInsideRect(m_rectLimit);

	if (ht >= splitterIntersection1 && ht <= splitterIntersection225)
	{
		// split two directions (two tracking rectangles)
		int row = (ht - splitterIntersection1) / 15;
		int col = (ht - splitterIntersection1) % 15;

		GetHitRect(row + vSplitterBar1, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = TRUE;
		GetHitRect(col + hSplitterBar1, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;
	}
	else if (ht == bothSplitterBox)
	{
		// hit on splitter boxes (for keyboard)
		GetHitRect(vSplitterBox, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = TRUE;
		GetHitRect(hSplitterBox, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;

		// center it
		m_rectTracker.OffsetRect(0, m_rectLimit.Height()/2);
		m_rectTracker2.OffsetRect(m_rectLimit.Width()/2, 0);
	}
	else
	{
		// only hit one bar
		GetHitRect(ht, m_rectTracker);
	}

	// allow active view to preserve focus before taking it away
/*	CView* pView = (CView*)GetActivePane();
	if (pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CView)))
	{
		ASSERT_VALID(pView);
		CFrameWnd* pFrameWnd = GetParentFrame();
		if(pFrameWnd)
			pView->SendMessage(
		pView->OnActivateFrame(WA_INACTIVE, pFrameWnd);
	}
*/
	// steal focus and capture
	SetCapture();
	SetFocus();

	// make sure no updates are pending
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);

	// set tracking state and appropriate cursor
	m_bTracking = TRUE;
	OnInvertTracker(m_rectTracker);
	if (m_bTracking2)
		OnInvertTracker(m_rectTracker2);
	m_htTrack = ht;
	SetSplitCursor(ht);
}

//void CSplitter::OnPaint()
//{
//	ASSERT_VALID(this);
//	CPaintDC dc(this);
//
//	CRect rectClient;
//	GetClientRect(&rectClient);
//	CRect rectInside;
//	GetInsideRect(rectInside);
//	
//	rectInside.InflateRect(4,4);
//	DrawAllSplitBars(&dc, rectInside.right, rectInside.bottom);
//	// draw splitter intersections (inside only)
//	GetInsideRect(rectInside);
//	dc.IntersectClipRect(rectInside);
//	CRect rect;
//	rect.top = rectInside.top;
//	for (int row = 0; row < m_nRows-1 ; row++)
//	{
//		rect.top += m_pRowInfo[row].nCurSize + m_cyBorderShare;
//		rect.bottom = rect.top + m_cySplitter;
//		rect.left = rectInside.left;
//		for (int col = 0; col < m_nCols ; col++)
//		{
//			rect.left += m_pColInfo[col].nCurSize + m_cxBorderShare;
//			rect.right = rect.left + m_cxSplitter;
//			OnDrawSplitter(&dc, splitIntersection, rect);
//			rect.left = rect.right + m_cxBorderShare;
//		}
//		rect.top = rect.bottom + m_cxBorderShare;
//	}
//}

void CSplitter::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
	if((nType != splitBorder) || (pDC == NULL))
	{
		CSplitterWnd::OnDrawSplitter(pDC, nType, rect);
		return;
	}

	ASSERT_VALID(pDC);

	pDC->Draw3dRect(rect, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHIGHLIGHT));
	//ASSERT_VALID(pDC);
	//CSplitterWnd::OnDrawSplitter(pDC, nType, rect);	
	CRect rc=rect;
	//pDC->Draw3dRect(rc, afxData.clrBtnFace, afxData.clrBtnFace);
	//rc.InflateRect(-CX_BORDER, -CY_BORDER);
//	pDC->Draw3dRect(rc, afxData.clrBtnShadow, afxData.clrBtnHilite);

	return ;
}


void CSplitter::LayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray,
									int nMax, int nSize, int nSizeSplitter)
{
	ASSERT(pInfoArray != NULL);
	ASSERT(nMax > 0);
	ASSERT(nSizeSplitter > 0);

	CSplitterWnd::CRowColInfo* pInfo;
	int i;

	if (nSize < 0)
		nSize = 0;  // if really too small, layout as zero size

	// start with ideal sizes
	for (i = 0, pInfo = pInfoArray; i < nMax-1; i++, pInfo++)
	{
		if (pInfo->nIdealSize < pInfo->nMinSize)
			pInfo->nIdealSize = 0;      // too small to see
		pInfo->nCurSize = pInfo->nIdealSize;
	}
	pInfo->nCurSize = INT_MAX;  // last row/column takes the rest

	for (i = 0, pInfo = pInfoArray; i < nMax; i++, pInfo++)
	{
		ASSERT(nSize >= 0);
		if (nSize == 0)
		{
			// no more room (set pane to be invisible)
			pInfo->nCurSize = 0;
			continue;       // don't worry about splitters
		}
		else if (nSize < pInfo->nMinSize && i != 0)
		{
			// additional panes below the recommended minimum size
			//   aren't shown and the size goes to the previous pane
			pInfo->nCurSize = 0;

			// previous pane already has room for splitter + border
			//   add remaining size and remove the extra border
			(pInfo-1)->nCurSize += nSize + 2;
			nSize = 0;
		}
		else
		{
			// otherwise we can add the second pane
			ASSERT(nSize > 0);
			if (pInfo->nCurSize == 0)
			{
				// too small to see
				if (i != 0)
					pInfo->nCurSize = 0;
			}
			else if (nSize < pInfo->nCurSize)
			{
				// this row/col won't fit completely - make as small as possible
				pInfo->nCurSize = nSize;
				nSize = 0;
			}
			else
			{
				// can fit everything
				nSize -= pInfo->nCurSize;
			}
		}

		// see if we should add a splitter
		ASSERT(nSize >= 0);
		if (i != nMax - 1)
		{
			// should have a splitter
			if (nSize > nSizeSplitter)
			{
				nSize -= nSizeSplitter; // leave room for splitter + border
				ASSERT(nSize > 0);
			}
			else
			{
				// not enough room - add left over less splitter size
				pInfo->nCurSize += nSize;
				if (pInfo->nCurSize > (nSizeSplitter - 2))
					pInfo->nCurSize -= (nSizeSplitter - 2);
				nSize = 0;
			}
		}
	}
	ASSERT(nSize == 0); // all space should be allocated
}

/* Original Method Implementation:
*
* AFX_STATIC void AFXAPI _AfxDeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
*	CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar)
*
* In WINSPLIT.CPP
*
* Credit for this adaptation goes to Kris.
*/
// repositions client area of specified window
// assumes everything has WS_BORDER or is inset like it does
//  (includes scroll bars)
void CSplitter::DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
									  CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar)
{
	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd != NULL);

	if (bScrollBar)
	{
		// if there is enough room, draw scroll bar without border
		// if there is not enough room, set the WS_BORDER bit so that
		//   we will at least get a proper border drawn
		BOOL bNeedBorder = (cx <= 1 || cy <= 1);
		pWnd->ModifyStyle(bNeedBorder ? 0 : 1, bNeedBorder ? 1 : 0);
	}
	CRect rect(x, y, x+cx, y+cy);

	// adjust for 3d border (splitter windows have implied border)
	if ((pWnd->GetExStyle() & WS_EX_CLIENTEDGE) || pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
	{
		rect.InflateRect(1, 1); // for proper draw CSplitter in view
		//		rect.InflateRect(afxData.cxBorder2, afxData.cyBorder2);
	}

	// first check if the new rectangle is the same as the current
	CRect rectOld;
	pWnd->GetWindowRect(rectOld);
	pWnd->GetParent()->ScreenToClient(&rectOld);
	if (rect != rectOld)
		AfxRepositionWindow(lpLayout, pWnd->m_hWnd, rect);
}

void CSplitter::RecalcLayout()
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 0 && m_nCols > 0); // must have at least one pane

	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);

	CRect rectInside;
	GetInsideRect(rectInside);

	// layout columns (restrict to possible sizes)
	LayoutRowCol(m_pColInfo, m_nCols, rectInside.Width(), m_cxSplitterGap);
	LayoutRowCol(m_pRowInfo, m_nRows, rectInside.Height(), m_cySplitterGap);

	// adjust the panes (and optionally scroll bars)

	// give the hint for the maximum number of HWNDs
	AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1) + 1);

	// size of scrollbars
	int cx = rectClient.right - rectInside.right;
	int cy = rectClient.bottom - rectInside.bottom;

	// reposition size box
	if (m_bHasHScroll && m_bHasVScroll)
	{
		CWnd* pScrollBar = GetDlgItem(AFX_IDW_SIZE_BOX);
		ASSERT(pScrollBar != NULL);

		// fix style if necessary
		BOOL bSizingParent = (GetSizingParent() != NULL);
		// modifyStyle returns TRUE if style changes
		if (pScrollBar->ModifyStyle(SBS_SIZEGRIP|SBS_SIZEBOX,
			bSizingParent ? SBS_SIZEGRIP : SBS_SIZEBOX))
			pScrollBar->Invalidate();
		pScrollBar->EnableWindow(bSizingParent);

		// reposition the size box
		DeferClientPos(&layout, pScrollBar,
			rectInside.right,
			rectInside.bottom, cx, cy, TRUE);
	}

	// reposition scroll bars
	if (m_bHasHScroll)
	{
		int cxSplitterBox = m_cxSplitter;// split box bigger
		int x = rectClient.left;
		int y = rectInside.bottom;
		for (int col = 0; col < m_nCols; col++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_HSCROLL_FIRST + col);
			ASSERT(pScrollBar != NULL);
			int cx = m_pColInfo[col].nCurSize;
			if (col == 0 && m_nCols < m_nMaxCols)
				x += cxSplitterBox, cx -= cxSplitterBox;
			DeferClientPos(&layout, pScrollBar, x, y, cx, cy, TRUE);
			x += cx + m_cxSplitterGap;
		}
	}

	if (m_bHasVScroll)
	{
		int cySplitterBox = m_cySplitter;// split box bigger
		int x = rectInside.right;
		int y = rectClient.top;
		for (int row = 0; row < m_nRows; row++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_VSCROLL_FIRST + row);
			ASSERT(pScrollBar != NULL);
			int cy = m_pRowInfo[row].nCurSize;
			if (row == 0 && m_nRows < m_nMaxRows)
				y += cySplitterBox, cy -= cySplitterBox;
			DeferClientPos(&layout, pScrollBar, x, y, cx, cy, TRUE);
			y += cy + m_cySplitterGap;
		}
	}

	//BLOCK: Reposition all the panes
	{
		int x = rectClient.left;
		for (int col = 0; col < m_nCols; col++)
		{
			int cx = m_pColInfo[col].nCurSize;
			int y = rectClient.top;
			for (int row = 0; row < m_nRows; row++)
			{
				int cy = m_pRowInfo[row].nCurSize;
				CWnd* pWnd = GetPane(row, col);
				DeferClientPos(&layout, pWnd, x, y, cx, cy, FALSE);
				y += cy + m_cySplitterGap;
			}
			x += cx + m_cxSplitterGap;
		}
	}

	// move and resize all the windows at once!
	if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
		TRACE0("Warning: DeferWindowPos failed - low system resources.\n");

	// invalidate all the splitter bars (with NULL pDC)
	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);
}

