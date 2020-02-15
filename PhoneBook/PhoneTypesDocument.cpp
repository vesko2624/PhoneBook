#include "pch.h"
#include "PhoneTypesDocument.h"
#include "Core/includes/Structures.h"
#include "Core/includes/PhoneTypesTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesDocument

IMPLEMENT_DYNCREATE(CPhoneTypesDocument, CDocument)

BEGIN_MESSAGE_MAP(CPhoneTypesDocument, CDocument)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------
CPhoneTypesDocument::CPhoneTypesDocument()
{

}

CPhoneTypesDocument::~CPhoneTypesDocument()
{
}


// Methods
// ----------------
bool CPhoneTypesDocument::LoadPhoneTypes()
{
	m_oPhoneTypesPtrArray.ClearArray();

	bool bResult;
	bResult = CPhoneTypesData::SelectAll(m_oPhoneTypesPtrArray);
	if (bResult == false)
	{
		return false;
	}

	return true;
}
void CPhoneTypesDocument::RemovePhoneTypeFromArray(long lID)
{
	PHONE_TYPES recPhoneTypeHint;
	recPhoneTypeHint.lID = lID;

	for (INT_PTR i = 0; i < m_oPhoneTypesPtrArray.GetCount(); ++i)
	{
		PHONE_TYPES* pPhoneType = m_oPhoneTypesPtrArray.GetAt(i);
		if (pPhoneType == NULL)
		{
			m_oPhoneTypesPtrArray.RemoveAt(i);
			continue;
		}
		if (pPhoneType->lID == lID)
		{
			recPhoneTypeHint = *pPhoneType;
			delete pPhoneType;
			m_oPhoneTypesPtrArray.RemoveAt(i);
			break;
		}
	}

	UpdateAllViews(NULL, OnDelete, reinterpret_cast<CObject*>(&recPhoneTypeHint));
}

bool CPhoneTypesDocument::SelectWhereIDFromArray(long lID, PHONE_TYPES& recPhoneType)
{
	for (INT_PTR i = 0; i < m_oPhoneTypesPtrArray.GetCount(); ++i)
	{
		PHONE_TYPES* pPhoneType = m_oPhoneTypesPtrArray.GetAt(i);
		if (pPhoneType == NULL)
		{
			m_oPhoneTypesPtrArray.RemoveAt(i);
			continue;
		}
		if (pPhoneType->lID == lID)
		{
			recPhoneType = *pPhoneType;
			return true;
		}
	}

	return false;
}

bool CPhoneTypesDocument::UpdateWhereID(long lID, PHONE_TYPES& recPhoneType)
{
	INT_PTR nFoundIndex = -1;
	for (INT_PTR i = 0; i < m_oPhoneTypesPtrArray.GetCount(); ++i)
	{
		PHONE_TYPES* pPhoneType = m_oPhoneTypesPtrArray.GetAt(i);
		if (pPhoneType == NULL)
		{
			m_oPhoneTypesPtrArray.RemoveAt(i);
			continue;
		}
		if (pPhoneType->lID == recPhoneType.lID)
		{
			recPhoneType.lUpdateCounter = pPhoneType->lUpdateCounter;
			nFoundIndex = i;

			break;
		}
	}

	bool bResult = CPhoneTypesData::UpdateWhereID(lID, recPhoneType);
	if (!bResult)
	{
		return false;
	}

	if (nFoundIndex == -1)
	{
		m_oPhoneTypesPtrArray.Add(new PHONE_TYPES(recPhoneType));
	}
	else
	{
		PHONE_TYPES* pPhoneType = m_oPhoneTypesPtrArray.GetAt(nFoundIndex);
		*pPhoneType = recPhoneType;
	}

	UpdateAllViews(NULL, OnEdit, reinterpret_cast<CObject*>(&recPhoneType));
	return true;
}

bool CPhoneTypesDocument::DeleteWhereID(long lID)
{
	PHONE_TYPES recPhoneTypeToDeleteHint;
	recPhoneTypeToDeleteHint.lID = lID;

	bool bResult = CPhoneTypesData::DeleteWhereID(lID);
	if (bResult == false)
	{
		return false;
	}

	for (INT_PTR i = 0; i < m_oPhoneTypesPtrArray.GetCount(); ++i)
	{
		PHONE_TYPES* pPhoneType = m_oPhoneTypesPtrArray.GetAt(i);
		if (pPhoneType == NULL)
		{
			m_oPhoneTypesPtrArray.RemoveAt(i);
			continue;
		}
		if (pPhoneType->lID == lID)
		{
			recPhoneTypeToDeleteHint = *pPhoneType;
			delete pPhoneType;
			m_oPhoneTypesPtrArray.RemoveAt(i);
			break;
		}
	}
	
	UpdateAllViews(NULL, OnDelete, reinterpret_cast<CObject*>(&recPhoneTypeToDeleteHint));
	return true;
}

bool CPhoneTypesDocument::InsertPhoneType(PHONE_TYPES& recPhoneType)
{
	bool bResult = CPhoneTypesData::InsertPhoneType(recPhoneType);
	if (bResult == false)
	{
		return false;
	}
	m_oPhoneTypesPtrArray.Add(new PHONE_TYPES(recPhoneType));

	UpdateAllViews(NULL, OnInsert, reinterpret_cast<CObject*>(&recPhoneType));
	return true;
}

const CPhoneTypesArray& CPhoneTypesDocument::GetPhoneTypesArray() const
{
	return m_oPhoneTypesPtrArray;
}


// Overrides
// ----------------
BOOL CPhoneTypesDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	SetTitle(_T("Типове Телефони"));

	bool bResult = LoadPhoneTypes();
	if (bResult == false)
	{
		return FALSE;
	}

	return TRUE;
}

void CPhoneTypesDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}

#ifdef _DEBUG
void CPhoneTypesDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CPhoneTypesDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

