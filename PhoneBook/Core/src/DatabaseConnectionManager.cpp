#include "../includes/DatabaseConnectionManager.h"

/////////////////////////////////////////////////////////////////////////////
// CDatabaseConnectionManager

// Static members definition
// ----------------
//  <summary>
//  Init Prop Set
//  </summary>
CDBPropSet CDatabaseConnectionManager::m_oDBPropSet(DBPROPSET_DBINIT);
CDataSource CDatabaseConnectionManager::m_oDataSource;
bool CDatabaseConnectionManager::m_bConnectionStatus = false;


// Constructor / Destructor
// ----------------
CDatabaseConnectionManager::CDatabaseConnectionManager()
{
    m_oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("DESKTOP-NMOS3K3"));	// сървър
    m_oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("PhoneBook"));	// база данни
    m_oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));	// база данни
    m_oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
    m_oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
    m_oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));
    Connect();
}

CDatabaseConnectionManager::~CDatabaseConnectionManager()
{
    Disconnect();
}


// Methods
// ----------------
//  <summary>
//  Connects to database
//  </summary>
void CDatabaseConnectionManager::Connect()
{
    if (m_bConnectionStatus == TRUE)
    {
        return;
    }
    HRESULT hResult = CoInitialize(NULL);

    hResult = m_oDataSource.Open(_T("SQLOLEDB.1"), &m_oDBPropSet);
    if (FAILED(hResult))
    {
        CString strError;
        strError.Format(_T("Unable to connect to SQL Server database. Error: %ld"), hResult);
        throw (LPCTSTR)strError;
    }
    m_bConnectionStatus = TRUE;
}

//  <summary>
//  Disconnects from database
//  </summary>
void CDatabaseConnectionManager::Disconnect()
{
    if (m_bConnectionStatus == TRUE)
    {
        m_oDataSource.Close();
        m_bConnectionStatus = FALSE;
    }
}

//  <summary>
//  Open a session
//  </summary>
//  <param name="oSession"> Reference to the session that will be opened </param>
void CDatabaseConnectionManager::OpenSession(CSession& oSession)
{
    if (m_bConnectionStatus == FALSE)
    {
        throw "No connection established";
    }
    HRESULT hResult = CoInitialize(NULL);
    hResult = oSession.Open(m_oDataSource);
    if (FAILED(hResult))
    {
        CString strError;
        strError.Format(_T("Unable to open session. Error: %d"), hResult);
        Disconnect();
        throw (LPCTSTR)strError;
    }
}

//  <summary>
//  Close a session
//  </summary>
//  <param name="oSession"> Reference to the session that will be closed </param>
void CDatabaseConnectionManager::CloseSession(CSession& oSession)
{
    oSession.Close();
}

//  <summary>
//  Data Source Getter
//  </summary>
const CDataSource& CDatabaseConnectionManager::GetDataSource()
{
    return m_oDataSource;
}

//  <summary>
//  Connection Status Getter
//  </summary>
const bool& CDatabaseConnectionManager::GetConnectionStatus()
{
    return m_bConnectionStatus;
}


// Overrides
// ----------------


