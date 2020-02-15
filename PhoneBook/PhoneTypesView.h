
// PhoneBookView.h : interface of the CPhoneBookView class
//

#pragma once
#include "PhoneBook.h"
#include "PhoneTypesDocument.h"
#include "ErrorHandler.h"
#include "CListSort.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesView

class CPhoneTypesView : public CListView
{
	DECLARE_DYNCREATE(CPhoneTypesView)
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------
private:
	enum ViewColumn
	{
		PhoneTypeColumn,
		IdColumn = -1
	};
	

// Constructor / Destructor
// ----------------
protected:
	CPhoneTypesView() noexcept;
public:
	virtual ~CPhoneTypesView();


// Methods
// ----------------
public:
	CPhoneTypesDocument* GetDocument() const;
	void OnCtxAddPhoneType();
	void OnCtxRemovePhoneType();
	void onCtxEdit();
	void OnCtxPreview();
	void OnReloadButton();
	void Sort();
	void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnContextMenu(CWnd* pWnd, CPoint ptMousePos);
	void DrawPhoneTypes();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Overrides
// ----------------
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual void OnInitialUpdate() override;


// Members
// ----------------
private:
	CMenu m_oCtxMenu;
	ViewColumn m_CurrentSortCriteria = IdColumn;
	SortOrder m_CurrentSortOrder = NoneOrder;
};
