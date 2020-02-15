#pragma once

/////////////////////////////////////////////////////////////////////////////
// CQueryExecutor

//  <summary>
//  Class realizing a Query execution on
//  given table, creates a unique session
//  for each instance
//  </summary>
template<typename TABLE>
class CQueryExecutor
{
// Constants
// ----------------


// Constructor / Destructor
// ----------------
public:
    CQueryExecutor(CSession& oSession,
        TABLE& oTable, CDBPropSet* pUpdateDBPropSet = NULL) :
        m_oSession(oSession),
        m_oTable(oTable)
    {
        oTable.Dummy = false;
        if (pUpdateDBPropSet == NULL)
        {
            m_oUpdateDBPropSet = CDBPropSet(DBPROPSET_ROWSET);
            m_oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
            m_oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
            m_oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
            m_oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
        }
        else
        {
            m_oUpdateDBPropSet = *pUpdateDBPropSet;
        }
    }

    ~CQueryExecutor()
    {
        Close();
    }


// Methods
// ----------------
public:
    bool Open(const CString& strQuery)
    {
        Close();
        HRESULT hResult = S_FALSE;
        hResult = m_oTable.Open(m_oSession, strQuery, &m_oUpdateDBPropSet);
        if (FAILED(hResult))
        {
            wprintf(_T("Error executing query. Error: %ld. Query: %s"), hResult, (LPCTSTR)strQuery);
            return FALSE;
        }
        return TRUE;
    }

    void Close()
    {
        m_oTable.Close();
    }


// Overrides
// ----------------


// Members
// ----------------
private:
    CDBPropSet m_oUpdateDBPropSet;

    CSession& m_oSession;
    TABLE& m_oTable;
};