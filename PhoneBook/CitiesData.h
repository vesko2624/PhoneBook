#pragma once
#include "Core/includes/CitiesTable.h"
#include "Core/includes/Structures.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesData

class CCitiesData : private CCitiesTable
{
// Constants
// ----------------


// Constructor / Destructor
// ----------------


// Methods
// ----------------
public:
	bool SelectAll(CCitiesArray&);
	bool SelectWhereID(long lID, CITIES& recCity);
	bool SelectCitiesWhereName(CString& strCityName, CCitiesArray& oCitiesArray);
	bool UpdateWhereID(long lID, CITIES& recCity);
	bool DeleteWhereID(long lID);
	bool InsertCity(CITIES& recCity);


// Overrides
// ----------------


// Members
// ----------------


};
