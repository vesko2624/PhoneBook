#include "../includes/PhoneTypesTable.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesTable

// Constructor / Destructor
// ----------------
//  <summary>
//  Calls CDatabaseTables constructor 
//  with the name of the table as parameter
//  </summary>
CPhoneTypesTable::CPhoneTypesTable() :
    CDatabaseTables(_T("PHONE_TYPES"))
{

}

CPhoneTypesTable::~CPhoneTypesTable()
{

}


// Methods
// ----------------


// Overrides
// ----------------

