#include "pch.h"
#include "PhoneBook.h"
#include "PhoneTypesDlg.h"
#include <regex>


/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesDlg

IMPLEMENT_DYNAMIC(CPhoneTypesDlg, CDialog)

BEGIN_MESSAGE_MAP(CPhoneTypesDlg, CDialog)
	ON_BN_CLICKED(IDD_PHONE_TYPE_OK, &CPhoneTypesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDD_PHONE_TYPE_CANCEL, &CPhoneTypesDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------
CPhoneTypesDlg::CPhoneTypesDlg(PHONE_TYPES& recPhoneType, bool bPreview /*=false*/, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CPhoneTypesDlg, pParent)
	, m_bPreview(!bPreview)
	, m_recPhoneType(recPhoneType)
{

}

CPhoneTypesDlg::~CPhoneTypesDlg()
{

}


// Methods
// ----------------
void CPhoneTypesDlg::OnBnClickedOk()
{
	bool bTypeHasErrors = false;
	CString strErrorMessage;
	CString strTypeError = _T("Поле тип телефон:\n");
	CString strType;
	CEdit* pEditBoxToFocus = NULL;

	m_edbPhoneTypeEdit.GetWindowTextW(strType);

	strType = strType.Trim(_T(" "));
	std::wregex oNumbersRegex(_T("[0-9].*"));
	if (strType.IsEmpty())
	{
		CString strError;
		strError.Format(_T("\t%s\n"), _T("Не може да бъде празно"));
		strTypeError.Append(strError);
		bTypeHasErrors = true;
		if (pEditBoxToFocus == NULL) pEditBoxToFocus = &m_edbPhoneTypeEdit;
	}
	else if (std::regex_match((LPCTSTR)strType, oNumbersRegex))
	{
		CString strError;
		strError.Format(_T("\t%s\n"), _T("Не може да съдържа цифри"));
		strTypeError.Append(strError);
		if (pEditBoxToFocus == NULL) pEditBoxToFocus = &m_edbPhoneTypeEdit;
		bTypeHasErrors = true;
	}

	if (bTypeHasErrors)
		strErrorMessage.Append(strTypeError);


	if (!strErrorMessage.IsEmpty())
	{
		MessageBox(strErrorMessage);
		if (pEditBoxToFocus != NULL)
			pEditBoxToFocus->SetFocus();
		return;
	}

	m_recPhoneType = PHONE_TYPES(m_recPhoneType.lID, m_recPhoneType.lUpdateCounter, strType);
	CDialog::OnOK();
}

void CPhoneTypesDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();
}


// Overrides
// ----------------
void CPhoneTypesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PHONE_TYPE_NAME, m_edbPhoneTypeEdit);
}

BOOL CPhoneTypesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edbPhoneTypeEdit.SetLimitText(Length::PHONE_TYPES_TYPE - 1);
	m_edbPhoneTypeEdit.SetWindowTextW(m_recPhoneType.szType);
	m_edbPhoneTypeEdit.EnableWindow(!m_bPreview);

	return TRUE;
}