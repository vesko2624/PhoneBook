#pragma once
#include "../includes/Structures.h"

/////////////////////////////////////////////////////////////////////////////
// CDatabaseConnectionManager

//  <summary>
//  Class for database connection managing
//  contains Init PropSet and DataSource
//  </summary>
class CDatabaseConnectionManager
{
// Constants
// ----------------


// Constructor / Destructor
// ----------------
public:
    CDatabaseConnectionManager();
    ~CDatabaseConnectionManager();
    

// Methods
// ----------------
public:
    static void Connect();
    static void Disconnect();

    static void OpenSession(CSession&);
    static void CloseSession(CSession&);

    static const CDataSource& GetDataSource();
    static const bool& GetConnectionStatus();


// Overrides
// ----------------


// Members
// ----------------
private:
    static CDBPropSet m_oDBPropSet;
    static CDataSource m_oDataSource;

    static bool m_bConnectionStatus;
};
