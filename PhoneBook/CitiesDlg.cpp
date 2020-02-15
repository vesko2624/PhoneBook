#include "pch.h"
#include "PhoneBook.h"
#include "CitiesDlg.h"
#include <regex>


/////////////////////////////////////////////////////////////////////////////
// CCitiesDlg

IMPLEMENT_DYNAMIC(CCitiesDlg, CDialog)

BEGIN_MESSAGE_MAP(CCitiesDlg, CDialog)
	ON_BN_CLICKED(ID_CITY_OK, &CCitiesDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CITY_CANCEL, &CCitiesDlg::OnBnClickedCityCancel)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------
CCitiesDlg::CCitiesDlg(CITIES& recCity, bool bPreview /*=false*/, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CCitiesDlg, pParent)
	, m_bPreview(!bPreview)
	, m_recCity(recCity)
{

}

CCitiesDlg::~CCitiesDlg()
{

}


// Methods
// ----------------
void CCitiesDlg::OnBnClickedOk()
{
	bool bCityHasErrors = false;
	bool bRegionHasErrors = false;
	CString strErrorMessage;
	CString strCityError = _T("Поле Град:\n");
	CString strRegionError = _T("Поле Регион:\n");
	CString strName, strRegion;
	CEdit* pEditBoxToFocus = NULL;
	
	m_edbCityEdit.GetWindowTextW(strName);
	m_edbRegionEdit.GetWindowTextW(strRegion);
	
	strName = strName.Trim(_T(" "));
	strRegion = strRegion.Trim(_T(" "));
	std::wregex oNumbersRegex(_T("[0-9].*"));
	if (strName.IsEmpty())
	{
		CString strError;
		strError.Format(_T("\t%s\n"), _T("Не може да бъде празно"));
		strCityError.Append(strError);
		bCityHasErrors = true;
		if (pEditBoxToFocus == NULL) pEditBoxToFocus = &m_edbCityEdit;
	}
	else if (std::regex_match((LPCTSTR)strName, oNumbersRegex))
	{
		CString strError;
		strError.Format(_T("\t%s\n"), _T("Не може да съдържа цифри"));
		strCityError.Append(strError);
		if (pEditBoxToFocus == NULL) pEditBoxToFocus = &m_edbCityEdit;
		bCityHasErrors = true;
	}

	if (strRegion.IsEmpty())
	{
		CString strError;
		strError.Format(_T("\t%s\n"), _T("Не може да бъде празно"));
		strRegionError.Append(strError);
		bRegionHasErrors = true;
		if (pEditBoxToFocus == NULL) pEditBoxToFocus = &m_edbRegionEdit;
	}
	else if (std::regex_match((LPCTSTR)strRegion, oNumbersRegex))
	{
		CString strError;
		strError.Format(_T("\t%s\n"), _T("Не може да съдържа цифри"));
		strRegionError.Append(strError);
		if (pEditBoxToFocus == NULL) pEditBoxToFocus = &m_edbRegionEdit;
		bRegionHasErrors = true;
	}
	if(bCityHasErrors)
		strErrorMessage.Append(strCityError);
	if (bRegionHasErrors)
	{
		if (strErrorMessage.IsEmpty() == false)
			strErrorMessage.AppendChar(_T('\n'));
		strErrorMessage.Append(strRegionError);
	}


	if (!strErrorMessage.IsEmpty())
	{
		MessageBox(strErrorMessage);
		if (pEditBoxToFocus != NULL)
			pEditBoxToFocus->SetFocus();
		return;
	}

	m_recCity = CITIES(m_recCity.lID, m_recCity.lUpdateCounter, strName, strRegion);
	CDialog::OnOK();
}

void CCitiesDlg::OnBnClickedCityCancel()
{
	CDialog::OnCancel();
}


// Overrides
// ----------------
void CCitiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CITYNAME, m_edbCityEdit);
	DDX_Control(pDX, IDC_REGIONNAME, m_edbRegionEdit);
}

BOOL CCitiesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edbCityEdit.SetLimitText(Length::CITIES_NAME - 1);
	m_edbCityEdit.SetWindowTextW(m_recCity.szName);
	m_edbCityEdit.EnableWindow(!m_bPreview);

	m_edbRegionEdit.SetLimitText(Length::CITIES_REGION - 1);
	m_edbRegionEdit.SetWindowTextW(m_recCity.szRegion);
	m_edbRegionEdit.EnableWindow(!m_bPreview);

	return TRUE;
}