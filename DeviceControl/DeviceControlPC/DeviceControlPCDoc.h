// GSM_TopUpDoc.h : interface of the CSmartSimTopUpDoc class
//


#pragma once


class DeviceControlPCDoc : public CDocument
{
protected: // create from serialization only
	DeviceControlPCDoc();
	DECLARE_DYNCREATE(DeviceControlPCDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~DeviceControlPCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


