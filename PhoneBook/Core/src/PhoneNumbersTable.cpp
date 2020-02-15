#include "../includes/PhoneNumbersTable.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersTable

// Constructor / Destructor
// ----------------
//	<summary>
//	Calls CDatabaseTables constructor 
//	with the name of the table as parameter
//	<summary>
CPhoneNumbersTable::CPhoneNumbersTable(CSession* pCommonSession /*=NULL*/)
	: CDatabaseTables(_T("PHONE_NUMBERS"), pCommonSession)
	, m_pCommonSession(pCommonSession)
{

}

CPhoneNumbersTable::~CPhoneNumbersTable()
{
	if (m_pCommonSession != NULL)
	{
		m_pCommonSession->Close();
	}
}


// Methods
// ----------------
bool CPhoneNumbersTable::SelectWherePersonID(long lID, CPhoneNumbersArray& oPhoneNumbersArray)
{
	HRESULT hResult = S_FALSE;
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM PHONE_NUMBERS WHERE PERSON_ID = %d"), lID);
	if (m_oQueryExecutor.Open(strQuery) == FALSE)
	{
		return false;
	}

	do
	{
		hResult = MoveNext();
		switch (hResult)
		{
			case S_OK:
				oPhoneNumbersArray.Add(new PHONE_NUMBERS(m_recTableRow));
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
	}while (hResult == S_OK);
	return true;
}

bool CPhoneNumbersTable::SelectWherePhoneTypesID(long lID, CPhoneNumbersArray& oPhoneNumbersArray)
{
	HRESULT hResult = S_FALSE;
	CString strQuery;

	strQuery.Format(_T("SELECT * FROM PHONE_NUMBERS WHERE PHONE_TYPES_ID = %d"), lID);
	if (m_oQueryExecutor.Open(strQuery) == FALSE)
	{
		return false;
	}

	do
	{
		hResult = MoveNext();
		switch (hResult)
		{
			case S_OK:
				oPhoneNumbersArray.Add(new PHONE_NUMBERS(m_recTableRow));
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

	return true;
}


// Overrides
// ----------------

