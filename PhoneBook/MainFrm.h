#pragma once

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------


// Constructor / Destructor
// ----------------
public:
	CMainFrame() noexcept;
	virtual ~CMainFrame();


// Methods
// ----------------
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


// Overrides
// ----------------
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


// Members
// ----------------
protected:  // control bar embedded members
	CMFCToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

};
