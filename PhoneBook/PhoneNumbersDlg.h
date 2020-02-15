#pragma once
#include "Core/includes/Structures.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersDlg

class CPhoneNumbersDlg : public CDialog
{
	DECLARE_DYNAMIC(CPhoneNumbersDlg)
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------
private:
	enum { IDD = IDD_CPhoneNumbersDlg };


// Constructor / Destructor
// ----------------
public:
	CPhoneNumbersDlg(PHONE_NUMBERS& recPhoneNumber, const CMapPhoneTypesByID& oPhoneTypesMapByID, bool bPreview = false, CWnd* pParent = nullptr);
	virtual ~CPhoneNumbersDlg();


// Methods
// ----------------
public:
	void OnBnClickedPhoneNumbersOk();
	void OnBnClickedPhoneNumbersCancel();
	void AddPhoneAllTypesToCMB(bool bSelectCurrent);

// Overrides
// ----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

// Members
// ----------------
private:
	PHONE_NUMBERS& m_recPhoneNumber;
	const CMapPhoneTypesByID& m_oPhoneTypesMapByID;
	bool m_bPreview;

	CEdit m_edbPhoneNumber;
	CComboBox m_edbPhoneType;
};
