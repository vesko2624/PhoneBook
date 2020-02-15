#pragma once
#include "PhoneBook.h"
#include "PersonsData.h"
#include "CitiesData.h"
#include "PhoneTypesData.h"


/////////////////////////////////////////////////////////////////////////////
// CCitiesDocument

class CPersonsDocument 
	: public CDocument
	, private CPersonsData
	, private CCitiesData
	, private CPhoneTypesData
{
	DECLARE_DYNCREATE(CPersonsDocument)
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------

// Constructor / Destructor
// ----------------
public:
	CPersonsDocument();
	virtual ~CPersonsDocument();


// Methods
// ----------------
public:
	bool LoadPersons();
	void RemovePersonFromMap(long lID);

	bool LoadCities();
	void RemoveCityFromMap(long lID);
	
	bool LoadPhoneTypes();

	bool SelectPersonWhereID(long lID, PERSONS& recPerson);
	bool SelectPhoneNumberWhereID(long lPhoneNumberID, PHONE_NUMBERS& recPhoneNumber);

	bool SelectAllPersons(CPersonsArray& oPersonsArray);
	bool SelectAllPhoneNumers(long lPersonID, CMapPhoneNumbersByID& oPhoneNumbersMapByID);

	bool UpdatePersonWhereID(long lPersonID, PERSONS& recPerson);
	bool UpdatePhoneNumberWhereID(long lPhoneNumberID, PHONE_NUMBERS& recPhoneNumber);

	bool DeletePersonWhereID(long lPersonID);
	bool DeletePhoneNumberWhereID(long lPhoneNumberID);

	bool InsertPerson(PERSONS& recPerson);
	bool InsertPhoneNumber(PHONE_NUMBERS& recPhoneNumber);

	bool EditPersonInfo(PersonInfo& oPersonInfo);
	bool InsertPair(PersonInfo& oPersonInfo);

	bool SelectCityWhereIDFromDB(long lID, CITIES& recCity);
	bool SelectCityWhereIDFromMap(long lIDToLookUp, CITIES*& pCity) const;

	bool SelectCitiesWhereNameFromDB(CString& strCityName, CCitiesArray& oCitiesArray);
	bool SelectCitiesWhereNameFromMap(CString& strCityName, CMapCitiesByID* pMapCitiesByID) const;

	bool SelectPhoneTypeWhereIDFromDB(long lID, PHONE_TYPES& recPhoneType);
	bool SelectPhoneTypeWhereIDFromMap(long lID, PHONE_TYPES*& recPhoneType) const;

	// Getters
	const CMapPersonsByID& GetPersonsMapByID() const;
	const CMapPhoneTypesByID& GetPhoneTypesMapByID() const;
	const CMapCitiesByNameMapID& GetCitiesMapByName() const;

// Overrides
// ----------------
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Members
// ----------------
private:
	CMapPersonsByID m_oMapPersonsByID;
	CMapPhoneTypesByID m_oMapPhoneTypesByID;
	CMapCitiesByNameMapID m_oMapCitiesByNameID;
};