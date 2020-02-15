#include "PhoneTypesData.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesData

// Constructor / Destructor
// ----------------


// Methods
// ----------------
//  <summary>
//  Selects all records from the database and store it in array
//  </summary>
//	<param name="oPhoneTypesArray"> Array for storing the records </param>
bool CPhoneTypesData::SelectAll(CPhoneTypesArray& oPhoneTypesArray)
{
	bool bResult = CPhoneTypesTable::SelectAll(oPhoneTypesArray);

	return bResult;
}

//  <summary>
//  Select a record with given ID from the database
//  </summary>
//	<param name="lID"> ID of the record for selection </param>
//	<param name="recPhoneType"> Reference to a place to store the record </param>
bool CPhoneTypesData::SelectWhereID(long lID, PHONE_TYPES& recPhoneType)
{
	bool bResult = CPhoneTypesTable::SelectWhereID(lID, recPhoneType);

	return bResult;
}

//  <summary>
//  Update a record with given ID in the database,
//	The update counter is raised by 1 for both the database
//	and the given record
//  </summary>
//	<param name="lID"> ID of the record for update </param>
//	<param name="recPhoneType"> Reference to a record storing the new information </param>
bool CPhoneTypesData::UpdateWhereID(long lID, PHONE_TYPES& recPhoneType)
{
	m_oSession.StartTransaction();
	bool bResult = CPhoneTypesTable::UpdateWhereID(lID, recPhoneType);

	if (!bResult)
		m_oSession.Abort();

	m_oSession.Commit();

	return bResult;
}

//  <summary>
//  Delete a record with given ID from the database
//  </summary>
//	<param name="lID"> ID of the record for delete </param>
bool CPhoneTypesData::DeleteWhereID(long lID)
{
	bool bResult = CPhoneTypesTable::DeleteWhereID(lID);

	return bResult;
}

//  <summary>
//  Insert a record in the database,
//	After insertion, the given record changes its ID with the generated ID
//	from the database
//  </summary>
//	<param name="recPhoneType"> Reference to the record to be inserted </param>
bool CPhoneTypesData::InsertPhoneType(PHONE_TYPES& recPhoneType)
{
	bool bResult = CPhoneTypesTable::InsertRecord(recPhoneType);

	return bResult;
}


// Overrides
// ----------------


