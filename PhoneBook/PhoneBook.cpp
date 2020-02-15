#include "pch.h"
#include "framework.h"
#include "PhoneBook.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include "CitiesDocument.h"
#include "CitiesView.h"

#include "PersonsDocument.h"
#include "PersonsView.h"

#include "PhoneTypesDocument.h"
#include "PhoneTypesView.h"

#include "AboutDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CPhoneBookApp
//class CAboutDlg;
// The one and only CPhoneBookApp object
CPhoneBookApp theApp;

BEGIN_MESSAGE_MAP(CPhoneBookApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CPhoneBookApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------
CPhoneBookApp::CPhoneBookApp() noexcept
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
	SetAppID(_T("PhoneBook.AppID.NoVersion"));
}


// Methods
// ----------------
void CPhoneBookApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Overrides
// ----------------
BOOL CPhoneBookApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_CitiesTableName,
		RUNTIME_CLASS(CCitiesDocument),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CCitiesView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_PersonsTableName,
		RUNTIME_CLASS(CPersonsDocument),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CPersonsView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_PhoneTypesTableName,
		RUNTIME_CLASS(CPhoneTypesDocument),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CPhoneTypesView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	POSITION pos = GetFirstDocTemplatePosition();
	while (pos)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)GetNextDocTemplate(pos);
		pTemplate->OpenDocumentFile(NULL);
	}

	return TRUE;
}

int CPhoneBookApp::ExitInstance()
{
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}
