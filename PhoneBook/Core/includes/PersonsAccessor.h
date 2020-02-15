#pragma once
#include "../includes/Structures.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsAccessor

// <summary>
// Class meant to map table columns into a structure
// </sumamry>
class CPersonsAccessor
{
// Constants
// ----------------


// Constructor / Destructor
// ----------------


// Methods
// ----------------


// Overrides
// ----------------
protected:
    BEGIN_ACCESSOR_MAP(CPersonsAccessor, 2)
        BEGIN_ACCESSOR(0, true)
        COLUMN_ENTRY(1, m_recTableRow.lID)
        END_ACCESSOR()

        BEGIN_ACCESSOR(1, true)
        COLUMN_ENTRY(2, m_recTableRow.lUpdateCounter)
        COLUMN_ENTRY(3, m_recTableRow.szFirstName)
        COLUMN_ENTRY(4, m_recTableRow.szMiddleName)
        COLUMN_ENTRY(5, m_recTableRow.szLastName)
        COLUMN_ENTRY(6, m_recTableRow.szIdentificationNumber)
        COLUMN_ENTRY(7, m_recTableRow.lCityId)
        COLUMN_ENTRY(8, m_recTableRow.szAddress)
        END_ACCESSOR()
    END_ACCESSOR_MAP()


// Members
// ----------------
protected:
    PERSONS m_recTableRow;
};