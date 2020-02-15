#include "PersonsData.h"

/////////////////////////////////////////////////////////////////////////////
// PersonsData

// Constructor / Destructor
// ----------------
CPersonsData::CPersonsData()
	: CPersonsTable(&m_oCommonSession)
	, CPhoneNumbersTable(&m_oCommonSession)
{
	CDatabaseConnectionManager::OpenSession(m_oCommonSession);
}

CPersonsData::~CPersonsData()
{
	CDatabaseConnectionManager::CloseSession(m_oCommonSession);
}

// Methods
// ----------------
bool CPersonsData::SelectPersonWhereID(long lPersonID, PERSONS& recPerson)
{
	bool bResult = CPersonsTable::SelectWhereID(lPersonID, recPerson);
	return bResult;
}

bool CPersonsData::SelectPhoneNumberWhereID(long lPhoneNumberID, PHONE_NUMBERS& recPhoneNumber)
{
	bool bResult = CPhoneNumbersTable::SelectWhereID(lPhoneNumberID, recPhoneNumber);
	return bResult;
}

bool CPersonsData::SelectAllPersons(CPersonsArray& oPersonsArray)
{
	bool bResult = CPersonsTable::SelectAll(oPersonsArray);
	return bResult;
}

bool CPersonsData::SelectAllPhoneNumers(long lPersonID, CPhoneNumbersArray& oPhoneNumbersArray)
{
	bool bResult = CPhoneNumbersTable::SelectWherePersonID(lPersonID, oPhoneNumbersArray);
	return bResult;
}

bool CPersonsData::UpdatePersonWhereID(long lPersonID, PERSONS& recPerson)
{
	m_oCommonSession.StartTransaction();
	bool bResult = CPersonsTable::UpdateWhereID(lPersonID, recPerson);
	
	if (!bResult)
		m_oCommonSession.Abort();
		
	m_oCommonSession.Commit();

	return bResult;
}

bool CPersonsData::UpdatePhoneNumberWhereID(long lPhoneNumberID, PHONE_NUMBERS& recPhoneNumber)
{
	m_oCommonSession.StartTransaction();
	bool bResult = CPhoneNumbersTable::UpdateWhereID(lPhoneNumberID, recPhoneNumber);

	if (!bResult)
		m_oCommonSession.Abort();
		
	m_oCommonSession.Commit();
	
	return bResult;
}

bool CPersonsData::DeletePersonWhereID(long lPersonID)
{
	bool bResult;
	CPhoneNumbersArray oPhoneNumbersPtrArray;
	bResult = CPhoneNumbersTable::SelectWherePersonID(lPersonID, oPhoneNumbersPtrArray);

	if (bResult == false)
		return false;

	m_oCommonSession.StartTransaction();

	while (oPhoneNumbersPtrArray.IsEmpty() == false)
	{
		bResult = CPhoneNumbersTable::DeleteWhereID(oPhoneNumbersPtrArray.GetAt(0)->lID);
		if (bResult == false)
		{
			m_oCommonSession.Abort();
			return false;
		}
		delete oPhoneNumbersPtrArray.GetAt(0);
		oPhoneNumbersPtrArray.RemoveAt(0);
	}

	bResult = CPersonsTable::DeleteWhereID(lPersonID);
	if (bResult == false)
	{
		m_oCommonSession.Abort();
		return false;
	}

	m_oCommonSession.Commit();
	return true;
}

bool CPersonsData::DeletePhoneNumberWhereID(long lPhoneNumberID)
{
	bool bResult = CPhoneNumbersTable::DeleteWhereID(lPhoneNumberID);

	return bResult;
}

bool CPersonsData::InsertPerson(PERSONS& recPerson)
{
	bool bResult = CPersonsTable::InsertRecord(recPerson);

	return bResult;
}

bool CPersonsData::InsertPhoneNumber(PHONE_NUMBERS& recPhoneNumber)
{
	bool bResult = CPhoneNumbersTable::InsertRecord(recPhoneNumber);

	return bResult;
}

bool CPersonsData::InsertMultiplePhoneNumbers(PersonInfo& oPersonInfo)
{
	bool bResultFlag = true;

	CMapPhoneNumbersByID& oPhoneNumbersMapByID = oPersonInfo.oPhoneNumbersMapByID;

	CMapPhoneNumbersByID& oPhoneNumbersInsertErrorMapByID =
		oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_INSERT_ERROR);

	CMapPhoneNumbersByID& oPhoneNumbersToInsertMapByID =
		oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_TO_INSERT);

	POSITION oPhoneNumbersMapItter =
		oPhoneNumbersToInsertMapByID.GetStartPosition();

	while (oPhoneNumbersMapItter != NULL)
	{
		long lKeyID;
		PHONE_NUMBERS* pPhoneNumber;

		oPhoneNumbersToInsertMapByID.GetNextAssoc(oPhoneNumbersMapItter, lKeyID, pPhoneNumber);

		if (pPhoneNumber == NULL)
		{
			oPhoneNumbersToInsertMapByID.RemoveKey(lKeyID);
			continue;
		}

		pPhoneNumber->lPersonId = oPersonInfo.recPerson.lID;

		if (InsertPhoneNumber(*pPhoneNumber) == true)
		{
			oPhoneNumbersMapByID.RemoveKey(lKeyID);
			oPhoneNumbersToInsertMapByID.RemoveKey(lKeyID);
			delete pPhoneNumber;
		}
		else
		{
			bResultFlag = false;
			PHONE_NUMBERS* pPhoneNumberToInsertError = NULL;
			if (oPhoneNumbersInsertErrorMapByID.Lookup(lKeyID, pPhoneNumberToInsertError) == TRUE && pPhoneNumberToInsertError != NULL)
			{
				delete pPhoneNumberToInsertError;
				oPhoneNumbersInsertErrorMapByID.RemoveKey(lKeyID);
			}

			oPhoneNumbersInsertErrorMapByID.SetAt(lKeyID, pPhoneNumber);
			oPhoneNumbersToInsertMapByID.RemoveKey(lKeyID);
		}
	}

	return bResultFlag;
}

bool CPersonsData::InsertPair(PersonInfo& oPersonInfo)
{
	m_oCommonSession.StartTransaction();

	if (InsertPerson(oPersonInfo.recPerson) == false)
	{
		m_oCommonSession.Abort();
		return false;
	}

	// We dont care for the result of that function
	// InsertPair return true if person can be inserted
	// it doesn't care how much of the phone numbers
	// were succesfully inserted
	InsertMultiplePhoneNumbers(oPersonInfo);
	m_oCommonSession.Commit();
	return true;
}

bool CPersonsData::DeleteMultiplePhoneNumbers(PersonInfo& oPersonInfo)
{
	bool bResultFlag = true;
	CMapPhoneNumbersByID& oPhoneNumbersMapByID = oPersonInfo.oPhoneNumbersMapByID;

	CMapPhoneNumbersByID& oPhoneNumbersToDeleteMapByID =
		oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_TO_DELETE);

	CMapPhoneNumbersByID& oPhoneNumbersToDeleteErrorMapByID =
		oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_INSERT_ERROR);

	POSITION oPhoneNumbersToDeleteMapByIDItter = oPhoneNumbersToDeleteMapByID.GetStartPosition();
	while (oPhoneNumbersToDeleteMapByIDItter != NULL)
	{
		long lKeyID;
		PHONE_NUMBERS* pPhoneNumber;
		oPhoneNumbersToDeleteMapByID.GetNextAssoc(oPhoneNumbersToDeleteMapByIDItter, lKeyID, pPhoneNumber);

		if (pPhoneNumber == NULL)
		{
			oPhoneNumbersToDeleteMapByID.RemoveKey(lKeyID);
			continue;
		}

		if (DeletePhoneNumberWhereID(pPhoneNumber->lID) == false)
		{
			bResultFlag = true;
			
			PHONE_NUMBERS* pPhoneNumberToDeleteError;
			if(oPhoneNumbersToDeleteErrorMapByID.Lookup(lKeyID, pPhoneNumberToDeleteError) && pPhoneNumberToDeleteError != NULL)
			{
				delete pPhoneNumberToDeleteError;
				oPhoneNumbersToDeleteErrorMapByID.RemoveKey(lKeyID);
			}

			oPhoneNumbersToDeleteErrorMapByID.SetAt(lKeyID, pPhoneNumber);
			oPhoneNumbersToDeleteMapByID.RemoveKey(lKeyID);
		}
		else
		{
			oPhoneNumbersMapByID.RemoveKey(lKeyID);
			oPhoneNumbersToDeleteMapByID.RemoveKey(lKeyID);
			delete pPhoneNumber;
		}
	}

	return bResultFlag;
}

bool CPersonsData::EditMultiplePhoneNumbers(PersonInfo& oPersonInfo)
{
	bool bResultFlag = true;

	CMapPhoneNumbersByID& oPhoneNumbersMapByID = oPersonInfo.oPhoneNumbersMapByID;

	CMapPhoneNumbersByID& oPhoneNumbersToEditMapByID =
		oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_TO_EDIT);

	CMapPhoneNumbersByID& oPhoneNumbersToEditErrorMapByID =
		oPersonInfo.oHintArray.GetAt((INT_PTR)RecordActions::DB_TO_INSERT);

	POSITION oPhoneNumbersToEditMapByIDItter = oPhoneNumbersToEditMapByID.GetStartPosition();
	while (oPhoneNumbersToEditMapByIDItter != NULL)
	{
		long lKeyID;
		PHONE_NUMBERS* pPhoneNumber;
		oPhoneNumbersToEditMapByID.GetNextAssoc(oPhoneNumbersToEditMapByIDItter, lKeyID, pPhoneNumber);
		
		if (pPhoneNumber == NULL)
		{
			oPhoneNumbersToEditMapByID.RemoveKey(lKeyID);
			continue;
		}

		if (UpdatePhoneNumberWhereID(pPhoneNumber->lID, *pPhoneNumber) == false)
		{
			bResultFlag = false;

			PHONE_NUMBERS* pPhoneNumberToEditError;
			if (oPhoneNumbersToEditErrorMapByID.Lookup(lKeyID, pPhoneNumberToEditError) && pPhoneNumberToEditError != NULL)
			{
				delete pPhoneNumberToEditError;
				oPhoneNumbersToEditErrorMapByID.RemoveKey(lKeyID);
			}

			oPhoneNumbersToEditErrorMapByID.SetAt(lKeyID, pPhoneNumber);
			oPhoneNumbersToEditMapByID.RemoveKey(lKeyID);
		}
		else
		{
			oPhoneNumbersMapByID.RemoveKey(lKeyID);
			oPhoneNumbersToEditMapByID.RemoveKey(lKeyID);
			delete pPhoneNumber;
		}
	}

	return bResultFlag;
}

bool CPersonsData::EditPersonInfo(PersonInfo& oPersonInfo)
{
	bool bResult = true;
	bResult = bResult && UpdatePersonWhereID(oPersonInfo.recPerson.lID, oPersonInfo.recPerson);
	bResult = bResult && InsertMultiplePhoneNumbers(oPersonInfo);
	bResult = bResult && DeleteMultiplePhoneNumbers(oPersonInfo);
	bResult = bResult && EditMultiplePhoneNumbers(oPersonInfo);

	return bResult;
}
// Overrides
// ----------------

