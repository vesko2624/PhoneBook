#include "../includes/CitiesTable.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

// Constructor / Destructor
// ----------------
//  <summary>
//  Calls CDatabaseTables constructor 
//  with the name of the table as parameter
//  </summary>
CCitiesTable::CCitiesTable() :
    CDatabaseTables(_T("CITIES"))
{

}

CCitiesTable::~CCitiesTable()
{

}


// Methods
// ----------------
bool CCitiesTable::SelectCitiesWhereName(CString& strCityName, CCitiesArray& oCitiesArray)
{
	CString strQuery;
	HRESULT hResult = S_FALSE;
	strQuery.Format(_T("SELECT * FROM CITIES WHERE NAME = %s"), (LPCTSTR)strCityName);
	if (m_oQueryExecutor.Open(strQuery) == FALSE)
	{
		return FALSE;
	}

	do
	{
		hResult = MoveNext();
		switch (hResult)
		{
		case S_OK:
			oCitiesArray.Add(new CITIES(m_recTableRow));
			break;

		case DB_S_ENDOFROWSET:
			break;

		default:
		{
			CString strError;
			strError.Format(_T("Error. MoveNext returned %ld"), hResult);
			throw (LPCTSTR)strError;
		}
		}
	} while (hResult == S_OK);
	return true;
}

// Overrides
// ----------------

