#pragma once
#include "framework.h"
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg

class CAboutDlg : public CDialogEx
{
	DECLARE_MESSAGE_MAP()
	// Constants
	// ----------------
private:
	enum { IDD = IDD_ABOUTBOX };


	// Constructor / Destructor
	// ----------------
public:
	CAboutDlg() noexcept;


	// Methods
	// ----------------


	// Overrides
	// ----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
};