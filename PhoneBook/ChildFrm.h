#pragma once

/////////////////////////////////////////////////////////////////////////////
// CChildFrame
class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------


// Constructor / Destructor
// ----------------
public:
	CChildFrame() noexcept;
	virtual ~CChildFrame();


// Methods
// ----------------
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Overrides
// ----------------
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


// Members
// ----------------
protected:
	CSplitterWnd m_wndSplitter;
};
