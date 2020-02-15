#pragma once
#include "PhoneBook.h"
#include "afxdialogex.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesDlg

class CPhoneTypesDlg : public CDialog
{
	DECLARE_DYNAMIC(CPhoneTypesDlg)
	DECLARE_MESSAGE_MAP()
	// Constants
	// ----------------
private:
	enum { IDD = IDD_CPhoneTypesDlg };


	// Constructor / Destructor
	// ----------------
public:
	CPhoneTypesDlg(PHONE_TYPES& recPhoneType, bool bPreview = false, CWnd* pParent = nullptr);
	virtual ~CPhoneTypesDlg();


	// Methods
	// ----------------
public:
	void OnBnClickedOk();
	void OnBnClickedCancel();


	// Overrides
	// ----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();


	// Members
	// ----------------
private:
	bool m_bPreview;
	PHONE_TYPES& m_recPhoneType;
	CEdit m_edbPhoneTypeEdit;
};