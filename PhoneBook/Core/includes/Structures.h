#pragma once
#include <afxtempl.h>
#include <atlbase.h>
#include <atldbcli.h>
#include <atldbsch.h>

#include "../includes/PtrArrayManager.h"

//  <summary>
//  namespace за големината на колоните ( дължина на стринговете ) в таблицата
//  </summary>
namespace Length
{
    const short CITIES_NAME = 65;
    const short CITIES_REGION = 65;
    const short PERSONS_FIRST_NAME = 33;
    const short PERSONS_MIDDLE_NAME = 33;
    const short PERSONS_LAST_NAME = 33;
    const short PERSONS_IDENTIFICATION_NUMBER = 17;
    const short PERSONS_ADDRESS = 65;
    const short PHONE_TYPES_TYPE = 9;
    const short PERSONS_NUMBER = 17;
}

// <summary>
// Дискова структура за таблица CITIES
// </summary>
struct CITIES
{
    long lID;
    long lUpdateCounter;
    TCHAR szName[Length::CITIES_NAME];
    TCHAR szRegion[Length::CITIES_REGION];

    CITIES()
    {
        SecureZeroMemory(this, sizeof(*this));
    }
    CITIES(long lID, long lUpdateCounter, const CString& strName, const CString& strRegion)
    {
        SecureZeroMemory(this, sizeof(*this));
        this->lID = lID;
        this->lUpdateCounter = lUpdateCounter;
        wcsncpy_s(this->szName, (LPCTSTR)strName, Length::CITIES_NAME);
        wcsncpy_s(this->szRegion, (LPCTSTR)strRegion, Length::CITIES_REGION);
    }
};

//  <summary>
//  Дискова структура за таблица PERSONS
//  </summary>
struct PERSONS
{
    long lID;
    long lUpdateCounter;
    TCHAR szFirstName[Length::PERSONS_FIRST_NAME];
    TCHAR szMiddleName[Length::PERSONS_MIDDLE_NAME];
    TCHAR szLastName[Length::PERSONS_LAST_NAME];
    TCHAR szIdentificationNumber[Length::PERSONS_IDENTIFICATION_NUMBER];
    long lCityId;
    TCHAR szAddress[Length::PERSONS_ADDRESS];
    PERSONS()
    {
        SecureZeroMemory(this, sizeof(*this));
    }
    PERSONS(long lID, long lUpdateCounter, const CString& strFirstName,
            const CString& strMiddleName, const CString& strLastName,
            const CString& strIdentificationNumber, long lCityId,
            const CString& strAddress)
    {
        this->lID = lID;
        this->lUpdateCounter = lUpdateCounter;
        wcsncpy_s(this->szFirstName, (LPCTSTR)strFirstName, Length::PERSONS_FIRST_NAME);
        wcsncpy_s(this->szMiddleName, (LPCTSTR)strMiddleName, Length::PERSONS_MIDDLE_NAME);
        wcsncpy_s(this->szLastName, (LPCTSTR)strLastName, Length::PERSONS_LAST_NAME);
        wcsncpy_s(this->szIdentificationNumber, (LPCTSTR)strIdentificationNumber, Length::PERSONS_IDENTIFICATION_NUMBER);
        this->lCityId = lCityId;
        wcsncpy_s(this->szAddress, (LPCTSTR)strAddress, Length::PERSONS_ADDRESS);
    }
};

//  <summary>
//  Дискова структура за таблица PHONE_TYPES
//  </summary>
struct PHONE_TYPES
{
    long lID;
    long lUpdateCounter;
    TCHAR szType[Length::PHONE_TYPES_TYPE];
    PHONE_TYPES()
    {
        SecureZeroMemory(this, sizeof(*this));
    }
    PHONE_TYPES(long lID, long lUpdateCounter, const CString& strType)
    {
        this->lID = lID;
        this->lUpdateCounter = lUpdateCounter;
        wcsncpy_s(this->szType, (LPCTSTR)strType, Length::PHONE_TYPES_TYPE);
    }
};

//  <summary>
//  Дискова структура за таблица PHONE_NUMERS
//  </summary>
struct PHONE_NUMBERS
{
    long lID;
    long lUpdateCounter;
    long lPersonId;
    long lPhoneTypesId;
    TCHAR szNumber[Length::PERSONS_NUMBER];
    PHONE_NUMBERS()
    {
        SecureZeroMemory(this, sizeof(*this));
    }
    PHONE_NUMBERS(long lID, long lUpdateCounter, long lPersonID,
                  long lPhoneTypesId, const CString& strNumber)
    {
        this->lID = lID;
        this->lUpdateCounter = lUpdateCounter;
        this->lPersonId = lPersonID;
        this->lPhoneTypesId = lPhoneTypesId;
        wcsncpy_s(this->szNumber, (LPCTSTR)strNumber, Length::PERSONS_NUMBER);
    }
};
typedef CPtrArrayManager<CITIES*> CCitiesArray;
typedef CPtrArrayManager<PERSONS*> CPersonsArray;
typedef CPtrArrayManager<PHONE_TYPES*> CPhoneTypesArray;
typedef CPtrArrayManager<PHONE_NUMBERS*> CPhoneNumbersArray;