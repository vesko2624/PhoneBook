
#include "pch.h"
#include "PersonsView.h"
#include "PersonsDlg.h"
#include "ErrorHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CCitiesView

IMPLEMENT_DYNCREATE(CPersonsView, CListView)

BEGIN_MESSAGE_MAP(CPersonsView, CListView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_RELOAD_BUTTON, &CPersonsView::OnReloadButton)

	ON_NOTIFY(HDN_ITEMCLICKA, 0, &CPersonsView::OnHeaderClicked)
	ON_NOTIFY(HDN_ITEMCLICKW, 0, &CPersonsView::OnHeaderClicked)

	ON_COMMAND(ID_MENU_INSERT, &CPersonsView::OnCtxAddPerson)
	ON_COMMAND(ID_MENU_DELETE, &CPersonsView::OnCtxRemovePerson)
	ON_COMMAND(ID_MENU_PREVIEW, &CPersonsView::OnCtxPreview)
	ON_COMMAND(ID_MENU_EDIT, &CPersonsView::onCtxEdit)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------
CPersonsView::CPersonsView() noexcept
{
	m_oCtxMenu.CreateMenu();
	CMenu oSubMenu;
	oSubMenu.LoadMenu(IDR_MENU1);
	m_oCtxMenu.AppendMenu(MF_POPUP, (UINT_PTR)oSubMenu.m_hMenu, _T(""));
	oSubMenu.Detach();
}

CPersonsView::~CPersonsView()
{

}


// Methods
// ----------------

#ifdef _DEBUG
CPersonsDocument* CPersonsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPersonsDocument)));
	return (CPersonsDocument*)m_pDocument;
}
#endif

void CPersonsView::GetCityRegionFromMapWhereID(long lCityID, CString& strCityRegionName) const
{
	CPersonsDocument* pPersonsDocument = GetDocument();
	if (pPersonsDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	bool bResult;
	CITIES* pCity;
	bResult = pPersonsDocument->SelectCityWhereIDFromMap(lCityID, pCity);
	if (bResult)
	{
		strCityRegionName.Format(_T("%s, %s"), pCity->szRegion, pCity->szName);
	}
	else
	{
		strCityRegionName = _T("Error, not found");
	}
}

void CPersonsView::OnCtxAddPerson()
{
	CPersonsDocument* pPersonsDocument = GetDocument();
	if (pPersonsDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	const CMapPhoneTypesByID& oPhoneTypesMapByID = pPersonsDocument->GetPhoneTypesMapByID();
	const CMapCitiesByNameMapID& oCitiesMapByNameMapID = pPersonsDocument->GetCitiesMapByName();
	PersonInfo oPersonInfo;
	
	CPersonsDlg oPersonsDlg(oPersonInfo, oCitiesMapByNameMapID, oPhoneTypesMapByID, false);
	INT_PTR nDlgResult;

	pPersonsDocument->LoadPhoneTypes();
	nDlgResult = oPersonsDlg.DoModal();
	
	if (nDlgResult == IDCANCEL)
	{
		return;
	}

	bool bResult = pPersonsDocument->InsertPair(oPersonInfo);
	bResult;
}

void CPersonsView::OnCtxRemovePerson()
{
	CPersonsDocument* pPersonsDocument = GetDocument();
	if (pPersonsDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	CListCtrl& oListCtrl = GetListCtrl();
	int nSelectedItem = oListCtrl.GetSelectionMark();

	if (nSelectedItem == -1)
		return;

	bool bResult;
	long lID = static_cast<long>(oListCtrl.GetItemData(nSelectedItem));

	bResult = pPersonsDocument->DeletePersonWhereID(lID);
	if (bResult == false)
	{
		MessageBox(_T("Something happened. Maybe the person was already deleted and you should refresh."));
		return;
	}
}

void CPersonsView::onCtxEdit()
{
	CPersonsDocument* pPersonsDocument = GetDocument();
	if (pPersonsDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	CListCtrl& oListCtrl = GetListCtrl();
	int nSelectedItem = oListCtrl.GetSelectionMark();

	if (nSelectedItem == -1)
		return;

	bool bResult;
	long lID = static_cast<long>(oListCtrl.GetItemData(nSelectedItem));
	PersonInfo oPersonInfo;

	bResult = pPersonsDocument->SelectPersonWhereID(lID, oPersonInfo.recPerson);
	if (bResult == false)
	{
		MessageBox(_T("Error, person not found"));
		return;
	}
	
	const CMapPhoneTypesByID& oPhoneTypesPtrArray = pPersonsDocument->GetPhoneTypesMapByID();
	const CMapCitiesByNameMapID& oCitiesMapByNameMapID = pPersonsDocument->GetCitiesMapByName();

	pPersonsDocument->SelectAllPhoneNumers(lID, oPersonInfo.oPhoneNumbersMapByID);
	
	CPersonsDlg oPersonsDlg(oPersonInfo, oCitiesMapByNameMapID, oPhoneTypesPtrArray, false);
	
	pPersonsDocument->LoadPhoneTypes();
	INT_PTR bUserAction = oPersonsDlg.DoModal();
	if (bUserAction == IDCANCEL)
	{
		return;
	}
	
	pPersonsDocument->EditPersonInfo(oPersonInfo);
}

void CPersonsView::OnCtxPreview()
{
	CListCtrl& oListCtrl = GetListCtrl();
	int nSelectedItem = oListCtrl.GetSelectionMark();
	if (nSelectedItem == -1)
		return;

	CPersonsDocument* pPersonsDocument = GetDocument();
	if (pPersonsDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	bool bResult;
	long lID = static_cast<long>(oListCtrl.GetItemData(nSelectedItem));
	PersonInfo oPersonInfo;

	bResult = pPersonsDocument->SelectPersonWhereID(lID, oPersonInfo.recPerson);
	if (bResult == false)
	{
		MessageBox(_T("Error, person not found"));
		return;
	}

	bResult = pPersonsDocument->SelectAllPhoneNumers(lID, oPersonInfo.oPhoneNumbersMapByID);
	if (bResult == false)
	{
		MessageBox(_T("Something wrong happened when selecting phone numbers"));
		return;
	}
	const CMapPhoneTypesByID& oPhoneTypesMapByID = pPersonsDocument->GetPhoneTypesMapByID();
	const CMapCitiesByNameMapID& oCitiesMapByNameMapID = pPersonsDocument->GetCitiesMapByName();

	CPersonsDlg oPersonsDlg(oPersonInfo, oCitiesMapByNameMapID, oPhoneTypesMapByID, true);
	oPersonsDlg.DoModal();
}

void CPersonsView::OnReloadButton()
{
	CPersonsDocument* pPersonsDocument = GetDocument();
	if (pPersonsDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	pPersonsDocument->UpdateAllViews(NULL, OnReload, NULL);
}

void CPersonsView::Sort()
{
	CListCtrl& oListCtrl = GetListCtrl();
	SortParams recSortParams(oListCtrl, m_CurrentSortCriteria, m_CurrentSortOrder);

	oListCtrl.SortItemsEx(ListCompareFunc, (LPARAM)&recSortParams);
}

void CPersonsView::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult)
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
	else if (m_CurrentSortOrder == DescendingOrder)
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

void CPersonsView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnCtxPreview();
}

void CPersonsView::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
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

void CPersonsView::DrawPersons()
{
	CListCtrl& oListCtrl = GetListCtrl();
	CPersonsDocument* pPersonsDocument = GetDocument();
	if (pPersonsDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	oListCtrl.DeleteAllItems();
	const CMapPersonsByID& oPersonsMapByID = pPersonsDocument->GetPersonsMapByID(); // DOC
	POSITION oPersonsMapItter = oPersonsMapByID.GetStartPosition();
	int nListPos = 0;
	while (oPersonsMapItter != NULL)
	{
		long lID;
		PERSONS* pPerson;
		oPersonsMapByID.GetNextAssoc(oPersonsMapItter, lID, pPerson);
		if (pPerson == NULL)
		{
			NullPointerCaught(__LINE__, _T(__FILE__));
			continue;
		}
		oListCtrl.InsertItem(nListPos, pPerson->szFirstName);
		oListCtrl.SetItemText(nListPos, MiddleNameColumn, pPerson->szMiddleName);
		oListCtrl.SetItemText(nListPos, LastNameColumn, pPerson->szLastName);
		oListCtrl.SetItemText(nListPos, IdentificationNumberColumn, pPerson->szIdentificationNumber);

		CString strCityRegion;
		GetCityRegionFromMapWhereID(pPerson->lCityId, strCityRegion);

		oListCtrl.SetItemText(nListPos, CityColumn, strCityRegion);
		oListCtrl.SetItemText(nListPos, AddressColumn, pPerson->szAddress);
		oListCtrl.SetItemData(nListPos, static_cast<DWORD_PTR>(pPerson->lID));
		nListPos += 1;
	}
}


// Overrides
// ----------------

void CPersonsView::OnDraw(CDC* /*pDC*/)
{
	CPersonsDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

void CPersonsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CListCtrl& oListCtrl = GetListCtrl();
	CPersonsDocument* pPersonsDocument = GetDocument();
	if (pPersonsDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	PERSONS recPerson;
	// Reload everything
	if (lHint == OnReload)
	{
		pPersonsDocument->LoadPersons();
		pPersonsDocument->LoadCities();
		pPersonsDocument->LoadPhoneTypes();
		DrawPersons();
	}
	else
	{
		if (pHint == NULL)
		{
			NullPointerCaught(__LINE__, _T(__FILE__));
			return;
		}
		recPerson = *reinterpret_cast<PERSONS*>(pHint);
	}

	if (lHint == OnEdit)
	{
		for (int i = 0; i < oListCtrl.GetItemCount(); ++i)
		{
			if (oListCtrl.GetItemData(i) == recPerson.lID)
			{
				oListCtrl.SetItemText(i, FirstNameColumn, recPerson.szFirstName);
				oListCtrl.SetItemText(i, MiddleNameColumn, recPerson.szMiddleName);
				oListCtrl.SetItemText(i, LastNameColumn, recPerson.szLastName);
				oListCtrl.SetItemText(i, IdentificationNumberColumn, recPerson.szIdentificationNumber);
				
				{ // Set city name
					CString strCityName;
					GetCityRegionFromMapWhereID(recPerson.lCityId, strCityName);
					oListCtrl.SetItemText(i, CityColumn, strCityName);
				}

				oListCtrl.SetItemText(i, AddressColumn, recPerson.szAddress);
				break;
			}
		}
	}
	if (lHint == OnInsert)
	{
		int nItemPos = oListCtrl.InsertItem(oListCtrl.GetItemCount(), _T(""));


		oListCtrl.SetItemText(nItemPos, FirstNameColumn, recPerson.szFirstName);
		oListCtrl.SetItemText(nItemPos, MiddleNameColumn, recPerson.szMiddleName);
		oListCtrl.SetItemText(nItemPos, LastNameColumn, recPerson.szLastName);
		oListCtrl.SetItemText(nItemPos, IdentificationNumberColumn, recPerson.szIdentificationNumber);

		{ // Set city region, name
			CString strCityName;
			GetCityRegionFromMapWhereID(recPerson.lCityId, strCityName);
			oListCtrl.SetItemText(nItemPos, CityColumn, strCityName);
		}

		oListCtrl.SetItemText(nItemPos, AddressColumn, recPerson.szAddress);
		oListCtrl.SetItemData(nItemPos, recPerson.lID);
	}
	if (lHint == OnDelete)
	{
		for (int i = 0; i < oListCtrl.GetItemCount(); ++i)
		{
			if (oListCtrl.GetItemData(i) == recPerson.lID)
			{
				oListCtrl.DeleteItem(i);
				break;
			}
		}
	}
	Sort();
}

BOOL CPersonsView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CListView::PreCreateWindow(cs);
}


void CPersonsView::OnInitialUpdate()
{
	CListCtrl& oListCtrl = GetListCtrl();

	oListCtrl.ModifyStyle(LVS_LIST, LVS_REPORT);
	oListCtrl.SetExtendedStyle(LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	oListCtrl.ModifyStyle(NULL, LVS_SINGLESEL, 0);
	oListCtrl.InsertColumn(FirstNameColumn, _T("Име"), LVCFMT_LEFT, 100);
	oListCtrl.InsertColumn(MiddleNameColumn, _T("Презиме"), LVCFMT_LEFT, 100);
	oListCtrl.InsertColumn(LastNameColumn, _T("Фамилия"), LVCFMT_LEFT, 100);
	oListCtrl.InsertColumn(IdentificationNumberColumn, _T("ЕГН"), LVCFMT_LEFT, 100);
	oListCtrl.InsertColumn(CityColumn, _T("Град"), LVCFMT_LEFT, 100);
	oListCtrl.InsertColumn(AddressColumn, _T("Адрес"), LVCFMT_LEFT, 100);

	DrawPersons();
	Sort();
}


#ifdef _DEBUG
void CPersonsView::AssertValid() const
{
	CListView::AssertValid();
}

void CPersonsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
