#pragma once
#include "../includes/Structures.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesAccessor

// <summary>
// Class meant to map table columns into a structure
// </sumamry>
class CPhoneTypesAccessor
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
    BEGIN_ACCESSOR_MAP(CPhoneTypesAccessor, 2)
        BEGIN_ACCESSOR(0, true)
        COLUMN_ENTRY(1, m_recTableRow.lID)
        END_ACCESSOR()

        BEGIN_ACCESSOR(1, true)
        COLUMN_ENTRY(2, m_recTableRow.lUpdateCounter)
        COLUMN_ENTRY(3, m_recTableRow.szType)
        END_ACCESSOR()
    END_ACCESSOR_MAP()


// Members
// ----------------
protected:
    PHONE_TYPES m_recTableRow;
};