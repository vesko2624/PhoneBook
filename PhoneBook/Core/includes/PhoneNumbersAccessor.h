#pragma once
#include "../includes/Structures.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersAccessor

// <summary>
// Class meant to map table columns into a structure
// </sumamry>
class CPhoneNumbersAccessor
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
    BEGIN_ACCESSOR_MAP(CPhoneNumbersAccessor, 2)
        BEGIN_ACCESSOR(0, true)
        COLUMN_ENTRY(1, m_recTableRow.lID)
        END_ACCESSOR()

        BEGIN_ACCESSOR(1, true)
        COLUMN_ENTRY(2, m_recTableRow.lUpdateCounter)
        COLUMN_ENTRY(3, m_recTableRow.lPersonId)
        COLUMN_ENTRY(4, m_recTableRow.lPhoneTypesId)
        COLUMN_ENTRY(5, m_recTableRow.szNumber)
        END_ACCESSOR()
    END_ACCESSOR_MAP()


// Members
// ----------------
protected:
    PHONE_NUMBERS m_recTableRow;
};