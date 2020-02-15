#pragma once
#include "PhoneBook.h"
#include "PhoneTypesData.h"


/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesDocument

class CPhoneTypesDocument : public CDocument, private CPhoneTypesData
{
	DECLARE_DYNCREATE(CPhoneTypesDocument)
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------


// Constructor / Destructor
// ----------------
public:
	CPhoneTypesDocument();
	virtual ~CPhoneTypesDocument();


// Methods
// ----------------
public:
	bool LoadPhoneTypes();
	void RemovePhoneTypeFromArray(long lID);
	bool SelectWhereIDFromArray(long lID, PHONE_TYPES& recPhoneType);
	bool UpdateWhereID(long lID, PHONE_TYPES& recPhoneType);
	bool DeleteWhereID(long lID);
	bool InsertPhoneType(PHONE_TYPES& recPhoneType);
	const CPhoneTypesArray& GetPhoneTypesArray() const;


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
	CPhoneTypesArray m_oPhoneTypesPtrArray;
};