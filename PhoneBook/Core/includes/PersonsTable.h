#pragma once
#include "../includes/PersonsAccessor.h"
#include "../includes/DatabaseTables.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsTable

//  <summary>
//  Class for accessing table CITIES
//  </summary>
class CPersonsTable : public CDatabaseTables<CPersonsAccessor, PERSONS>
{
private:
    // Constants
    // ----------------


public:
    // Constructor / Destructor
    // ----------------
    CPersonsTable(CSession* pCommonSession = NULL);
    ~CPersonsTable();


    // Methods
    // ----------------
    bool SelectWhereCityId(long, CPersonsArray&);


    // Overrides
    // ----------------


    // Members
    // ----------------
private:
    CSession* m_pCommonSession;

};