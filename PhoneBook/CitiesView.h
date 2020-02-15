
// PhoneBookView.h : interface of the CPhoneBookView class
//

#pragma once
#include "PhoneBook.h"
#include "CitiesDocument.h"
#include "ErrorHandler.h"
#include "CListSort.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesView

class CCitiesView : public CListView
{
	DECLARE_DYNCREATE(CCitiesView)
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------
private:
	enum ViewColumn
	{
		CityColumn,
		RegionColumn,
		IdColumn = -1
	};

// Constructor / Destructor
// ----------------
protected:
	CCitiesView() noexcept;
public:
	virtual ~CCitiesView();


// Methods
// ----------------
public:
	CCitiesDocument* GetDocument() const;
	void OnCtxAddCity();
	void OnCtxRemoveCity();
	void onCtxEdit();
	void OnCtxPreview();
	void OnReloadButton();
	void Sort();
	void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnContextMenu(CWnd* pWnd, CPoint ptMousePos);
	void DrawCities();
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
