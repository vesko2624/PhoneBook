#pragma once
#include "../includes/PhoneTypesAccessor.h"
#include "../includes/DatabaseTables.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesTable

//  <summary>
//  Class for accessing table CITIES
//  </summary>
class CPhoneTypesTable : public CDatabaseTables<CPhoneTypesAccessor, PHONE_TYPES>
{
private:
    // Constants
    // ----------------


public:
    // Constructor / Destructor
    // ----------------
    CPhoneTypesTable();
    ~CPhoneTypesTable();


    // Methods
    // ----------------


    // Overrides
    // ----------------


    // Members
    // ----------------


};