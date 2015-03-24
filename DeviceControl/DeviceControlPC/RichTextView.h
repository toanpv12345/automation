#pragma once


// CRichTextView view
#include <afxrich.h>
class CRichTextView : public CRichEditView
{
	bool m_bColor;
	DECLARE_DYNCREATE(CRichTextView)

protected:
	CRichTextView();           // protected constructor used by dynamic creation
	virtual ~CRichTextView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void Addtext(CString szVal, bool isBlue);
	void ClearText();
};


