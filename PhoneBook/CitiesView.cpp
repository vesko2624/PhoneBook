
#include "pch.h"
#include "CitiesView.h"
#include "CitiesDlg.h"
#include "ErrorHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CCitiesView

IMPLEMENT_DYNCREATE(CCitiesView, CListView)

BEGIN_MESSAGE_MAP(CCitiesView, CListView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_RELOAD_BUTTON, &CCitiesView::OnReloadButton)

	ON_NOTIFY(HDN_ITEMCLICKA, 0, &CCitiesView::OnHeaderClicked)
	ON_NOTIFY(HDN_ITEMCLICKW, 0, &CCitiesView::OnHeaderClicked)

	ON_COMMAND(ID_MENU_INSERT, &CCitiesView::OnCtxAddCity)
	ON_COMMAND(ID_MENU_DELETE, &CCitiesView::OnCtxRemoveCity)
	ON_COMMAND(ID_MENU_PREVIEW, &CCitiesView::OnCtxPreview)
	ON_COMMAND(ID_MENU_EDIT, &CCitiesView::onCtxEdit)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------
CCitiesView::CCitiesView() noexcept
{
	m_oCtxMenu.CreateMenu();
	CMenu oSubMenu;
	oSubMenu.LoadMenu(IDR_MENU1);
	m_oCtxMenu.AppendMenu(MF_POPUP, (UINT_PTR)oSubMenu.m_hMenu, _T(""));
	oSubMenu.Detach();
}

CCitiesView::~CCitiesView()
{

}


// Methods
// ----------------

#ifdef _DEBUG
	CCitiesDocument* CCitiesView::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCitiesDocument)));
		return (CCitiesDocument*)m_pDocument;
	}
#endif

void CCitiesView::OnCtxAddCity()
{
	CITIES recSelectedCity;
	CCitiesDlg oCitiesDlg(recSelectedCity, true);
	if (oCitiesDlg.DoModal() == IDCANCEL)
	{
		return;
	}

	CCitiesDocument* pCitiesDocument = GetDocument();
	if (pCitiesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	if (pCitiesDocument->InsertCity(recSelectedCity) == false)
	{
		MessageBox(_T("City with this name/region already exists."));
		return;
	}
}

void CCitiesView::OnCtxRemoveCity()
{
	CCitiesDocument* pCitiesDocument = GetDocument();
	if (pCitiesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	CListCtrl& oListCtrl = GetListCtrl();
	int nCurrentRow = oListCtrl.GetSelectionMark();
	long lID = static_cast<long>(oListCtrl.GetItemData(nCurrentRow));

	if (pCitiesDocument->DeleteWhereID(lID) == false)
	{
		MessageBox(_T("This City is either not existing and you need to reload, or it cannot be deleted because something depends on it"));
		return;
	}
}

void CCitiesView::onCtxEdit()
{
	CListCtrl& oListCtrl = GetListCtrl();
	int nCurrentRow = oListCtrl.GetSelectionMark();

	if (nCurrentRow == -1)
		return;

	CCitiesDocument* pCitiesDocument = GetDocument();
	if (pCitiesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	long lID = static_cast<long>(oListCtrl.GetItemData(nCurrentRow));
	CITIES recSelectedCity;

	if (pCitiesDocument->SelectWhereIDFromMap(lID, recSelectedCity) == false)
	{
		MessageBox(_T("This city doesn't exists anymore."));
		pCitiesDocument->RemoveCityFromMap(lID);
		return;
	}
	CCitiesDlg oCitiesDlg(recSelectedCity, true);

	if (oCitiesDlg.DoModal() == IDCANCEL)
	{
		return;
	}

	if (pCitiesDocument->UpdateWhereID(recSelectedCity.lID, recSelectedCity) == false)
	{
		MessageBox(_T("City with this name/region already exists or the record was already updated and you need to reload."));
		return;
	}
}

void CCitiesView::OnCtxPreview()
{
	CCitiesDocument* pCitiesDocument = GetDocument();
	if (pCitiesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	CListCtrl& oListCtrl = GetListCtrl();
	int nCurrentRow = oListCtrl.GetSelectionMark();
	if (nCurrentRow == -1)
		return;

	long lID = static_cast<long>(oListCtrl.GetItemData(nCurrentRow));
	CITIES recSelectedCity;

	if (pCitiesDocument->SelectWhereIDFromMap(lID, recSelectedCity) == false)
	{
		MessageBox(_T("This city doesn't exists anymore."));
		pCitiesDocument->RemoveCityFromMap(lID);
		return;
	}
	CCitiesDlg oCitiesDlg(recSelectedCity, false);
	oCitiesDlg.DoModal();
}

void CCitiesView::OnReloadButton()
{
	CCitiesDocument* pCitiesDocument = GetDocument();
	if (pCitiesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	pCitiesDocument->UpdateAllViews(NULL, OnReload, NULL);
}

void CCitiesView::Sort()
{
	CListCtrl& oListCtrl = GetListCtrl();
	SortParams recSortParams(oListCtrl, m_CurrentSortCriteria, m_CurrentSortOrder);

	oListCtrl.SortItemsEx(ListCompareFunc, (LPARAM)&recSortParams);
}

void CCitiesView::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	CListCtrl& oListCtrl = GetListCtrl();
	HD_NOTIFY* phdn = (HD_NOTIFY*)pNMHDR;
	if (phdn == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	int nSelectedColumn = phdn->iItem;
	if (m_CurrentSortCriteria != (ViewColumn)nSelectedColumn)
	{
		m_CurrentSortCriteria = (ViewColumn)nSelectedColumn;
		m_CurrentSortOrder = NoneOrder;
	}

	if (m_CurrentSortOrder == AscendingOrder)
	{
		m_CurrentSortOrder = DescendingOrder;
	}
	else if(m_CurrentSortOrder == DescendingOrder)
	{
		m_CurrentSortCriteria = IdColumn;
		m_CurrentSortOrder = NoneOrder;
	}
	else
	{
		m_CurrentSortOrder = AscendingOrder;
	}

	Sort();
}

void CCitiesView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnCtxPreview();
}

void CCitiesView::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
{
	CListCtrl& oListCtrl = GetListCtrl();
	CHeaderCtrl* pHeader = oListCtrl.GetHeaderCtrl();
	if (pHeader == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	CPoint hitPoint = ptMousePos;
	ScreenToClient(&hitPoint);

	HDHITTESTINFO hitTestHeader = { 0 };
	hitTestHeader.pt = hitPoint;
	if (pHeader->HitTest(&hitTestHeader) != -1)
        return;

	CMenu* pPopUp = nullptr;
	pPopUp = m_oCtxMenu.GetSubMenu(0);
	if (oListCtrl.GetSelectedCount() >= 1)
	{
		pPopUp->EnableMenuItem(ID_MENU_INSERT, MF_DISABLED | MF_GRAYED);
		pPopUp->EnableMenuItem(ID_MENU_EDIT, MF_ENABLED);
		pPopUp->EnableMenuItem(ID_MENU_PREVIEW, MF_ENABLED);
		pPopUp->EnableMenuItem(ID_MENU_DELETE, MF_ENABLED);
	}
	else
	{
		pPopUp->EnableMenuItem(ID_MENU_INSERT, MF_ENABLED);
		pPopUp->EnableMenuItem(ID_MENU_EDIT, MF_DISABLED | MF_GRAYED);
		pPopUp->EnableMenuItem(ID_MENU_PREVIEW, MF_DISABLED | MF_GRAYED);
		pPopUp->EnableMenuItem(ID_MENU_DELETE, MF_DISABLED | MF_DISABLED);
	}

	if (pPopUp)
		pPopUp->TrackPopupMenu(TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this);
}

void CCitiesView::DrawCities()
{
	CListCtrl& oListCtrl = GetListCtrl();
	CCitiesDocument* pCitiesDocument = GetDocument();
	if (pCitiesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	oListCtrl.DeleteAllItems();
	const CMapCitiesByID& oCitiesMapByID = pCitiesDocument->GetCitiesMap(); // DOC
	oListCtrl.SetItemCount(oCitiesMapByID.GetCount());

	int nListPos = 0;
	POSITION oCitiesMapItter = oCitiesMapByID.GetStartPosition();
	while (oCitiesMapItter)
	{
		long lID;
		CITIES* pCityInMap;
		oCitiesMapByID.GetNextAssoc(oCitiesMapItter, lID, pCityInMap);
		if (pCityInMap == NULL)
		{
			NullPointerCaught(__LINE__, _T(__FILE__));
			pCitiesDocument->RemoveCityFromMap(lID);
		}
		oListCtrl.InsertItem(nListPos, pCityInMap->szName);
		oListCtrl.SetItemText(nListPos, RegionColumn, pCityInMap->szRegion);
		oListCtrl.SetItemData(nListPos, static_cast<DWORD_PTR>(pCityInMap->lID));
		nListPos += 1;
	}
}


// Overrides
// ----------------

void CCitiesView::OnDraw(CDC* /*pDC*/)
{
	CCitiesDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

void CCitiesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CListCtrl& oListCtrl = GetListCtrl();
	CCitiesDocument* pCitiesDocument = GetDocument();
	if (pCitiesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	CITIES recCity;
	// Reload everything
	if (lHint == OnReload)
	{
		pCitiesDocument->LoadCities();
		DrawCities();
	}
	else
	{
		if (pHint == NULL)
		{
			NullPointerCaught(__LINE__, _T(__FILE__));
			return;
		}
		recCity = *reinterpret_cast<CITIES*>(pHint);
	}

	if (lHint == OnEdit)
	{
		for (int i = 0; i < oListCtrl.GetItemCount(); ++i)
		{
			if (oListCtrl.GetItemData(i) == recCity.lID)
			{
				oListCtrl.SetItemText(i, CityColumn, recCity.szName);
				oListCtrl.SetItemText(i, RegionColumn, recCity.szRegion);
				break;
			}
		}
	}
	if (lHint == OnInsert)
	{
		int nItemPos = oListCtrl.InsertItem(oListCtrl.GetItemCount(), _T(""));
		oListCtrl.SetItemText(nItemPos, CityColumn, recCity.szName);
		oListCtrl.SetItemText(nItemPos, RegionColumn, recCity.szRegion);
		oListCtrl.SetItemData(nItemPos, recCity.lID);
	}
	if (lHint == OnDelete)
	{
		for (int i = 0; i < oListCtrl.GetItemCount(); ++i)
		{
			if (oListCtrl.GetItemData(i) == recCity.lID)
			{
				oListCtrl.DeleteItem(i);
				break;
			}
		}
		return;
	}
	Sort();
}

BOOL CCitiesView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CListView::PreCreateWindow(cs);
}


void CCitiesView::OnInitialUpdate()
{
	CListCtrl& oListCtrl = GetListCtrl();

	oListCtrl.ModifyStyle(LVS_LIST, LVS_REPORT);
	oListCtrl.SetExtendedStyle(LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	oListCtrl.ModifyStyle(NULL, LVS_SINGLESEL, 0);
	oListCtrl.InsertColumn(CityColumn, _T("Град"), LVCFMT_LEFT, 100);
	oListCtrl.InsertColumn(RegionColumn, _T("Регион"), LVCFMT_LEFT, 100);

	DrawCities();
	Sort();
}


#ifdef _DEBUG
	void CCitiesView::AssertValid() const
	{
		CListView::AssertValid();
	}

	void CCitiesView::Dump(CDumpContext& dc) const
	{
		CListView::Dump(dc);
	}
#endif