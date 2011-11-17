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
//���ã�������Դ��������
//������1.��Դ���֡�2.�Ƿ��߳�ͬ����3.����
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
	
	pRes = CreateRes(strName,buildNow,pParam); 	//��������Դ
	if (NULL == pRes) 
	{
		//ASSERTEX1( false, "Failed to create resource [%s].", strName.c_str() );
		return NULL;
	}

	const string& pKey = pRes->SetName( strName );
	pRes->SetMngr(this);
	pRes->IncRef();

	m_mapRes.insert( make_pair(pKey,pRes) );

	if (buildNow) //��ͬ�����أ�ֱ�ӿ���
	{	
		pRes->SetState( RES_STATE_VALID );
	}
	else  //����Դ�����߳����һ����������
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
		//���ü���Ϊ0�����ٱ�ʹ��
		bool bDestroy = false;

		if ( !pRes->IsValid() )
		{
			//��Դδ�ɹ�����
			assert( ResLoader() != NULL );
			if ( pRes->IsLoadFailed() )
			{	//����ʧ��
				bDestroy = true;
			}
			else
			{
				//���ڼ��أ�һ�����첽����
				if ( ResLoader()->RemRequest( pRes ) )
				{	//�ɹ�ɾ����������
					bDestroy = true;
				}
			}
			//���ɾ����������ʧ����˵����Դ�����������ڱ�����
			//���������ֻ�ܽ���Դ����Free����������ֱ��ɾ��
		}
		else if ( pRes->GetDelayTime() == 0.0f )
		{
			//�ӳ�Ϊ0����������
			bDestroy = true;
		}

		if ( bDestroy )
		{
			m_mapRes.erase( iter );
			DestroyRes( pRes );
		}
		else
		{	//����Free����
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
		//δ�ɹ����ص���Դ������Fix
		if ( pRes->IsValid() )
		{
			bool bFree = pRes->IsFree();
			bool bRet = FixRes( pRes, bFree, pParam );
			if ( !bRet && bFree )
			{
				//��free����ɾ��
				ResList_t::iterator iterFree = pRes->GetIterFree();
				m_listFree.erase( iterFree );
				//����Դӳ���ɾ��
				m_mapRes.erase( iterTemp );
				DestroyRes( pRes );
			}
		}
	}//for����
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
		//��free����ɾ��
		ResList_t::iterator iterTemp = iter;
		++iter;
		m_listFree.erase( iterTemp );
		//����Դӳ���ɾ��
		m_mapRes.erase( iterMap );
		//����
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
		//ʱ�䵽
		//��free����ɾ��
		ResList_t::iterator iterTemp = iter;
		++iter;
		m_listFree.erase( iterTemp );
		//����Դӳ���ɾ��
		m_mapRes.erase( iterMap );
		//����
		DestroyRes( pRes );
	}
}

///////////////////////////////////////////////////////////////////////////////
//Ĭ��ʵ�֣����������Ҫ������
bool CResMngr::BuildRes( CRes* pRes )
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////
//Ĭ��ʵ�֣����������Ҫ������
CRes* CResMngr::DuplicateRes( const CRes* pRes, void* pParam )
{
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//Ĭ��ʵ�֣����������Ҫ������
bool CResMngr::FixRes( CRes* pRes, bool bFree, void* pParam )
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//Ĭ��ʵ�֣����������Ҫ������
bool CResMngr::Init()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//Ĭ��ʵ�֣����������Ҫ������
//������ر������ClearFree()��������ܻ������Դй©
void CResMngr::Shutdown()
{
	ClearFree();
	CLearAll();
}

///////////////////////////////////////////////////////////////////////////////
//��ͬ����Դ���ҵ���һ��free��
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
//�ҵ�ָ����Դ
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
