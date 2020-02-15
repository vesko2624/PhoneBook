
#include "pch.h"
#include "PhoneTypesView.h"
#include "PhoneTypesDlg.h"
#include "ErrorHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesView

IMPLEMENT_DYNCREATE(CPhoneTypesView, CListView)

BEGIN_MESSAGE_MAP(CPhoneTypesView, CListView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_RELOAD_BUTTON, &CPhoneTypesView::OnReloadButton)

	ON_NOTIFY(HDN_ITEMCLICKA, 0, &CPhoneTypesView::OnHeaderClicked)
	ON_NOTIFY(HDN_ITEMCLICKW, 0, &CPhoneTypesView::OnHeaderClicked)

	ON_COMMAND(ID_MENU_INSERT, &CPhoneTypesView::OnCtxAddPhoneType)
	ON_COMMAND(ID_MENU_DELETE, &CPhoneTypesView::OnCtxRemovePhoneType)
	ON_COMMAND(ID_MENU_PREVIEW, &CPhoneTypesView::OnCtxPreview)
	ON_COMMAND(ID_MENU_EDIT, &CPhoneTypesView::onCtxEdit)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------
CPhoneTypesView::CPhoneTypesView() noexcept
{
	m_oCtxMenu.CreateMenu();
	CMenu oSubMenu;
	oSubMenu.LoadMenu(IDR_MENU1);
	m_oCtxMenu.AppendMenu(MF_POPUP, (UINT_PTR)oSubMenu.m_hMenu, _T(""));
	oSubMenu.Detach();
}

CPhoneTypesView::~CPhoneTypesView()
{

}


// Methods
// ----------------

#ifdef _DEBUG
CPhoneTypesDocument* CPhoneTypesView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhoneTypesDocument)));
	return (CPhoneTypesDocument*)m_pDocument;
}
#endif

void CPhoneTypesView::OnCtxAddPhoneType()
{
	PHONE_TYPES recSelectedPhoneType;
	CPhoneTypesDlg oPhoneTypesDlg(recSelectedPhoneType, true);
	if (oPhoneTypesDlg.DoModal() == IDCANCEL)
	{
		return;
	}

	CPhoneTypesDocument* pPhoneTypesDocument = GetDocument();
	if (pPhoneTypesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	if (pPhoneTypesDocument->InsertPhoneType(recSelectedPhoneType) == false)
	{
		MessageBox(_T("Phone type with this name already exists."));
		return;
	}
}

void CPhoneTypesView::OnCtxRemovePhoneType()
{
	CPhoneTypesDocument* pPhoneTypesDocument = GetDocument();
	if (pPhoneTypesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	CListCtrl& oListCtrl = GetListCtrl();
	int nCurrentRow = oListCtrl.GetSelectionMark();
	long lID = static_cast<long>(oListCtrl.GetItemData(nCurrentRow));

	if (pPhoneTypesDocument->DeleteWhereID(lID) == false)
	{
		MessageBox(_T("This phone type is either not existing and you need to reload, or it cannot be deleted because something depends on it"));
		return;
	}
}

void CPhoneTypesView::onCtxEdit()
{
	CListCtrl& oListCtrl = GetListCtrl();
	int nCurrentRow = oListCtrl.GetSelectionMark();

	if (nCurrentRow == -1)
		return;

	CPhoneTypesDocument* pPhoneTypesDocument = GetDocument();
	if (pPhoneTypesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	long lID = static_cast<long>(oListCtrl.GetItemData(nCurrentRow));
	PHONE_TYPES recSelectedPhoneType;

	if (pPhoneTypesDocument->SelectWhereIDFromArray(lID, recSelectedPhoneType) == false)
	{
		MessageBox(_T("This phone type doesn't exists anymore. You need to reload"));
		return;
	}
	CPhoneTypesDlg oPhoneTypesDlg(recSelectedPhoneType, true);

	if (oPhoneTypesDlg.DoModal() == IDCANCEL)
	{
		return;
	}

	if (pPhoneTypesDocument->UpdateWhereID(recSelectedPhoneType.lID, recSelectedPhoneType) == false)
	{
		MessageBox(_T("Phone Type with this name already exists or the record was already updated and you need to reload."));
		return;
	}
}

void CPhoneTypesView::OnCtxPreview()
{
	CPhoneTypesDocument* pPhoneTypeDocument = GetDocument();
	if (pPhoneTypeDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	CListCtrl& oListCtrl = GetListCtrl();
	int nCurrentRow = oListCtrl.GetSelectionMark();
	if (nCurrentRow == -1)
		return;

	long lID = static_cast<long>(oListCtrl.GetItemData(nCurrentRow));
	PHONE_TYPES recSelectedPhoneType;

	if (pPhoneTypeDocument->SelectWhereIDFromArray(lID, recSelectedPhoneType) == false)
	{
		MessageBox(_T("This phone type doesn't exists anymore."));
		return;
	}

	CPhoneTypesDlg oPhoneTypesDlg(recSelectedPhoneType, false);
	oPhoneTypesDlg.DoModal();
}

void CPhoneTypesView::OnReloadButton()
{
	CPhoneTypesDocument* pPhoneTypesDocument = GetDocument();
	if (pPhoneTypesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	pPhoneTypesDocument->UpdateAllViews(NULL, OnReload, NULL);
}

void CPhoneTypesView::Sort()
{
	CListCtrl& oListCtrl = GetListCtrl();
	SortParams recSortParams(oListCtrl, m_CurrentSortCriteria, m_CurrentSortOrder);

	oListCtrl.SortItemsEx(ListCompareFunc, (LPARAM)&recSortParams);
}

void CPhoneTypesView::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult)
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

void CPhoneTypesView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnCtxPreview();
}

void CPhoneTypesView::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
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

void CPhoneTypesView::DrawPhoneTypes()
{
	CListCtrl& oListCtrl = GetListCtrl();
	CPhoneTypesDocument* pPhoneTypeDocument = GetDocument();
	if (pPhoneTypeDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	oListCtrl.DeleteAllItems();
	const CPhoneTypesArray& oPhoneTypesPtrArray = pPhoneTypeDocument->GetPhoneTypesArray(); // DOC
	int nArraySize = static_cast<int>(oPhoneTypesPtrArray.GetCount());
	oListCtrl.SetItemCount(nArraySize);

	for (INT_PTR i = 0; i < nArraySize; ++i)
	{
		PHONE_TYPES* pPhoneType = oPhoneTypesPtrArray.GetAt(i);
		oListCtrl.InsertItem(i, pPhoneType->szType);
		oListCtrl.SetItemData(i, static_cast<DWORD_PTR>(pPhoneType->lID));
	}
}


// Overrides
// ----------------

void CPhoneTypesView::OnDraw(CDC* /*pDC*/)
{
	CPhoneTypesDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

void CPhoneTypesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CListCtrl& oListCtrl = GetListCtrl();
	CPhoneTypesDocument* pPhoneTypesDocument = GetDocument();
	if (pPhoneTypesDocument == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}
	PHONE_TYPES recPhoneType;
	// Reload everything
	if (lHint == OnReload)
	{
		pPhoneTypesDocument->LoadPhoneTypes();
		DrawPhoneTypes();
	}
	else
	{
		if (pHint == NULL)
		{
			NullPointerCaught(__LINE__, _T(__FILE__));
			return;
		}
		recPhoneType = *reinterpret_cast<PHONE_TYPES*>(pHint);
	}

	if (lHint == OnEdit)
	{
		for (int i = 0; i < oListCtrl.GetItemCount(); ++i)
		{
			if (oListCtrl.GetItemData(i) == recPhoneType.lID)
			{
				oListCtrl.SetItemText(i, PhoneTypeColumn, recPhoneType.szType);
				break;
			}
		}
	}
	if (lHint == OnInsert)
	{
		int nItemPos = oListCtrl.InsertItem(oListCtrl.GetItemCount(), _T(""));
		oListCtrl.SetItemText(nItemPos, PhoneTypeColumn, recPhoneType.szType);
		oListCtrl.SetItemData(nItemPos, recPhoneType.lID);
	}
	if (lHint == OnDelete)
	{
		for (int i = 0; i < oListCtrl.GetItemCount(); ++i)
		{
			if (oListCtrl.GetItemData(i) == recPhoneType.lID)
			{
				oListCtrl.DeleteItem(i);
				break;
			}
		}
		return;
	}
	Sort();
}

BOOL CPhoneTypesView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CListView::PreCreateWindow(cs);
}


void CPhoneTypesView::OnInitialUpdate()
{
	CListCtrl& oListCtrl = GetListCtrl();

	oListCtrl.ModifyStyle(LVS_LIST, LVS_REPORT);
	oListCtrl.SetExtendedStyle(LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	oListCtrl.ModifyStyle(NULL, LVS_SINGLESEL, 0);
	oListCtrl.InsertColumn(PhoneTypeColumn, _T("Тип телефон"), LVCFMT_LEFT, 100);

	DrawPhoneTypes();
	Sort();
}


#ifdef _DEBUG
void CPhoneTypesView::AssertValid() const
{
	CListView::AssertValid();
}

void CPhoneTypesView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif