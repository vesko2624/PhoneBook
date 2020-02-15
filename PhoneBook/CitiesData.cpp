#include "CitiesData.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesData

// Constructor / Destructor
// ----------------


// Methods
// ----------------
//  <summary>
//  Selects all cities from the database and store it in array
//  </summary>
//	<param name="oCitiesArray"> Array for storing the cities </param>
bool CCitiesData::SelectAll(CCitiesArray& oCitiesArray)
{
	bool bResult = CCitiesTable::SelectAll(oCitiesArray);
	return bResult;
}

//  <summary>
//  Select a city with given ID from the database
//  </summary>
//	<param name="lID"> ID of the city for selection </param>
//	<param name="recCity"> Reference to a place to store the record </param>
bool CCitiesData::SelectWhereID(long lID, CITIES& recCity)
{
	bool bResult = CCitiesTable::SelectWhereID(lID, recCity);
	return bResult;
}

//  <summary>
//  Select all cities with a given name
//  </summary>
//	<param name="strCityName"> Name to look for </param>
//	<param name="oCitiesArray"> Array to store found cities </param>
bool CCitiesData::SelectCitiesWhereName(CString& strCityName, CCitiesArray& oCitiesArray)
{
	bool bResult = CCitiesTable::SelectCitiesWhereName(strCityName, oCitiesArray);
	return bResult;
}

//  <summary>
//  Update a city with given ID in the database,
//	The update counter is raised by 1 for both the database
//	and the array
//  </summary>
//	<param name="lID"> ID of the city for update </param>
//	<param name="recCity"> Reference to a city storing the new information </param>
bool CCitiesData::UpdateWhereID(long lID, CITIES& recCity)
{
	m_oSession.StartTransaction();
	bool bResult = CCitiesTable::UpdateWhereID(lID, recCity);

	if (!bResult)
		m_oSession.Abort();

	m_oSession.Commit();

	return bResult;
}

//  <summary>
//  Delete a city with given ID from the database
//  </summary>
//	<param name="lID"> ID of the city for removing </param>
bool CCitiesData::DeleteWhereID(long lID)
{
	m_oSession.StartTransaction();
	bool bResult = CCitiesTable::DeleteWhereID(lID);

	if (!bResult)
		m_oSession.Abort();
		
	m_oSession.Commit();

	return bResult;
}

//  <summary>
//  Insert a city in the database,
//	After insertion, the given record changes its ID with the generated ID
//	from the database
//  </summary>
//	<param name="recCity"> Reference to the city to be inserted </param>
bool CCitiesData::InsertCity(CITIES& recCity)
{
	bool bResult = CCitiesTable::InsertRecord(recCity);
	return bResult;
}


// Overrides
// ----------------


