#pragma once
#include "Core/includes/PhoneTypesTable.h"
#include "Core/includes/Structures.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesData

class CPhoneTypesData : private CPhoneTypesTable
{
	// Constants
	// ----------------


	// Constructor / Destructor
	// ----------------


	// Methods
	// ----------------
public:
	bool SelectAll(CPhoneTypesArray& oPhoneTypesArray);
	bool SelectWhereID(long lID, PHONE_TYPES& recPhoneType);
	bool UpdateWhereID(long lID, PHONE_TYPES& recPhoneType);
	bool DeleteWhereID(long lID);
	bool InsertPhoneType(PHONE_TYPES& recPhoneType);


	// Overrides
	// ----------------


	// Members
	// ----------------


};
