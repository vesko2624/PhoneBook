#pragma once
#include "PersonsData.h"
#include "CListSort.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsDlg
class CPersonsDlg : public CDialog
{
	DECLARE_DYNAMIC(CPersonsDlg)
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------
private:
	enum { IDD = IDD_CPersonsDlg };
	enum ViewColumn
	{
		PhoneNumberColumn,
		PhoneTypeColumn,
		IdColumn = -1
	};

// Constructor / Destructor
// ----------------
public:
	CPersonsDlg(PersonInfo& oPersonInfo, const CMapCitiesByNameMapID& oCitiesMapByNameMapID, const CMapPhoneTypesByID& oPhoneTypesMap, bool bPreview = false, CWnd* pParent = nullptr);
	virtual ~CPersonsDlg();


// Methods
// ----------------
public:
	void Sort();
	void FindPhoneTypeByID(long lID, CString& strPhoneTypeName) const;

	void AddPhoneNumberToListCtrl(PHONE_NUMBERS& recPhoneNumber);
	void RemovePhoneNumberFromListCtrl(PHONE_NUMBERS& recPhoneNumber);

	void OnCtxAddPhoneNumber();
	void OnCtxRemovePhoneNumber();
	void OnCtxPreviewPhoneNumber();
	void onCtxEditPhoneNumber();

	void OnContextMenu(CWnd* pWnd, CPoint ptMousePos);
	void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);
	void OnItemDbClick(NMHDR* pNMHDR, LRESULT* pResult);

	void OnBnClickedPhoneNumbersOk();
	void OnBnClickedPhoneNumbersCancel();
	void OnCbnSelendokCityCombo();

	void AddRegionsByCityName(const CMapCitiesByID* oMapByCityName);
	void AddAllRegions(bool bSelectCurrent);
	void ClearRegionComboBox();
	void ClearCitiesComboBox();

// Overrides
// ----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();


// Members
// ----------------
private:
	// Open Mode
	bool m_bPreview;

	// Maps
	const CMapPhoneTypesByID& m_oPhoneTypesMapByID;
	const CMapCitiesByNameMapID& m_oCitiesMapByNameMapID;

	// PersonInfo
	PersonInfo& m_oPersonInfo;
	long m_lLastID = -1;
	// Menu
	CMenu m_oCtxMenu;

	// Controls
	CListCtrl m_oPhoneNumbersListCtrl;
	CEdit m_edbFirstName;
	CEdit m_edbMiddleName;
	CEdit m_edbLastName;
	CEdit m_edbIdentificationNumber;
	CEdit m_edbAddress;

	// Sort memorizing variables
	ViewColumn m_CurrentSortCriteria = IdColumn;
	SortOrder m_CurrentSortOrder = NoneOrder;

	CComboBox m_edbCityCombo;
	CComboBox m_edbRegionCombo;
};
