// PersonsDlg.cpp : implementation file
//

#include "pch.h"
#include "PhoneBook.h"
#include "afxdialogex.h"

#include "PersonsDlg.h"
#include "PhoneNumbersDlg.h"
#include "ErrorHandler.h"
/////////////////////////////////////////////////////////////////////////////
// CCitiesDlg

IMPLEMENT_DYNAMIC(CPersonsDlg, CDialog)

BEGIN_MESSAGE_MAP(CPersonsDlg, CDialog)
	ON_WM_CONTEXTMENU()

	ON_NOTIFY(HDN_ITEMCLICKA, 0, &CPersonsDlg::OnHeaderClicked)
	ON_NOTIFY(HDN_ITEMCLICKW, 0, &CPersonsDlg::OnHeaderClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PHONE_NUMBERS, &CPersonsDlg::OnItemDbClick)

	ON_COMMAND(ID_MENU_INSERT, &CPersonsDlg::OnCtxAddPhoneNumber)
	ON_COMMAND(ID_MENU_DELETE, &CPersonsDlg::OnCtxRemovePhoneNumber)
	ON_COMMAND(ID_MENU_PREVIEW, &CPersonsDlg::OnCtxPreviewPhoneNumber)
	ON_COMMAND(ID_MENU_EDIT, &CPersonsDlg::onCtxEditPhoneNumber)

	ON_BN_CLICKED(ID_PHONE_NUMBERS_OK, &CPersonsDlg::OnBnClickedPhoneNumbersOk)
	ON_BN_CLICKED(ID_PHONE_NUMBERS_CANCEL, &CPersonsDlg::OnBnClickedPhoneNumbersCancel)
	ON_CBN_SELENDOK(IDC_CITY_COMBO, &CPersonsDlg::OnCbnSelendokCityCombo)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------
CPersonsDlg::CPersonsDlg(PersonInfo& oPersonInfo, const CMapCitiesByNameMapID& oCitiesMapByNameMapID, const CMapPhoneTypesByID& oPhoneTypesMapByID, bool bPreview /*=false*/, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CPersonsDlg, pParent)
	, m_oPersonInfo(oPersonInfo)
	, m_oCitiesMapByNameMapID(oCitiesMapByNameMapID)
	, m_oPhoneTypesMapByID(oPhoneTypesMapByID)
	, m_bPreview(!bPreview)
{

}

CPersonsDlg::~CPersonsDlg()
{
}


// Methods
// ----------------
void CPersonsDlg::Sort()
{
	SortParams recSortParams(m_oPhoneNumbersListCtrl, m_CurrentSortCriteria, m_CurrentSortOrder);

	m_oPhoneNumbersListCtrl.SortItemsEx(ListCompareFunc, (LPARAM)&recSortParams);
}

void CPersonsDlg::FindPhoneTypeByID(long lID, CString& strPhoneTypeName) const
{
	PHONE_TYPES* pPhoneType;
	strPhoneTypeName = _T("Phone type not found");
	m_oPhoneTypesMapByID.Lookup(lID, pPhoneType);

	if (pPhoneType != NULL)
	{
		strPhoneTypeName = pPhoneType->szType;
	}
}

void CPersonsDlg::AddPhoneNumberToListCtrl(PHONE_NUMBERS& recPhoneNumber)
{
	CString strPhoneType;
	FindPhoneTypeByID(recPhoneNumber.lPhoneTypesId, strPhoneType);
	int nIndex = m_oPhoneNumbersListCtrl.GetItemCount();

	nIndex = m_oPhoneNumbersListCtrl.InsertItem(nIndex, recPhoneNumber.szNumber);
	m_oPhoneNumbersListCtrl.SetItemText(nIndex, PhoneTypeColumn, strPhoneType);
	m_oPhoneNumbersListCtrl.SetItemData(nIndex, static_cast<DWORD_PTR>(recPhoneNumber.lID));
}

void CPersonsDlg::RemovePhoneNumberFromListCtrl(PHONE_NUMBERS& recPhoneNumber)
{
	int nItemCount = m_oPhoneNumbersListCtrl.GetItemCount();
	for (int i = 0; i < nItemCount; ++i)
	{
		long lID = static_cast<long>(m_oPhoneNumbersListCtrl.GetItemData(i));
		if (lID == recPhoneNumber.lID)
		{
			m_oPhoneNumbersListCtrl.DeleteItem(i);
			break;
		}
	}
}

void CPersonsDlg::OnCtxAddPhoneNumber()
{
	int nSelection = m_oPhoneNumbersListCtrl.GetSelectionMark();
	PHONE_NUMBERS recPhoneNumber;
	CPhoneNumbersDlg oPhoneNumbersDlg(recPhoneNumber, m_oPhoneTypesMapByID, false);
	INT_PTR nDlgResult;
	nDlgResult = oPhoneNumbersDlg.DoModal();

	if (nDlgResult == IDCANCEL)
	{
		return;
	}

	CMapPhoneNumbersByID& oPhoneNumbersToDeleteMapID =
		m_oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_TO_DELETE);
	CMapPhoneNumbersByID& oPhoneNumbersToEditMapByID =
		m_oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_TO_EDIT);

	if (oPhoneNumbersToDeleteMapID.GetCount() > 0)
	{
		POSITION oPhoneNumbersToDeleteMapIDItter =
			oPhoneNumbersToDeleteMapID.GetStartPosition();

		long lKeyID;
		PHONE_NUMBERS* pPhoneNumber = NULL;
		while (pPhoneNumber == NULL && oPhoneNumbersToDeleteMapIDItter != NULL)
		{
			oPhoneNumbersToDeleteMapID.GetNextAssoc(oPhoneNumbersToDeleteMapIDItter, lKeyID, pPhoneNumber);
			if (pPhoneNumber == NULL)
				oPhoneNumbersToDeleteMapID.RemoveKey(lKeyID);
		}
		if (pPhoneNumber != NULL)
		{
			// Found a delete phone number that can be used for insertion
			oPhoneNumbersToDeleteMapID.RemoveKey(lKeyID);

			wcsncpy_s(pPhoneNumber->szNumber, recPhoneNumber.szNumber, Length::PERSONS_NUMBER);
			pPhoneNumber->lPhoneTypesId = recPhoneNumber.lPhoneTypesId;
			oPhoneNumbersToEditMapByID.SetAt(lKeyID, pPhoneNumber);
			AddPhoneNumberToListCtrl(*pPhoneNumber);
			return;
		}
	}

	// If we reached here, then there were no delete phone numbers
	CMapPhoneNumbersByID& oPhoneNumbersToInsertMapByID =
		m_oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_TO_INSERT);

	recPhoneNumber.lID = ++m_lLastID;
	recPhoneNumber.lPersonId = m_oPersonInfo.recPerson.lID;

	CMapPhoneNumbersByID& oPhoneNumbersMapByID =
		m_oPersonInfo.oPhoneNumbersMapByID;
	
	PHONE_NUMBERS* pPhoneNumberToInsert;
	if (oPhoneNumbersMapByID.Lookup(recPhoneNumber.lID, pPhoneNumberToInsert) == TRUE && pPhoneNumberToInsert != NULL)
	{
		// if we entered here for some reason the id was already present in the main map
		MessageBox(_T("Phone number already exists"));
		return;
	}
	if (oPhoneNumbersToInsertMapByID.Lookup(recPhoneNumber.lID, pPhoneNumberToInsert) == TRUE && pPhoneNumberToInsert != NULL)
	{
		// if we entered here for some reason the id was already present in the map for insertion
		MessageBox(_T("Phone number already inserted"));
		return;
	}

	pPhoneNumberToInsert = new PHONE_NUMBERS(recPhoneNumber);
	oPhoneNumbersMapByID.SetAt(recPhoneNumber.lID, pPhoneNumberToInsert);
	oPhoneNumbersToInsertMapByID.SetAt(recPhoneNumber.lID, pPhoneNumberToInsert);
	AddPhoneNumberToListCtrl(recPhoneNumber);
}

void CPersonsDlg::OnCtxRemovePhoneNumber()
{
	int nSelection = m_oPhoneNumbersListCtrl.GetSelectionMark();
	if (nSelection == -1)
		return;
	
	long lID = static_cast<long>(m_oPhoneNumbersListCtrl.GetItemData(nSelection));
	CMapPhoneNumbersByID& oPhoneNumbersMapByID = 
		m_oPersonInfo.oPhoneNumbersMapByID;
	CMapPhoneNumbersByID& oPhoneNumbersToInsertMapByID =
		m_oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_TO_INSERT);

	PHONE_NUMBERS* pPhoneNumber;
	if (oPhoneNumbersMapByID.Lookup(lID, pPhoneNumber) == FALSE || pPhoneNumber == NULL)
	{
		MessageBox(_T("Phone number not found"));
		return;
	}
	// Search if it was inserted
	// if it was inserted then it's not in the database
	// so no need deleting it from there
	PHONE_NUMBERS* pPhoneNumberToInsert;
	if (oPhoneNumbersToInsertMapByID.Lookup(lID, pPhoneNumberToInsert) == TRUE && pPhoneNumberToInsert != NULL)
	{
		RemovePhoneNumberFromListCtrl(*pPhoneNumberToInsert);
		delete pPhoneNumberToInsert;
		oPhoneNumbersMapByID.RemoveKey(lID);
		oPhoneNumbersToInsertMapByID.RemoveKey(lID);
		return;
	}
	CMapPhoneNumbersByID& oPhoneNumbersToEditMapByID =
		m_oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_TO_EDIT);

	// Here we check if the phone numbers was eddited
	// If it was move it to the map for deletion
	PHONE_NUMBERS* pPhoneNumberToEdit;
	if (oPhoneNumbersToEditMapByID.Lookup(lID, pPhoneNumberToEdit) == TRUE && pPhoneNumber != NULL)
	{
		oPhoneNumbersToEditMapByID.RemoveKey(lID);
	}

	CMapPhoneNumbersByID& oPhoneNumbersToDeleteMapByID =
		m_oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_TO_DELETE);
	// Here we check if the phone number is present in
	// the map for deletion, if yes then something wrong happened
	PHONE_NUMBERS* pPhoneNumberToDelete;
	if (oPhoneNumbersToDeleteMapByID.Lookup(lID, pPhoneNumberToDelete) == TRUE && pPhoneNumber != NULL)
	{
		delete pPhoneNumberToDelete;
		oPhoneNumbersToDeleteMapByID.RemoveKey(lID);
	}
	oPhoneNumbersToDeleteMapByID.SetAt(lID, pPhoneNumber);
	RemovePhoneNumberFromListCtrl(*pPhoneNumber);
}

void CPersonsDlg::OnCtxPreviewPhoneNumber()
{
	int nSelection = m_oPhoneNumbersListCtrl.GetSelectionMark();
	if (nSelection == -1)
	{
		return;
	}
	long lID = static_cast<long>(m_oPhoneNumbersListCtrl.GetItemData(nSelection));
	PHONE_NUMBERS* pPhoneNumber;
	CMapPhoneNumbersByID& oPhoneNumbersMapByID = m_oPersonInfo.oPhoneNumbersMapByID;

	oPhoneNumbersMapByID.Lookup(lID, pPhoneNumber);
	if (pPhoneNumber == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		MessageBox(_T("Phone Number was not found"));
		return;
	}
	CPhoneNumbersDlg oPhoneNumbersDlg(*pPhoneNumber, m_oPhoneTypesMapByID, true);
	oPhoneNumbersDlg.DoModal();
}

void CPersonsDlg::onCtxEditPhoneNumber()
{
	int nSelection = m_oPhoneNumbersListCtrl.GetSelectionMark();
	if (nSelection == -1)
	{
		return;
	}

	PHONE_NUMBERS* pPhoneNumber;
	long lID = static_cast<long>(m_oPhoneNumbersListCtrl.GetItemData(nSelection));
	CMapPhoneNumbersByID& oPhoneNumbersMapByID = m_oPersonInfo.oPhoneNumbersMapByID;

	if (oPhoneNumbersMapByID.Lookup(lID, pPhoneNumber) == false)
	{
		MessageBox(_T("Phone number already deleted"));
		return;
	}
	if (pPhoneNumber == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		MessageBox(_T("Phone Number was not found"));
		return;
	}
	PHONE_NUMBERS recPhoneNumberToEdit = *pPhoneNumber;
	CPhoneNumbersDlg oPhoneNumbersDlg(recPhoneNumberToEdit, m_oPhoneTypesMapByID, false);
	INT_PTR nDlgResult;
	nDlgResult = oPhoneNumbersDlg.DoModal();

	if (nDlgResult == IDCANCEL)
	{
		return;
	}

	// if the information is the same
	if (wcscmp(recPhoneNumberToEdit.szNumber, pPhoneNumber->szNumber) == false &&
		recPhoneNumberToEdit.lPhoneTypesId == pPhoneNumber->lPhoneTypesId)
	{
		return;
	}
	else
	{
		// if we reached here the phone number/type
		// was changed and it is valid, so we copy it
		// in the main array
		*pPhoneNumber = recPhoneNumberToEdit;
	}

	CMapPhoneNumbersByID& oPhoneNumbersToEditMapByID = 
		m_oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_TO_EDIT);

	// Then insert pointer to the phone number inside the main map
	// into hint array
	PHONE_NUMBERS* pPhoneNumberToEdit;
	if (oPhoneNumbersToEditMapByID.Lookup(lID, pPhoneNumberToEdit) == TRUE && pPhoneNumberToEdit != NULL)
	{
		*pPhoneNumberToEdit = recPhoneNumberToEdit;
	}
	else
	{
		oPhoneNumbersToEditMapByID.SetAt(lID, pPhoneNumber);
	}

	CString strPhoneType;
	FindPhoneTypeByID(pPhoneNumber->lPhoneTypesId, strPhoneType);

	m_oPhoneNumbersListCtrl.SetItemText(nSelection, PhoneNumberColumn, pPhoneNumber->szNumber);
	m_oPhoneNumbersListCtrl.SetItemText(nSelection, PhoneTypeColumn, strPhoneType);
}

void CPersonsDlg::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
{
	if (m_bPreview == false) // == false because inverted at construction
	{
		return;
	}
	CHeaderCtrl* pHeader = m_oPhoneNumbersListCtrl.GetHeaderCtrl();
	if (pHeader == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	CRect rect;
	CPoint hitPoint = ptMousePos;

	m_oPhoneNumbersListCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	ScreenToClient(&hitPoint);
	if (PtInRect(&rect, hitPoint) == FALSE)
	{
		return;
	}
	
	pHeader->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (PtInRect(&rect, hitPoint) == TRUE)
	{
		return;
	}

	CMenu* pPopUp = nullptr;
	pPopUp = m_oCtxMenu.GetSubMenu(0);

	if (m_oPhoneNumbersListCtrl.GetSelectedCount() >= 1)
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

void CPersonsDlg::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
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

void CPersonsDlg::OnItemDbClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_bPreview == false)
		return;

	OnCtxPreviewPhoneNumber();
}

void CPersonsDlg::OnBnClickedPhoneNumbersOk()
{
	auto EmptyCheck =
		[](CString& strStringToCheck, CString& strErrorMessage)
	{
		bool bNoErrors = false;
		strStringToCheck = strStringToCheck.Trim(_T(" "));
		if (strStringToCheck.IsEmpty() == true)
		{
			CString strEmptyError = _T("\tCannot Be Empty\n");
			strErrorMessage.Append(strEmptyError);
			bNoErrors = true;
		}
		return bNoErrors;
	};

	// Returns true if there are no errors
	auto NoNumbersCheck =
		[&EmptyCheck](CString& strStringToCheck, CString& strErrorMessage)
	{
		bool bNoErrors = true;

		bNoErrors = !EmptyCheck(strStringToCheck, strErrorMessage);

		for (int i = 0; i < strStringToCheck.GetLength(); ++i)
		{
			wchar_t cCharacter = strStringToCheck.GetAt(i);
			bool bLatinCondition =
				(cCharacter >= _T('0') && cCharacter <= _T('9'));
			if (bLatinCondition)
			{
				CString strInvalidStringError = _T("\tCan contain only letters\n");
				strErrorMessage.Append(strInvalidStringError);
				bNoErrors = false;
				break;
			}
		}
		return bNoErrors;
	};

	auto OnlyNumbersCheck =
		[&EmptyCheck](CString& strStringToCheck, CString& strErrorMessage)
	{
		bool bNoErrors = true;
		
		bNoErrors = !EmptyCheck(strStringToCheck, strErrorMessage);

		for (int i = 0; i < strStringToCheck.GetLength(); ++i)
		{
			wchar_t cCharacter = strStringToCheck.GetAt(i);
			bool bCondition =
				(cCharacter >= _T('0') && cCharacter <= _T('9'));

			if (!bCondition)
			{
				CString strInvalidStringError = _T("\tCan contain only numbers\n");
				strErrorMessage.Append(strInvalidStringError);
				bNoErrors = false;
				break;
			}
		}
		return bNoErrors;
	};

	CString strErrorMessage;

	CString strFirstNameErrors = _T("First Name:\n");
	CString strMiddleNameErrors = _T("\nMiddle Name:\n");
	CString strLastNameErrors = _T("\nLast Name:\n");
	CString strIdentificationNumberErrors = _T("\nIdentification Number:\n");
	CString strCityErrors = _T("\nCity:\n");
	CString strRegionErrors = _T("\nRegion:\n");
	CString strAddressErrors = _T("\nAddress:\n");

	CString strFirstName;
	m_edbFirstName.GetWindowTextW(strFirstName);
	if (NoNumbersCheck(strFirstName, strFirstNameErrors) == false)
	{
		strErrorMessage.Append(strFirstNameErrors);
	}

	CString strMiddleName;
	m_edbMiddleName.GetWindowTextW(strMiddleName);
	if (NoNumbersCheck(strMiddleName, strMiddleNameErrors) == false)
	{
		strErrorMessage.Append(strMiddleNameErrors);
	}

	CString strLastName;
	m_edbLastName.GetWindowTextW(strLastName);
	if (NoNumbersCheck(strLastName, strLastNameErrors) == false)
	{
		strErrorMessage.Append(strLastNameErrors);
	}

	CString strIdentificationNumber;
	m_edbIdentificationNumber.GetWindowTextW(strIdentificationNumber);
	if (OnlyNumbersCheck(strIdentificationNumber, strIdentificationNumberErrors) == false)
	{
		strErrorMessage.Append(strIdentificationNumberErrors);
	}

	CString strCity;
	int nCitySelectedIndex = m_edbCityCombo.GetCurSel();
	m_edbCityCombo.GetLBText(nCitySelectedIndex, strCity);
	if (EmptyCheck(strCity, strCityErrors) == true)
	{
		strErrorMessage.Append(strCityErrors);
	}

	CString strRegion;
	int nRegionSelectedIndex = m_edbRegionCombo.GetCurSel();
	m_edbRegionCombo.GetLBText(nRegionSelectedIndex, strRegion);
	if (EmptyCheck(strRegion, strRegionErrors) == true)
	{
		strErrorMessage.Append(strRegionErrors);
	}

	CString strAddress;
	m_edbAddress.GetWindowTextW(strAddress);
	if (EmptyCheck(strAddress, strAddressErrors) == true)
	{
		strErrorMessage.Append(strAddressErrors);
	}
	
	if (strErrorMessage.IsEmpty() == false)
	{
		MessageBox(strErrorMessage);
		return;
	}
	// If we reached here then all
	// fields for PERSON are correct
	// Copy them to m_oPersonInfo.recPerson
	
	PERSONS& recPerson = m_oPersonInfo.recPerson;

	recPerson.lCityId = static_cast<long>(m_edbRegionCombo.GetItemData(nRegionSelectedIndex));
	wcsncpy_s(recPerson.szFirstName, strFirstName, Length::PERSONS_FIRST_NAME);
	wcsncpy_s(recPerson.szMiddleName, strMiddleName, Length::PERSONS_MIDDLE_NAME);
	wcsncpy_s(recPerson.szLastName, strLastName, Length::PERSONS_LAST_NAME);
	wcsncpy_s(recPerson.szIdentificationNumber, strIdentificationNumber, Length::PERSONS_IDENTIFICATION_NUMBER);
	wcsncpy_s(recPerson.szAddress, strAddress, Length::PERSONS_ADDRESS);

	CDialog::OnOK();
}

void CPersonsDlg::OnBnClickedPhoneNumbersCancel()
{
	CDialog::OnCancel();
}

void CPersonsDlg::OnCbnSelendokCityCombo()
{
	m_edbRegionCombo.SetRedraw(FALSE);
	ClearRegionComboBox();

	CString strSelectedCity;
	int nCurrentSelIndex = m_edbCityCombo.GetCurSel();

	m_edbCityCombo.GetLBText(nCurrentSelIndex, strSelectedCity);
	if (strSelectedCity.IsEmpty())
	{
		ClearCitiesComboBox();
		AddAllRegions(false);
		m_edbCityCombo.SetCurSel(0);
		m_edbRegionCombo.SetCurSel(0);
	}
	else
	{
		CMapCitiesByID* oMapRegionsByCityName;
		m_edbRegionCombo.InsertString(0, _T(""));
		if (m_oCitiesMapByNameMapID.Lookup(strSelectedCity, oMapRegionsByCityName))
		{
			AddRegionsByCityName(oMapRegionsByCityName);
		}
		m_edbRegionCombo.SetCurSel(0);
	}

	m_edbRegionCombo.SetRedraw(TRUE);
	m_edbRegionCombo.Invalidate();
}

void CPersonsDlg::AddRegionsByCityName(const CMapCitiesByID* oMapByCityName)
{
	if (oMapByCityName == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return;
	}

	POSITION oSubMapItter = oMapByCityName->GetStartPosition();
	while (oSubMapItter != NULL)
	{
		long lSubKeyID;
		CITIES* pCity;

		oMapByCityName->GetNextAssoc(oSubMapItter, lSubKeyID, pCity);
		int nIndex = m_edbRegionCombo.AddString(pCity->szRegion);
		m_edbRegionCombo.SetItemData(nIndex, static_cast<DWORD_PTR>(pCity->lID));
	}
}
void CPersonsDlg::AddAllRegions(bool bSelectCurrent)
{
	bool bSelectedRegionFound = false;
	PERSONS recPerson = m_oPersonInfo.recPerson;
	CITIES* pCityToInitialFill;

	// Disable redraw
	m_edbCityCombo.SetRedraw(false);
	m_edbRegionCombo.SetRedraw(false);

	// Fill the combos
	POSITION oMainMapItter = m_oCitiesMapByNameMapID.GetStartPosition();
	while (oMainMapItter != NULL)
	{
		CString strMainKeyCityName;
		CMapCitiesByID* pMapCitiesByID;

		m_oCitiesMapByNameMapID.GetNextAssoc(oMainMapItter, strMainKeyCityName, pMapCitiesByID);

		m_edbCityCombo.AddString(strMainKeyCityName);

		POSITION oSubMapItter = pMapCitiesByID->GetStartPosition();
		AddRegionsByCityName(pMapCitiesByID);

		if ( bSelectCurrent &&  pMapCitiesByID->Lookup(recPerson.lCityId, pCityToInitialFill) )
		{
			bSelectedRegionFound = true;
			int nStartIndexToSeachCity = m_edbCityCombo.GetCount() - static_cast<int>(pMapCitiesByID->GetCount()) - 1;
			int nStartIndexToSeachRegion = m_edbRegionCombo.GetCount() - static_cast<int>(pMapCitiesByID->GetCount()) - 1;
			m_edbCityCombo.SelectString(nStartIndexToSeachCity - 1, pCityToInitialFill->szName);
			m_edbRegionCombo.SelectString(nStartIndexToSeachCity - 1, pCityToInitialFill->szRegion);
		}
	}
	m_edbCityCombo.AddString(_T(""));
	m_edbRegionCombo.AddString(_T(""));

	if (bSelectedRegionFound == false)
	{
		m_edbCityCombo.SetCurSel(0);
		m_edbRegionCombo.SetCurSel(0);
	}

	// Enable redraw && invalidate
	m_edbCityCombo.SetRedraw(true);
	m_edbCityCombo.Invalidate();

	m_edbRegionCombo.SetRedraw(true);
	m_edbRegionCombo.Invalidate();
}
void CPersonsDlg::ClearRegionComboBox()
{
	m_edbRegionCombo.ResetContent();
}
void CPersonsDlg::ClearCitiesComboBox()
{
	m_edbCityCombo.ResetContent();
}

// Overrides
// ----------------
void CPersonsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PHONE_NUMBERS, m_oPhoneNumbersListCtrl);
	DDX_Control(pDX, IDC_EDIT_FIRST_NAME, m_edbFirstName);
	DDX_Control(pDX, IDC_EDIT_MIDDLE_NAME, m_edbMiddleName);
	DDX_Control(pDX, IDC_EDIT_LAST_NAME, m_edbLastName);
	DDX_Control(pDX, IDC_EDIT_IDENTIFICATION_NUMBER, m_edbIdentificationNumber);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_edbAddress);
	DDX_Control(pDX, IDC_CITY_COMBO, m_edbCityCombo);
	DDX_Control(pDX, IDC_REGION_COMBO, m_edbRegionCombo);
}

BOOL CPersonsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Style
	m_oPhoneNumbersListCtrl.ModifyStyle(LVS_LIST, LVS_REPORT);
	m_oPhoneNumbersListCtrl.SetExtendedStyle(LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_oPhoneNumbersListCtrl.ModifyStyle(NULL, LVS_SINGLESEL, 0);
	m_oPhoneNumbersListCtrl.SetBkColor(0);
	m_oPhoneNumbersListCtrl.SetTextBkColor(0);
	m_oPhoneNumbersListCtrl.SetTextColor(0x00ffffff);

	m_oPhoneNumbersListCtrl.InsertColumn(0, _T("Телефонен Номер"), LVCFMT_LEFT, 150);
	m_oPhoneNumbersListCtrl.InsertColumn(1, _T("Тип Телефон"), LVCFMT_LEFT, 100);

	// Menu
	m_oCtxMenu.CreateMenu();
	CMenu oSubMenu;
	oSubMenu.LoadMenu(IDR_MENU1);
	m_oCtxMenu.AppendMenu(MF_POPUP, (UINT_PTR)oSubMenu.m_hMenu, _T(""));
	oSubMenu.Detach();

	// Controls
	m_edbFirstName.SetLimitText(Length::PERSONS_FIRST_NAME - 1);
	m_edbMiddleName.SetLimitText(Length::PERSONS_MIDDLE_NAME - 1);
	m_edbLastName.SetLimitText(Length::PERSONS_LAST_NAME - 1);
	m_edbIdentificationNumber.SetLimitText(Length::PERSONS_IDENTIFICATION_NUMBER - 1);
	m_edbAddress.SetLimitText(Length::PERSONS_ADDRESS - 1);

	m_edbFirstName.EnableWindow(m_bPreview);
	m_edbMiddleName.EnableWindow(m_bPreview);
	m_edbLastName.EnableWindow(m_bPreview);
	m_edbIdentificationNumber.EnableWindow(m_bPreview);
	m_edbAddress.EnableWindow(m_bPreview);
	m_edbCityCombo.EnableWindow(m_bPreview);
	m_edbRegionCombo.EnableWindow(m_bPreview);

	// Fill the fields
	PERSONS& recPerson = m_oPersonInfo.recPerson;
	CMapPhoneNumbersByID& oPhoneNumbersMapByID = m_oPersonInfo.oPhoneNumbersMapByID;

	m_edbFirstName.SetWindowTextW(recPerson.szFirstName);
	m_edbMiddleName.SetWindowTextW(recPerson.szMiddleName);
	m_edbLastName.SetWindowTextW(recPerson.szLastName);
	m_edbIdentificationNumber.SetWindowTextW(recPerson.szIdentificationNumber);
	m_edbAddress.SetWindowTextW(recPerson.szAddress);


	// Fill Phone Numbers
	POSITION oPhoneNumbersMapItter = oPhoneNumbersMapByID.GetStartPosition();
	while (oPhoneNumbersMapItter != NULL)
	{
		long lKeyID;
		PHONE_NUMBERS* pPhoneNumber;
		oPhoneNumbersMapByID.GetNextAssoc(oPhoneNumbersMapItter, lKeyID, pPhoneNumber);
		if (pPhoneNumber == NULL)
		{
			oPhoneNumbersMapByID.RemoveKey(lKeyID);
			continue;
		}
		
		if (lKeyID > m_lLastID)
		{
			m_lLastID = lKeyID;
		}

		AddPhoneNumberToListCtrl(*pPhoneNumber);
	}
	
	// Fill the combos
	if(m_bPreview)
		AddAllRegions(true);
	else
	{
		CString strCityName;
		POSITION oCitiesMapByNameMapIDItter = m_oCitiesMapByNameMapID.GetStartPosition();
		while (oCitiesMapByNameMapIDItter != NULL)
		{
			CString strCityName;
			CMapCitiesByID* pMapCitiesByID;
			m_oCitiesMapByNameMapID.GetNextAssoc(oCitiesMapByNameMapIDItter, strCityName, pMapCitiesByID);
			if (pMapCitiesByID == NULL)
			{
				NullPointerCaught(__LINE__, _T(__FILE__));
				continue;
			}
			CITIES* pCity;
			if (pMapCitiesByID->Lookup(recPerson.lCityId, pCity) == TRUE && pCity != NULL)
			{
				m_edbCityCombo.InsertString(0, pCity->szName);
				m_edbCityCombo.SetCurSel(0);
				m_edbRegionCombo.InsertString(0, pCity->szRegion);
				m_edbRegionCombo.SetCurSel(0);
			}
		}
	}
	Sort();
	return TRUE;
}
