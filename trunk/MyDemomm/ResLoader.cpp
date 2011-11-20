#include <assert.h>
#include <algorithm>

#include "ResLoader.h"
#include "Res.h"
#include "ResMngr.h"
#include "process.h"
#include "Debug.h"


const DWORD RES_THREAD_CLOSE_TIMEOUT = 5000;
const DWORD RES_REQUEST_TIMEOUT = 1000;

///////////////////////////////////////////////////////////////////////////////
CResLoader* CResLoader::	GetTheOne()
{
	static CResLoader TheOne;
	return &TheOne;
}

///////////////////////////////////////////////////////////////////////////////
CResLoader::CResLoader() :	m_hThread( NULL ),
							m_hEventRequest( NULL ),
							m_bThreadRunning( false )
{
	m_strPath.clear();
}

///////////////////////////////////////////////////////////////////////////////
bool CResLoader::Init()
{
	//初始化关键区和事件
	::InitializeCriticalSection( &m_CriticalSection );

	m_hEventRequest = ::CreateEvent( NULL, TRUE, FALSE, NULL );
	if ( NULL == m_hEventRequest )
	{
		return false;
	}
	
	//开启资源加载线程
	m_bThreadRunning = true;
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, CResLoader::ResThreadProc, this, 0, NULL );
	if ( NULL == m_hThread )
	{
		return false;
	}

	return true;
}

void CResLoader::SetPath( const char* pInPath )
{
	assert( pInPath != NULL );
	if( pInPath != NULL )
	{
		m_strPath = pInPath;
	}
}

///////////////////////////////////////////////////////////////////////////////
void CResLoader::Shutdown()
{
	//关闭资源加载线程
	if ( m_hThread != NULL )
	{
		m_bThreadRunning = false;
		if ( WAIT_TIMEOUT == WaitForSingleObject( m_hThread, RES_THREAD_CLOSE_TIMEOUT ) )
		{	//没响应，强行关闭
			TerminateThread( m_hThread, 0 );
		}
		CloseHandle( m_hThread );
		m_hThread = NULL;
	}

	::DeleteCriticalSection( &m_CriticalSection );

	if ( m_hEventRequest != NULL )
	{
		CloseHandle( m_hEventRequest );
		m_hEventRequest = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
void CResLoader::AddRequest( CRes* pRes )
{
	assert( pRes != NULL );
	assert( !pRes->IsValid() );

	//////////////////////////////////////////////////////////
	//关键区
	::EnterCriticalSection( &m_CriticalSection );

	m_listRequest.push_back( pRes );

	::LeaveCriticalSection( &m_CriticalSection );
	//////////////////////////////////////////////////////////

	SetEvent( m_hEventRequest );
}

///////////////////////////////////////////////////////////////////////////////
bool CResLoader::RemRequest( CRes* pRes )
{
	assert( pRes != NULL );

	bool bRet = false;
	//////////////////////////////////////////////////////////
	//关键区
	::EnterCriticalSection( &m_CriticalSection );

	RequestList_t::iterator iterFound = find( m_listRequest.begin(), m_listRequest.end(), pRes );
	if ( iterFound != m_listRequest.end() )
	{
		m_listRequest.erase( iterFound );
		bRet = true;
	}

	::LeaveCriticalSection( &m_CriticalSection );
	//////////////////////////////////////////////////////////

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////
CRes* CResLoader::GetRequest()
{
	CRes* pRes = NULL;

	//////////////////////////////////////////////////////////
	//关键区
	::EnterCriticalSection( &m_CriticalSection );
	
	if ( m_listRequest.begin() != m_listRequest.end() )
	{
		pRes = m_listRequest.front();
		m_listRequest.pop_front();
	}
	
	::LeaveCriticalSection( &m_CriticalSection );
	//////////////////////////////////////////////////////////

	return pRes;
}

///////////////////////////////////////////////////////////////////////////////
unsigned int WINAPI CResLoader::ResThreadProc( LPVOID pParam )
{
	CResLoader* pLoader = static_cast< CResLoader* >( pParam );

	
	assert( pLoader != NULL );
	assert( pLoader->m_hEventRequest != NULL );

	while ( pLoader->m_bThreadRunning )
	{
		if ( WAIT_TIMEOUT == ::WaitForSingleObject( pLoader->m_hEventRequest,
													RES_REQUEST_TIMEOUT ) )
		{
			continue;
		}

		ResetEvent( pLoader->m_hEventRequest );

		CRes *pRes = NULL;
		while ( ( pRes = pLoader->GetRequest() ) != NULL )
		{
			CResMngr *pMngr = pRes->GetMngr();
			assert( pMngr != NULL );
			if ( pMngr->BuildRes( pRes ) )
			{
				pRes->SetState( RES_STATE_VALID );
			}
			else
			{
				pRes->SetState( RES_STATE_LOADFAILED );
				assert(false);
				ASSERTEX1( false, "Failed to build resource in resource thread[%s].",
									pRes->GetName().c_str() );
			}
		}

	}

	return 0;
}

