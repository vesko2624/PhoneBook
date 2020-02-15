#include "pch.h"
#include "PersonsDocument.h"
#include "Core/includes/Structures.h"
#include "Core/includes/PersonsTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CPersonsDocument

IMPLEMENT_DYNCREATE(CPersonsDocument, CDocument)

BEGIN_MESSAGE_MAP(CPersonsDocument, CDocument)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------
CPersonsDocument::CPersonsDocument()
{

}

CPersonsDocument::~CPersonsDocument()
{
}


// Methods
// ----------------
bool CPersonsDocument::LoadPersons()
{
	bool bResult;
	CPersonsArray oPersonsPtrArray;

	bResult = SelectAllPersons(oPersonsPtrArray);
	if (bResult == false)
	{
		return false;
	}

	m_oMapPersonsByID.ClearMap();
	while (oPersonsPtrArray.IsEmpty() == false)
	{
		PERSONS* pPerson = oPersonsPtrArray.GetAt(0);
		m_oMapPersonsByID.SetAt(pPerson->lID, pPerson);
		oPersonsPtrArray.RemoveAt(0);
	}

	return true;
}

void CPersonsDocument::RemovePersonFromMap(long lID)
{

	PERSONS* pPerson;
	PERSONS recPersonToRemoveFromView;

	if (m_oMapPersonsByID.Lookup(lID, pPerson) && pPerson != NULL)
	{
		recPersonToRemoveFromView = *pPerson;
		delete pPerson;
		m_oMapPersonsByID.RemoveKey(lID);
	}
	else
	{
		recPersonToRemoveFromView.lID = lID;
	}

	CObject* pUpdateHint = reinterpret_cast<CObject*>(&recPersonToRemoveFromView);
	UpdateAllViews(NULL, OnDelete, pUpdateHint);
}

bool CPersonsDocument::LoadCities()
{
	bool bResult;
	CCitiesArray oCitiesPtrArray;

	bResult = CCitiesData::SelectAll(oCitiesPtrArray);
	if (bResult == false)
	{	
		return false;
	}

	m_oMapCitiesByNameID.ClearMap();
	while (oCitiesPtrArray.IsEmpty() == false)
	{
		CITIES* pCurrentCity = oCitiesPtrArray.GetAt(0);
		CMapCitiesByNameMapID::CPair* pPairStringMap;

		pPairStringMap = m_oMapCitiesByNameID.PLookup(pCurrentCity->szName);
		if (pPairStringMap == NULL)
		{
			CMapCitiesByID* pMapCitiesByID = new CMapCitiesByID;

			pMapCitiesByID->SetAt(pCurrentCity->lID, pCurrentCity);
			m_oMapCitiesByNameID.SetAt(pCurrentCity->szName, pMapCitiesByID);
		}
		else
		{
			CMapCitiesByID* pMapCitiesByID = pPairStringMap->value;
			pMapCitiesByID->SetAt(pCurrentCity->lID, pCurrentCity);
		}
		oCitiesPtrArray.RemoveAt(0);
	}
	return true;
}

void CPersonsDocument::RemoveCityFromMap(long lID)
{
	POSITION oMainMapItter = m_oMapCitiesByNameID.GetStartPosition();
	for (INT_PTR i = 0; i < m_oMapCitiesByNameID.GetCount(); ++i)
	{
		CMapCitiesByID* pSubMap;
		m_oMapCitiesByNameID.GetAtIndex(i, pSubMap);
		if (pSubMap != NULL && pSubMap->RemoveAtKey(lID))
		{
			return;
		}
	}
}

bool CPersonsDocument::LoadPhoneTypes()
{
	CPhoneTypesArray oPhoneTypesPtrArray;
	bool bResult;

	bResult = CPhoneTypesData::SelectAll(oPhoneTypesPtrArray);
	if (bResult == false)
	{
		return false;
	}

	m_oMapPhoneTypesByID.ClearMap();
	while (oPhoneTypesPtrArray.IsEmpty() == false)
	{
		PHONE_TYPES* pPhoneType = oPhoneTypesPtrArray.GetAt(0);
		m_oMapPhoneTypesByID.SetAt(pPhoneType->lID, pPhoneType);
		oPhoneTypesPtrArray.RemoveAt(0);
	}

	return true;
}

bool CPersonsDocument::SelectPersonWhereID(long lID, PERSONS& recPerson)
{
	return CPersonsData::SelectPersonWhereID(lID, recPerson);
}

bool CPersonsDocument::SelectPhoneNumberWhereID(long lPhoneNumberID, PHONE_NUMBERS& recPhoneNumber)
{
	return CPersonsData::SelectPhoneNumberWhereID(lPhoneNumberID, recPhoneNumber);
}

bool CPersonsDocument::SelectAllPersons(CPersonsArray& oPersonsArray)
{
	return CPersonsData::SelectAllPersons(oPersonsArray);
}

bool CPersonsDocument::SelectAllPhoneNumers(long lPersonID, CMapPhoneNumbersByID& oPhoneNumbersMapByID)
{
	CPhoneNumbersArray oPhoneNumbersPtrArray;
	if (CPersonsData::SelectAllPhoneNumers(lPersonID, oPhoneNumbersPtrArray) == false)
	{
		return false;
	}

	while (oPhoneNumbersPtrArray.IsEmpty() == false)
	{
		PHONE_NUMBERS* pPhoneNumberInArray = oPhoneNumbersPtrArray.GetAt(0);
		if (pPhoneNumberInArray == NULL)
		{
			oPhoneNumbersPtrArray.RemoveAt(0);
			continue;
		}

		long lKeyID = pPhoneNumberInArray->lID;
		oPhoneNumbersMapByID.SetAt(lKeyID, pPhoneNumberInArray);
		oPhoneNumbersPtrArray.RemoveAt(0);
	}
	return true;
}

bool CPersonsDocument::UpdatePersonWhereID(long lID, PERSONS& recPerson)
{

	PERSONS* pPersonInMap;

	BOOL bFoundInMap = m_oMapPersonsByID.Lookup(lID, pPersonInMap);
	if (bFoundInMap)
	{
		recPerson.lUpdateCounter = pPersonInMap->lUpdateCounter;
	}

	bool bResult = CPersonsData::UpdatePersonWhereID(lID, recPerson);
	if (!bResult)
		return false;
	
* pPersonInMap = recPerson;
UpdateAllViews(NULL, OnEdit, reinterpret_cast<CObject*>(&recPerson));
return true;
}

bool CPersonsDocument::UpdatePhoneNumberWhereID(long lPhoneNumberID, PHONE_NUMBERS& recPhoneNumber)
{
	return CPersonsData::UpdatePhoneNumberWhereID(lPhoneNumberID, recPhoneNumber);
}

bool CPersonsDocument::DeletePersonWhereID(long lPersonID)
{
	PERSONS* pPersonInMap;
	bool bResult = CPersonsData::DeletePersonWhereID(lPersonID);
	bool bFoundInMap = m_oMapPersonsByID.Lookup(lPersonID, pPersonInMap);

	if (bResult && bFoundInMap)
	{
		PERSONS recPersonToRemove;
		if (pPersonInMap != NULL)
		{
			recPersonToRemove = *pPersonInMap;
			delete pPersonInMap;
		}
		else
		{
			recPersonToRemove.lID = lPersonID;
		}
		m_oMapPersonsByID.RemoveKey(lPersonID);

		UpdateAllViews(NULL, OnDelete, reinterpret_cast<CObject*>(&recPersonToRemove));
	}
	return bResult;
}

bool CPersonsDocument::DeletePhoneNumberWhereID(long lPhoneNumberID)
{
	return CPersonsData::DeletePhoneNumberWhereID(lPhoneNumberID);
}

bool CPersonsDocument::InsertPerson(PERSONS& recPerson)
{
	bool bResult = CPersonsData::InsertPerson(recPerson);
	if (bResult)
	{
		PERSONS* pPerson;
		if (m_oMapPersonsByID.Lookup(recPerson.lID, pPerson) && pPerson != NULL)
		{
			*pPerson = recPerson;
		}
		else
		{
			m_oMapPersonsByID.SetAt(recPerson.lID, new PERSONS(recPerson));
		}
		UpdateAllViews(NULL, OnInsert, reinterpret_cast<CObject*>(&recPerson));
	}
	return bResult;
}

bool CPersonsDocument::InsertPhoneNumber(PHONE_NUMBERS& recPhoneNumber)
{
	return CPersonsData::InsertPhoneNumber(recPhoneNumber);
}

bool CPersonsDocument::EditPersonInfo(PersonInfo& oPersonInfo)
{
	bool bResult = CPersonsData::EditPersonInfo(oPersonInfo);

	return bResult;
}

bool CPersonsDocument::InsertPair(PersonInfo& oPersonInfo)
{
	bool bResult = CPersonsData::InsertPair(oPersonInfo);
	if (bResult == false)
	{
		return false;
	}

	PERSONS& recPersonToInsert = oPersonInfo.recPerson;
	m_oMapPersonsByID.SetAt(recPersonToInsert.lID, new PERSONS(recPersonToInsert));

	UpdateAllViews(NULL, OnInsert, reinterpret_cast<CObject*>(&recPersonToInsert));
	return true;
}

bool CPersonsDocument::SelectCityWhereIDFromDB(long lID, CITIES& recCity)
{
	return CCitiesData::SelectWhereID(lID, recCity);
}

bool CPersonsDocument::SelectCityWhereIDFromMap(long lIDToLookUp, CITIES*& pCity) const
{
	POSITION oMainMapItter = m_oMapCitiesByNameID.GetStartPosition();
	while (oMainMapItter != NULL)
	{
		CString strMainKeyCityName;
		CMapCitiesByID* pSubMap;
		m_oMapCitiesByNameID.GetNextAssoc(oMainMapItter, strMainKeyCityName, pSubMap);

		if (pSubMap->Lookup(lIDToLookUp, pCity))
		{
			return true;
		}
	}
	return false;
}

bool CPersonsDocument::SelectCitiesWhereNameFromDB(CString& strCityName, CCitiesArray& oCitiesArray)
{
	return CCitiesData::SelectCitiesWhereName(strCityName, oCitiesArray);
}

bool CPersonsDocument::SelectCitiesWhereNameFromMap(CString& strCityName, CMapCitiesByID* pMapCitiesByID) const
{
	return m_oMapCitiesByNameID.Lookup(strCityName, pMapCitiesByID);
}

bool CPersonsDocument::SelectPhoneTypeWhereIDFromDB(long lID, PHONE_TYPES& recPhoneType)
{
	return CPhoneTypesData::SelectWhereID(lID, recPhoneType);
}

bool CPersonsDocument::SelectPhoneTypeWhereIDFromMap(long lID, PHONE_TYPES*& recPhoneType) const
{
	return m_oMapPhoneTypesByID.Lookup(lID, recPhoneType);
}

// Getters
const CMapPersonsByID& CPersonsDocument::GetPersonsMapByID() const
{
	return m_oMapPersonsByID;
}

const CMapPhoneTypesByID& CPersonsDocument::GetPhoneTypesMapByID() const
{
	return m_oMapPhoneTypesByID;
}

const CMapCitiesByNameMapID& CPersonsDocument::GetCitiesMapByName() const
{
	return m_oMapCitiesByNameID;
}

// Overrides
// ----------------
BOOL CPersonsDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	SetTitle(_T("Потребители"));
	bool bResult;

	bResult = LoadPersons() && LoadCities() && LoadPhoneTypes();

	return bResult;
}

void CPersonsDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}

#ifdef _DEBUG
void CPersonsDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CPersonsDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

