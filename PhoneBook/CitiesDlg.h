#pragma once
#include "PhoneBook.h"
#include "afxdialogex.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesDlg

class CCitiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CCitiesDlg)
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------
private:
	enum { IDD = IDD_CCitiesDlg };


// Constructor / Destructor
// ----------------
public:
	CCitiesDlg(CITIES& recCity, bool bPreview = false, CWnd* pParent = nullptr);
	virtual ~CCitiesDlg();


// Methods
// ----------------
public:
	void OnBnClickedOk();
	void OnBnClickedCityCancel();


// Overrides
// ----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();


// Members
// ----------------
private:
	bool m_bPreview;
	CITIES& m_recCity;
	CEdit m_edbCityEdit;
	CEdit m_edbRegionEdit;
};