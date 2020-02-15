
// PhoneBook.h : main header file for the PhoneBook application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"
#include "IterableMap.h"
#include "Core/includes/DatabaseConnectionManager.h"

enum OnUpdateHint
{
	OnEdit,
	OnInsert,
	OnDelete,
	OnReload
};

typedef CIterableMap<long, long, CITIES*, CITIES*> CMapCitiesByID;
typedef CIterableMap<CString, LPCTSTR, CMapCitiesByID*, CMapCitiesByID*> CMapCitiesByNameMapID;
typedef CIterableMap<long, long, PHONE_TYPES*, PHONE_TYPES*> CMapPhoneTypesByID;
typedef CIterableMap<long, long, PERSONS*, PERSONS*> CMapPersonsByID;
typedef CIterableMap<long, long, PHONE_NUMBERS*, PHONE_NUMBERS*> CMapPhoneNumbersByID;

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookApp
class CPhoneBookApp : public CWinApp
{
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------


// Constructor / Destructor
// ----------------
public:
	CPhoneBookApp() noexcept;


// Methods
// ----------------
public:
	void OnAppAbout();


// Overrides
// ----------------
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();


// Members
// ----------------
public:
	CDatabaseConnectionManager oDatabaseManager;
};

extern CPhoneBookApp theApp;