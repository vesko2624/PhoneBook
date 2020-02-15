#pragma once
#include "../includes/PhoneNumbersAccessor.h"
#include "../includes/DatabaseTables.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersTable

//  <summary>
//  Class for accessing table CITIES
//  </summary>
class CPhoneNumbersTable : public CDatabaseTables<CPhoneNumbersAccessor, PHONE_NUMBERS>
{
private:
// Constants
// ----------------


public:
// Constructor / Destructor
// ----------------
    CPhoneNumbersTable(CSession* pCommonSession = NULL);
    ~CPhoneNumbersTable();


// Methods
// ----------------
    bool SelectWherePersonID(long, CPhoneNumbersArray&);
    bool SelectWherePhoneTypesID(long, CPhoneNumbersArray&);


// Overrides
// ----------------


// Members
// ----------------
    CSession* m_pCommonSession;
};