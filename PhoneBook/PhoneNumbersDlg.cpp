// PhoneNumbersDlg.cpp : implementation file
//

#include "pch.h"
#include "PhoneBook.h"
#include "PhoneNumbersDlg.h"
#include "afxdialogex.h"
#include "ErrorHandler.h"
/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersDlg

IMPLEMENT_DYNAMIC(CPhoneNumbersDlg, CDialog)
BEGIN_MESSAGE_MAP(CPhoneNumbersDlg, CDialog)
	ON_BN_CLICKED(ID_PHONE_NUMBERS_OK, &CPhoneNumbersDlg::OnBnClickedPhoneNumbersOk)
	ON_BN_CLICKED(ID_PHONE_NUMBERS_CANCEL, &CPhoneNumbersDlg::OnBnClickedPhoneNumbersCancel)
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------
CPhoneNumbersDlg::CPhoneNumbersDlg(PHONE_NUMBERS& recPhoneNumber, const CMapPhoneTypesByID& oPhoneTypesMapByID, bool bPreview /*=false*/, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CPhoneNumbersDlg, pParent)
	, m_recPhoneNumber(recPhoneNumber)
	, m_oPhoneTypesMapByID(oPhoneTypesMapByID)
	, m_bPreview(!bPreview)
{

}

CPhoneNumbersDlg::~CPhoneNumbersDlg()
{
}


// Methods
// ----------------
void CPhoneNumbersDlg::OnBnClickedPhoneNumbersOk()
{
	if (m_bPreview == false) // Because inverted at construction
	{
		CDialog::OnOK();
	}

	bool bValidNumber = true;
	CString strPhoneNumber;

	m_edbPhoneNumber.GetWindowTextW(strPhoneNumber);

	strPhoneNumber = strPhoneNumber.Trim(_T(" "));
	if (strPhoneNumber.IsEmpty() == true)
	{
		MessageBox(_T("Phone number cannot be empty"));
		return;
	}

	for (int i = 0; i < strPhoneNumber.GetLength(); ++i)
	{
		wchar_t cCharacter = strPhoneNumber.GetAt(i);
		bool bCondition = cCharacter >= _T('0') && cCharacter <= _T('9') || cCharacter == _T('-');
		if (!bCondition)
		{
			bValidNumber = false;
		}
	}
	
	if (bValidNumber == false)
	{
		MessageBox(_T("Phone number can contain only numbers"));
		return;
	}
	
	CString strPhoneType;
	int nSelectedPhoneType = m_edbPhoneType.GetCurSel();
	long lPhoneTypeID = static_cast<long>(m_edbPhoneType.GetItemData(nSelectedPhoneType));

	m_edbPhoneType.GetLBText(nSelectedPhoneType, strPhoneType);
	strPhoneType = strPhoneType.Trim(_T(" "));

	if (strPhoneType.IsEmpty() == true)
	{
		MessageBox(_T("Phone type is required"));
		return;
	}

	m_edbPhoneNumber.GetWindowTextW(m_recPhoneNumber.szNumber, Length::PERSONS_NUMBER);

	m_recPhoneNumber.lPhoneTypesId = lPhoneTypeID;
	CDialog::OnOK();
}


void CPhoneNumbersDlg::OnBnClickedPhoneNumbersCancel()
{
	CDialog::OnCancel();
}

void CPhoneNumbersDlg::AddPhoneAllTypesToCMB(bool bSelectCurrent)
{
	m_edbPhoneType.SetRedraw(FALSE);
	m_edbPhoneType.ResetContent();

	POSITION oPhoneTypesMapItter = m_oPhoneTypesMapByID.GetStartPosition();
	while (oPhoneTypesMapItter != NULL)
	{
		long lKeyID;
		PHONE_TYPES* pPhoneType;
		m_oPhoneTypesMapByID.GetNextAssoc(oPhoneTypesMapItter, lKeyID, pPhoneType);
		if (pPhoneType == NULL)
		{
			NullPointerCaught(__LINE__, _T(__FILE__));
			continue;
		}
		int nItemCount = m_edbPhoneType.GetCount();
		m_edbPhoneType.InsertString(nItemCount, pPhoneType->szType);
		m_edbPhoneType.SetItemData(nItemCount, static_cast<DWORD_PTR>(pPhoneType->lID));
		if(bSelectCurrent && m_recPhoneNumber.lPhoneTypesId == pPhoneType->lID)
		{
			m_edbPhoneType.SelectString(nItemCount - 1, pPhoneType->szType);
		}
	}

	m_edbPhoneType.SetRedraw(TRUE);
	m_edbPhoneType.Invalidate();
}

// Overrides
// ----------------
void CPhoneNumbersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PHONENUMBER, m_edbPhoneNumber);
	DDX_Control(pDX, IDC_PHONE_NUMBERS_TYPE_COMBO, m_edbPhoneType);
}

BOOL CPhoneNumbersDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Set Limits
	m_edbPhoneNumber.SetLimitText(Length::PERSONS_NUMBER - 1);

	m_edbPhoneNumber.EnableWindow(m_bPreview);
	m_edbPhoneType.EnableWindow(m_bPreview);

	if (m_bPreview == false)
	{
		PHONE_TYPES* pPhoneType;
		if (m_oPhoneTypesMapByID.Lookup(m_recPhoneNumber.lPhoneTypesId, pPhoneType) == false || pPhoneType == NULL)
		{
			return FALSE;
		}
		m_edbPhoneType.InsertString(0, pPhoneType->szType);
		m_edbPhoneType.SelectString(0, pPhoneType->szType);
	}
	else
	{
		m_edbPhoneType.InsertString(0, _T(" "));
		AddPhoneAllTypesToCMB(true);
	}
	m_edbPhoneNumber.SetWindowTextW(m_recPhoneNumber.szNumber);
	return TRUE;
}
