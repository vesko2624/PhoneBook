#include "../includes/PersonsTable.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsTable

// Constructor / Destructor
// ----------------
//	<summary>
//	Calls CDatabaseTables constructor 
//	with the name of the table as parameter
//	</summary>
CPersonsTable::CPersonsTable(CSession* pCommonSession /*=NULL*/)
	: CDatabaseTables(_T("PERSONS"), pCommonSession)
	, m_pCommonSession(pCommonSession)
{

}

CPersonsTable::~CPersonsTable()
{
	if (m_pCommonSession != NULL)
	{
		m_pCommonSession->Close();
	}
}


// Methods
// ----------------
bool CPersonsTable::SelectWhereCityId(long lID, CPersonsArray& oPersonsPtrArray)
{
	HRESULT hResult = S_FALSE;
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM PERSONS WHERE CITY_ID = %d"), lID);
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
				oPersonsPtrArray.Add(new PERSONS(m_recTableRow));
				break;

			case DB_S_ENDOFROWSET:
				break;

			default:
			{
				CString strError;
				strError.Format(_T("Error. MoveNext() returned %ld"), hResult);
				throw (LPCTSTR)strError;
			}
		}
	} while (hResult == S_OK);
	
	return (bool)oPersonsPtrArray.GetSize();
}


// Overrides
// ----------------

