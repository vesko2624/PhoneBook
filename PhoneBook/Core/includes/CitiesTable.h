#pragma once
#include "../includes/CitiesAccessor.h"
#include "../includes/DatabaseTables.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

//  <summary>
//  Class for accessing table CITIES
//  </summary>
class CCitiesTable : public CDatabaseTables<CCitiesAccessor, CITIES>
{
private:
    // Constants
    // ----------------


public:
    // Constructor / Destructor
    // ----------------
    CCitiesTable();
    ~CCitiesTable();


    // Methods
    // ----------------
    bool SelectCitiesWhereName(CString& strCityName, CCitiesArray& oCitiesArray);

    // Overrides
    // ----------------


    // Members
    // ----------------


};