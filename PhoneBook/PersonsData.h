#pragma once
#include "pch.h" // For phonebook.h
#include "PhoneBook.h" // For maps
#include "Core/includes/PersonsTable.h"
#include "Core/includes/PhoneNumbersTable.h"
#include "Core/includes/Structures.h"



enum class RecordActions
{
	DB_TO_EDIT,
	DB_TO_INSERT,
	DB_TO_DELETE,
	DB_INSERT_ERROR,
	RECORD_ACTIONS_SIZE = DB_INSERT_ERROR
};

//	<summary>
//	Структура обединяваща информацията за един потребител
//	</summary>
class PersonInfo {
public:
	PERSONS recPerson;
	CMapPhoneNumbersByID oPhoneNumbersMapByID;
	CArray<CMapPhoneNumbersByID> oHintArray;
	PersonInfo() 
	{
		INT_PTR nHintArraySize = (INT_PTR)RecordActions::RECORD_ACTIONS_SIZE + 1;
		oHintArray.SetSize(nHintArraySize);
	}
	~PersonInfo()
	{
		for (INT_PTR i = 0; i < oHintArray.GetCount(); ++i)
		{
			oHintArray.GetAt(i).RemoveAll();
		}
	}
};

/////////////////////////////////////////////////////////////////////////////
// CPersonsData

class CPersonsData : private CPersonsTable, private CPhoneNumbersTable
{
// Constants
// ----------------


// Constructor / Destructor
// ----------------
public:
	CPersonsData();
	~CPersonsData();


// Methods
// ----------------
public:
	bool SelectPersonWhereID(long lPersonID, PERSONS& recPerson);
	bool SelectPhoneNumberWhereID(long lPhoneNumberID, PHONE_NUMBERS& recPhoneNumber);

	bool SelectAllPersons(CPersonsArray& oPersonsArray);
	bool SelectAllPhoneNumers(long lPersonID, CPhoneNumbersArray& oPhoneNumbersArray);

	bool UpdatePersonWhereID(long lPersonID, PERSONS& recPerson);
	bool UpdatePhoneNumberWhereID(long lPhoneNumberID, PHONE_NUMBERS& recPhoneNumber);
	
	bool DeletePersonWhereID(long lPersonID);
	bool DeletePhoneNumberWhereID(long lPhoneNumberID);

	bool InsertPerson(PERSONS& recPerson);
	bool InsertPhoneNumber(PHONE_NUMBERS& recPhoneNumber);

	bool InsertMultiplePhoneNumbers(PersonInfo& oPersonInfo);
	bool InsertPair(PersonInfo& oPersonInfo);

	bool DeleteMultiplePhoneNumbers(PersonInfo& oPersonInfo);
	bool EditMultiplePhoneNumbers(PersonInfo& oPersonInfo);

	bool EditPersonInfo(PersonInfo& oPersonInfo);
// Overrides
// ----------------


// Members
// ----------------
private:
	CSession m_oCommonSession;
};
