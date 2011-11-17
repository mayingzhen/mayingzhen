#include "public.h"

#include "ResMngr.h"
#include "Res.h"
#include "ResLoader.h"

///////////////////////////////////////////////////////////////////////////////
CResMngr::CResMngr() :	m_fTimeCur( 0.0f )
						
{
}


///////////////////////////////////////////////////////////////////////////////
CResMngr::~CResMngr()
{
	if ( m_mapRes.size() > 0 )
	{
		for ( ResMap_t::iterator iter = m_mapRes.begin();
			  iter != m_mapRes.end();
			  ++iter )
		{
			//ASSERTEX1( false, "[%s] not released.", (*iter).second->GetName()->c_str() );
			//ATLTRACE( "[%s] not released.\n", (*iter).second->GetName()->c_str() );
			assert(false && "res not released");
		}
	}

	if ( !m_listFree.empty() )
	{
		assert( false && "Some resource remain in free list while ResMngr destroyed." );
	}
}

///////////////////////////////////////////////////////////////////////////////
bool CResMngr::Find( const std::string& strName )
{
	ResMap_t::iterator iter = m_mapRes.find(strName);
	if ( m_mapRes.end() == iter ) return false;
	else return true;
}

///////////////////////////////////////////////////////////////////////////////
//作用：发送资源加载请求
//参数：1.资源名字。2.是否线程同步。3.参数
CRes* CResMngr::Query(const string &strName, bool buildNow, void* pParam)
{
	CRes *pRes = NULL;

	ResMap_t::iterator iter = m_mapRes.find(strName);
	if ( iter != m_mapRes.end() )
	{
		pRes = (*iter).second; 
		if ( pRes->IsFree() ) 
		{
			ResList_t::iterator iterFree = (ResList_t::iterator)pRes->GetIterFree();
			m_listFree.erase(iterFree);
		}
		pRes->IncRef();
		return pRes;
	}
	
	pRes = CreateRes(strName,buildNow,pParam); 	//创建新资源
	if (NULL == pRes) 
	{
		//ASSERTEX1( false, "Failed to create resource [%s].", strName.c_str() );
		return NULL;
	}

	const string& pKey = pRes->SetName( strName );
	pRes->SetMngr(this);
	pRes->IncRef();

	m_mapRes.insert( make_pair(pKey,pRes) );

	if (buildNow) //已同步加载，直接可用
	{	
		pRes->SetState( RES_STATE_VALID );
	}
	else  //给资源加载线程添加一个加载请求
	{	
		assert( ResLoader() );
		if ( ResLoader() )
			ResLoader()->AddRequest( pRes );
	}

	return pRes;
}

///////////////////////////////////////////////////////////////////////////////
void CResMngr::Release( CRes* pRes )
{
	assert( pRes != NULL );
	assert( !pRes->IsFree() );

	ResMap_t::iterator iter = FindRes( pRes );
	if ( iter == m_mapRes.end() )
	{
		return;
	}

	if ( 0 == pRes->DecRef() )
	{
		//引用计数为0，不再被使用
		bool bDestroy = false;

		if ( !pRes->IsValid() )
		{
			//资源未成功加载
			assert( ResLoader() != NULL );
			if ( pRes->IsLoadFailed() )
			{	//加载失败
				bDestroy = true;
			}
			else
			{
				//正在加载，一定是异步加载
				if ( ResLoader()->RemRequest( pRes ) )
				{	//成功删除加载请求
					bDestroy = true;
				}
			}
			//如果删除加载请求失败则说明资源加载请求正在被处理
			//这种情况下只能将资源放入Free链表，而不能直接删除
		}
		else if ( pRes->GetDelayTime() == 0.0f )
		{
			//延迟为0，立即销毁
			bDestroy = true;
		}

		if ( bDestroy )
		{
			m_mapRes.erase( iter );
			DestroyRes( pRes );
		}
		else
		{	//加入Free链表
			pRes->SetDeleteTime( m_fTimeCur + pRes->GetDelayTime() );
			ResList_t::iterator iterFree = m_listFree.insert( m_listFree.end(), iter );
			pRes->SetIterFree( iterFree );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void CResMngr::FixAll( void* pParam )
{
	for ( ResMap_t::iterator iter = m_mapRes.begin();
		  iter != m_mapRes.end(); )
	{
		CRes* pRes = (*iter).second;
		assert( pRes != NULL );

		ResMap_t::iterator iterTemp = iter;
		++iter;
		//未成功加载的资源不进行Fix
		if ( pRes->IsValid() )
		{
			bool bFree = pRes->IsFree();
			bool bRet = FixRes( pRes, bFree, pParam );
			if ( !bRet && bFree )
			{
				//从free链表删除
				ResList_t::iterator iterFree = pRes->GetIterFree();
				m_listFree.erase( iterFree );
				//从资源映射表删除
				m_mapRes.erase( iterTemp );
				DestroyRes( pRes );
			}
		}
	}//for……
}

///////////////////////////////////////////////////////////////////////////////
void CResMngr::ClearFree()
{
	for ( ResList_t::iterator iter = m_listFree.begin();
		  iter != m_listFree.end(); )
	{
		ResMap_t::iterator iterMap = *iter;
		CRes *pRes = (*iterMap).second;
		assert( pRes != NULL );
		assert( pRes->GetIterFree() == iter );
		if ( !pRes->IsValid() )
		{
			continue;
		}
		//从free链表删除
		ResList_t::iterator iterTemp = iter;
		++iter;
		m_listFree.erase( iterTemp );
		//从资源映射表删除
		m_mapRes.erase( iterMap );
		//销毁
		DestroyRes( pRes );
	}
}

void CResMngr::CLearAll()
{
	for ( ResMap_t::iterator iter = m_mapRes.begin();
		  iter != m_mapRes.end(); ++iter )
	{
		CRes* pRes = iter->second;
		DestroyRes(pRes);
	}
	m_mapRes.clear();
}

///////////////////////////////////////////////////////////////////////////////
void CResMngr::Refresh( double fTime )
{
	m_fTimeCur = fTime;

	for ( ResList_t::iterator iter = m_listFree.begin();
		  iter != m_listFree.end(); )
	{
		ResMap_t::iterator iterMap = *iter;
		CRes *pRes = (*iterMap).second;
		assert( pRes != NULL );

		if ( fTime < pRes->GetDeleteTime() || ( !pRes->IsValid() && !pRes->IsLoadFailed() ) )
		{
			++iter;
			continue;
		}
		//时间到
		//从free链表删除
		ResList_t::iterator iterTemp = iter;
		++iter;
		m_listFree.erase( iterTemp );
		//从资源映射表删除
		m_mapRes.erase( iterMap );
		//销毁
		DestroyRes( pRes );
	}
}

///////////////////////////////////////////////////////////////////////////////
//默认实现，子类如果需要可重载
bool CResMngr::BuildRes( CRes* pRes )
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////
//默认实现，子类如果需要可重载
CRes* CResMngr::DuplicateRes( const CRes* pRes, void* pParam )
{
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//默认实现，子类如果需要可重载
bool CResMngr::FixRes( CRes* pRes, bool bFree, void* pParam )
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//默认实现，子类如果需要可重载
bool CResMngr::Init()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//默认实现，子类如果需要可重载
//如果重载必须调用ClearFree()，否则可能会造成资源泄漏
void CResMngr::Shutdown()
{
	ClearFree();
	CLearAll();
}

///////////////////////////////////////////////////////////////////////////////
//从同名资源中找到第一个free的
CRes* CResMngr::FindFirstFreeRes( ResMap_t::iterator& iter, const string& strName )
{
	assert( (*iter).first == strName );

	do
	{
		CRes* pRes = (*iter).second;
		if ( pRes->IsFree() )
		{
			return pRes;
		}
		++iter;
	} while( iter != m_mapRes.end() && (*iter).first == strName );
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//找到指定资源
ResMap_t::iterator CResMngr::FindRes( CRes* pRes )
{
	assert( pRes != NULL );
	ResMap_t::iterator iter = m_mapRes.find( pRes->GetName() );
	if ( m_mapRes.end() == iter )
	{
		return iter;
	}

	do
	{
		if ( (*iter).second == pRes )
		{
			return iter;
		}
		++iter;
	} while( iter != m_mapRes.end() && (*iter).first == pRes->GetName() );

	return m_mapRes.end();
}
