#pragma once
#include "../includes/DatabaseConnectionManager.h"
#include "../includes/QueryExecutor.h"
#include "../includes/TableStatusManager.h"

enum TableStateOnExit
{
	PreserveOpen = true,
	CloseOnExit = false
};
/////////////////////////////////////////////////////////////////////////////
// CDatabaseTables

//	<summary>
//	Class combining common
//	functionality of all tables
//	(SelectAll, SelectWhereID etc...)
//	</summary>
template<class ACCESSOR, class STRUCTURE>
class CDatabaseTables : protected CCommand<CAccessor<ACCESSOR>>
{
// Constants
// ----------------
private:
	const CString m_strTableName;
	typedef CPtrArrayManager<STRUCTURE*> CStructArray;
	typedef CDatabaseTables<ACCESSOR, STRUCTURE> DatabaseTableClass;
	friend CQueryExecutor<CDatabaseTables<ACCESSOR, STRUCTURE>>;


// Constructor / Destructor
// ----------------
public:
	//	<summary>
	//	Създава сесия и Query Executor
	//	</summary>
	//	<param name="strTableName"> Име на таблица </param>
	CDatabaseTables(CString strTableName, CSession* pCommonSession = NULL)
		: m_strTableName(strTableName)
		, m_bHasCommonSession(pCommonSession == NULL)
		, m_oQueryExecutor((m_bHasCommonSession)? m_oSession : *pCommonSession, *this)
	{
		if (m_bHasCommonSession)
		{
			CDatabaseConnectionManager::OpenSession(m_oSession);
		}
	}

	~CDatabaseTables()
	{
		if (m_bHasCommonSession)
		{
			CDatabaseConnectionManager::CloseSession(m_oSession);
		}
	}


// Methods
// ----------------
public:
	//	<summary>
	//	Взема всички записи от таблица strTableName
	//	</summary>
	//	<param name="oRecordsArray"> Масив в който съхраняваме записите </param>
	virtual bool SelectAll(CStructArray& oRecordsPtrArray, bool bPreserveOpen = CloseOnExit)
	{
		CTableStatusManager<DatabaseTableClass> oTableStatusManager((bPreserveOpen) ? NULL : &m_oQueryExecutor);

		CString strQuery;
		HRESULT hResult = S_FALSE;
		strQuery.Format(_T("SELECT * FROM %s"), (LPCTSTR)m_strTableName);
		if (m_oQueryExecutor.Open(strQuery) == FALSE)
		{
			return FALSE;
		}

		do
		{
			hResult = MoveNext();
			switch (hResult)
			{
				case S_OK:
					oRecordsPtrArray.Add(new STRUCTURE(m_recTableRow));
					break;

				case DB_S_ENDOFROWSET:
					break;

				default:
				{
					CString strError;
					strError.Format(_T("Error. MoveNext returned %ld"), hResult);
					throw (LPCTSTR)strError;
				}
			}
		} while (hResult == S_OK);
		return true;
	}

	//	<summary>
	//	Взема всички записи от таблица strTableName със ID = lID
	//	</summary>
	//	<param name="recTableRecord"> Приема стойност на намерения запис </param>
	virtual bool SelectWhereID(const long lID, STRUCTURE& recTableRecord, bool bPreserveOpen = CloseOnExit)
	{
		CTableStatusManager<DatabaseTableClass> oTableStatusManager((bPreserveOpen) ? NULL : &m_oQueryExecutor);
		HRESULT hResult = S_FALSE;
		CString strQuery;

		strQuery.Format(_T("SELECT * FROM %s WHERE ID = %d"), (LPCTSTR)m_strTableName, lID);
		if (m_oQueryExecutor.Open(strQuery) == false)
		{
			return FALSE;
		}

		hResult = MoveNext();
		if (hResult == DB_S_ENDOFROWSET) 
			return false;
		else
		{
			if (hResult != S_OK)
			{
				CString strError;
				strError.Format(_T("Error. MoveNext returned %ld"), hResult);
				throw (LPCTSTR)strError;
			}
		}

		recTableRecord = m_recTableRow;
		return true;
	}

	//	<summary>
	//	UPDATE на запис със ID = lID в таблица "strTableName"
	//	</summary>
	//	<param name="recTableRecord"> Новата стойност на записа </param>
	virtual bool UpdateWhereID(const long lID , STRUCTURE& recTableRecord, bool bPreserveOpen = CloseOnExit)
	{
		CTableStatusManager<DatabaseTableClass> oTableStatusManager((bPreserveOpen)? NULL : &m_oQueryExecutor);
		HRESULT hResult = S_FALSE;
		STRUCTURE recCurrentDbRecord;
		if (SelectWhereID(lID, recCurrentDbRecord, PreserveOpen) == false)
		{
			return false;
		}
		if (recCurrentDbRecord.lUpdateCounter != recTableRecord.lUpdateCounter)
		{
			return false;
		}

		m_recTableRow = recTableRecord;
		m_recTableRow.lUpdateCounter = recCurrentDbRecord.lUpdateCounter + 1;

		recTableRecord.lUpdateCounter = m_recTableRow.lUpdateCounter;

		hResult = SetData(1);
		if (hResult != S_OK)
		{
			return false;
		}
		return true;
	}

	//	<summary>
	//	INSERT на запис в таблица "strTableName"
	//	</summary>
	//	<param name="recTableRecord"> Запис за INSERT </param>
	virtual bool InsertRecord(STRUCTURE& recTableRecord, bool bPreserveOpen = CloseOnExit)
	{
		CTableStatusManager<DatabaseTableClass> oTableStatusManager((bPreserveOpen) ? NULL : &m_oQueryExecutor);
		HRESULT hResult = S_FALSE;
		CString strQuery;
		strQuery.Format(_T("SELECT TOP(0)* FROM %s"), (LPCTSTR)m_strTableName);
		if (m_oQueryExecutor.Open(strQuery) == false)
		{
			return false;
		}

		MoveLast();
		m_recTableRow = recTableRecord;

		hResult = Insert(1);
		if (hResult != S_OK)
		{
			return false;
		}
		hResult = MoveNext();
		if (hResult != S_OK)
		{
			return false;
		}
		recTableRecord.lID = m_recTableRow.lID;

		return true;
	}

	//	<summary>
	//	DELETE на запис по ID
	//	</summary>
	//	<param name="lID"> id на запис за триене </param>
	virtual bool DeleteWhereID(const long lID, bool bPreserveOpen = CloseOnExit)
	{
		CTableStatusManager<DatabaseTableClass> oTableStatusManager((bPreserveOpen) ? NULL : &m_oQueryExecutor);
		HRESULT hResult = S_FALSE;
		CString strQuery;

		if (SelectWhereID(lID, STRUCTURE(), PreserveOpen) == false)
		{
			return false;
		}

		hResult = Delete();
		if (hResult != S_OK)
		{
			return false;
		}
		return true;
	}

// Overrides
// ----------------


// Members
// ----------------
protected:
	bool Dummy = true;
	bool m_bHasCommonSession;
	CSession m_oSession;
	CQueryExecutor<CDatabaseTables> m_oQueryExecutor;
};