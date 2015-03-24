

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Splitter.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// Modify by Ha Minh Nham
// Haminhamht@yahoo.com ,haminhamht@gmail.com1
// CSplitter window
#define CX_BORDER   1
#define CY_BORDER   1

class CSplitter : public CSplitterWnd
{
// Construction
public:
	CSplitter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitter)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CWnd *SetPanelView( int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext = NULL);
	virtual CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
	virtual ~CSplitter();
	// Generated message map functions
protected:
	virtual void StartTracking(int ht);

	//{{AFX_MSG(CSplitter)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnPaint();
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	virtual void RecalcLayout();    // call after changing sizes
	static void DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
		CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar);
	static void LayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray,
		int nMax, int nSize, int nSizeSplitter);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


