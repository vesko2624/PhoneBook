#pragma once
#include "QueryExecutor.h"
/////////////////////////////////////////////////////////////////////////////
// CTableStatusManager

//  <summary>
//  Class meant to close table on scope exit
//  When passed NULL the table is preserved open
//  </sumamry>
template<typename TABLECLASS>
class CTableStatusManager
{
// Constants
// ----------------


// Constructor / Destructor
// ----------------
public:
    CTableStatusManager(CQueryExecutor<TABLECLASS>* pQueryExecutor)
        : m_pQueryExecutor(pQueryExecutor)
    {
    }

    ~CTableStatusManager()
    {
        if (m_pQueryExecutor != NULL)
        {
            m_pQueryExecutor->Close();
        }
    }


// Methods
// ----------------


// Overrides
// ----------------


// Members
// ----------------
private:
    CQueryExecutor<TABLECLASS>* m_pQueryExecutor;
};