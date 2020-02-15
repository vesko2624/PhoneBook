#pragma once
#include "PhoneBook.h"
#include "CitiesData.h"


/////////////////////////////////////////////////////////////////////////////
// CCitiesDocument

class CCitiesDocument : public CDocument, private CCitiesData
{
	DECLARE_DYNCREATE(CCitiesDocument)
	DECLARE_MESSAGE_MAP()
// Constants
// ----------------


// Constructor / Destructor
// ----------------
public:
	CCitiesDocument();
	virtual ~CCitiesDocument();


// Methods
// ----------------
public:
	bool LoadCities();
	void RemoveCityFromMap(long lID);
	bool SelectWhereIDFromMap(long lID, CITIES& recCity);
	bool UpdateWhereID(long lID, CITIES& recCity);
	bool DeleteWhereID(long lID);
	bool InsertCity(CITIES& recCity);
	const CMapCitiesByID& GetCitiesMap() const;


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
	CMapCitiesByID m_oCitiesMapByID;
};