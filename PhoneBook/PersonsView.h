
// PhoneBookView.h : interface of the CPhoneBookView class
//

#pragma once
#include "PhoneBook.h"
#include "PersonsDocument.h"
#include "ErrorHandler.h"
#include "CListSort.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsView

class CPersonsView : public CListView
{
	DECLARE_DYNCREATE(CPersonsView)
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------
private:
	enum ViewColumn
	{
		FirstNameColumn,
		MiddleNameColumn,
		LastNameColumn,
		IdentificationNumberColumn,
		CityColumn,
		AddressColumn,
		IdColumn = -1
	};

// Constructor / Destructor
// ----------------
protected:
	CPersonsView() noexcept;
public:
	virtual ~CPersonsView();


// Methods
// ----------------
public:
	CPersonsDocument* GetDocument() const;
	void GetCityRegionFromMapWhereID(long lCityID, CString& strCityName) const;
	void OnCtxAddPerson();
	void OnCtxRemovePerson();
	void onCtxEdit();
	void OnCtxPreview();
	void OnReloadButton();
	void Sort();
	void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnContextMenu(CWnd* pWnd, CPoint ptMousePos);
	void DrawPersons();
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
