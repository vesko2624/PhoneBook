#include "pch.h"
#include "CitiesDocument.h"
#include "Core/includes/Structures.h"
#include "Core/includes/CitiesTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CCitiesDocument

IMPLEMENT_DYNCREATE(CCitiesDocument, CDocument)

BEGIN_MESSAGE_MAP(CCitiesDocument, CDocument)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------
CCitiesDocument::CCitiesDocument()
{

}

CCitiesDocument::~CCitiesDocument()
{
}


// Methods
// ----------------
bool CCitiesDocument::LoadCities()
{
	CCitiesArray oCitiesPtrArray;
	bool bResult;

	bResult = CCitiesData::SelectAll(oCitiesPtrArray);
	if (bResult == false)
	{
		return false;
	}

	m_oCitiesMapByID.ClearMap();
	while (oCitiesPtrArray.IsEmpty() == false)
	{
		CITIES* pFirstCity = oCitiesPtrArray.GetAt(0);
		m_oCitiesMapByID.SetAt(pFirstCity->lID, pFirstCity);
		oCitiesPtrArray.RemoveAt(0);
	}
	return true;
}
void CCitiesDocument::RemoveCityFromMap(long lID)
{
	CITIES* pCity = NULL;
	CITIES recCityToRemoveFromView;
	if (m_oCitiesMapByID.Lookup(lID, pCity) != 0)
	{
		recCityToRemoveFromView = *pCity;

		delete pCity;
		m_oCitiesMapByID.RemoveKey(lID);
	}
	else
	{
		recCityToRemoveFromView.lID = lID;
	}

	UpdateAllViews(NULL, OnDelete, reinterpret_cast<CObject*>(&recCityToRemoveFromView));
}
bool CCitiesDocument::SelectWhereIDFromMap(long lID, CITIES& recCity)
{
	BOOL bFoundInMap;
	CITIES* pCityInMap = NULL;

	bFoundInMap = m_oCitiesMapByID.Lookup(lID, pCityInMap);
	if (bFoundInMap)
	{
		recCity = *pCityInMap;
	}

	return bFoundInMap;
}

bool CCitiesDocument::UpdateWhereID(long lID, CITIES& recCity)
{
	CITIES* pCity = NULL;

	if (m_oCitiesMapByID.Lookup(lID, pCity) == 0)
	{
		pCity = new CITIES(recCity);
		m_oCitiesMapByID.SetAt(lID, pCity);
	}
	else
	{
		recCity.lUpdateCounter = pCity->lUpdateCounter;
	}

	bool bResult = CCitiesData::UpdateWhereID(lID, recCity);
	if (!bResult)
		return false;

	*pCity = recCity;

	UpdateAllViews(NULL, OnEdit, reinterpret_cast<CObject*>(&recCity));
	return true;
}

bool CCitiesDocument::DeleteWhereID(long lID)
{
	CITIES* pCity = NULL;
	CITIES recCityToRemoveFromView;

	bool bResult = CCitiesData::DeleteWhereID(lID);
	if (bResult == false)
	{
		return false;
	}

	if (m_oCitiesMapByID.Lookup(lID, pCity) != 0)
	{
		if (pCity == NULL)
		{
			recCityToRemoveFromView.lID = lID;
		}
		else {
			recCityToRemoveFromView = *pCity;
			delete pCity;
			m_oCitiesMapByID.RemoveKey(lID);
		}
	}

	UpdateAllViews(NULL, OnDelete, reinterpret_cast<CObject*>(&recCityToRemoveFromView));
	return true;
}

bool CCitiesDocument::InsertCity(CITIES& recCity)
{
	bool bResult = CCitiesData::InsertCity(recCity);
	if (bResult == false)
	{
		return false;
	}

	CITIES* pCityInMap = NULL;
	if (m_oCitiesMapByID.Lookup(recCity.lID, pCityInMap) != 0)
	{
		*pCityInMap = recCity;
	}
	else
	{
		pCityInMap = new CITIES(recCity);
		m_oCitiesMapByID.SetAt(recCity.lID, pCityInMap);
	}

	UpdateAllViews(NULL, OnInsert, reinterpret_cast<CObject*>(pCityInMap));
	return true;
}

const CMapCitiesByID& CCitiesDocument::GetCitiesMap() const
{
	return m_oCitiesMapByID;
}

// Overrides
// ----------------
BOOL CCitiesDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	SetTitle(_T("Градове"));

	bool bResult = LoadCities();
	if (bResult == false)
	{
		return FALSE;
	}

	return TRUE; 
}

void CCitiesDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}

#ifdef _DEBUG
void CCitiesDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CCitiesDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

