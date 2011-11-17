#ifndef __RES_H__
#define __RES_H__

#include "ResMngr.h"	//������û�취��Ϊ�˿���ResMap_t��ResList_t


enum ResState
{
	RES_STATE_EMPTY = 0,
	RES_STATE_VALID,
	RES_STATE_LOADFAILED
};

class CRes
{
	friend class CResMngr;
	friend class CResLoader;

public:
	CRes();
	virtual ~CRes();

public:
	//�ͷ�
	void Release();

	//��ȡ����
	inline const std::string& GetName() const;

	//�Ƿ����
	inline bool IsValid() const;

	//�Ƿ����ʧ��
	inline bool IsLoadFailed() const;

	// Pointer 08.01.27 ��ʱ�����Ҫ�ֹ�AddRef
	inline void AddRef();

protected:
	//������Դ�ӳ�����ʱ�䣬��Դ������ɵ���
	inline void SetDelayTime( float fTimeOut );

private:
	///////////////////////////////////////////////////////////////////////////////
	//���½ӿڽ���CResMngr��CResLoader����
	///////////////////////////////////////////////////////////////////////////////
	//�������ü���
	inline int IncRef();
	//�������ü���
	inline int DecRef();
	//�Ƿ����ɣ����ü���Ϊ0��
	inline bool IsFree() const;

	//��ȡ�����ӳ�ʱ��
	inline float GetDelayTime() const;
	//��������ʱ��
	inline void SetDeleteTime( double fDeleteTime );
	//��ȡ����ʱ��
	inline double GetDeleteTime() const;
	//��������
	inline const std::string& SetName( const std::string &pStrName );
	//����iterator
	inline void SetIterFree( const ResList_t::iterator &iter );
	//��ȡiterator
	inline const ResList_t::iterator& GetIterFree() const;
	//���ù�����ָ��
	inline void SetMngr( CResMngr* pMngr );
	//��ȡ������ָ��
	inline CResMngr* GetMngr() const;
	//�����Ƿ���Ч
	inline void SetState( ResState state );

private:
	std::string				m_strName;		//����
	int					m_dwRefCount;	//���ü���
	float					m_fTimeDelay;	//ɾ������ʱ�����룩
	double					m_fTimeDelete;	//����ɾ��ʱ�̣��룩
	ResList_t::iterator		m_iterListFree;	//��Free�����У�����ڵĻ�����iterator
	CResMngr*				m_pMngr;		//���ڹ�����ָ��
	ResState				m_State;		//״̬
};

///////////////////////////////////////////////////////////////////////////////
inline void CRes::SetDelayTime( float fTimeDelay )
{
	m_fTimeDelay = fTimeDelay;
}

///////////////////////////////////////////////////////////////////////////////
inline float CRes::GetDelayTime() const
{
	return m_fTimeDelay;
}

///////////////////////////////////////////////////////////////////////////////
inline const std::string& CRes::GetName() const
{
	return m_strName;
}

///////////////////////////////////////////////////////////////////////////////
inline bool CRes::IsValid() const
{
	return ( m_State == RES_STATE_VALID );
}

///////////////////////////////////////////////////////////////////////////////
inline bool CRes::IsLoadFailed() const
{
	return ( m_State == RES_STATE_LOADFAILED );
}

///////////////////////////////////////////////////////////////////////////////
inline void CRes::AddRef()
{
	m_dwRefCount ++;
}

///////////////////////////////////////////////////////////////////////////////
inline int CRes::IncRef()
{
	return ++m_dwRefCount;
}

///////////////////////////////////////////////////////////////////////////////
inline int CRes::DecRef()
{
	return --m_dwRefCount;
}

///////////////////////////////////////////////////////////////////////////////
inline bool CRes::IsFree() const
{
	return ( m_dwRefCount == 0 );
}

///////////////////////////////////////////////////////////////////////////////
inline void CRes::SetDeleteTime( double fTimeDelete )
{
	m_fTimeDelete = fTimeDelete;
}

///////////////////////////////////////////////////////////////////////////////
inline double CRes::GetDeleteTime() const
{
	return m_fTimeDelete;
}

///////////////////////////////////////////////////////////////////////////////
inline const std::string& CRes::SetName( const std::string &strName )
{
	m_strName = strName;
	return m_strName;
}

///////////////////////////////////////////////////////////////////////////////
inline void CRes::SetIterFree( const ResList_t::iterator &iter )
{
	m_iterListFree = iter;
}

///////////////////////////////////////////////////////////////////////////////
inline const ResList_t::iterator& CRes::GetIterFree() const
{
	return m_iterListFree;
}

///////////////////////////////////////////////////////////////////////////////
inline CResMngr* CRes::GetMngr() const
{
	return m_pMngr;
}

///////////////////////////////////////////////////////////////////////////////
inline void CRes::SetMngr( CResMngr* pMngr )
{
	m_pMngr = pMngr;
}

///////////////////////////////////////////////////////////////////////////////
inline void CRes::SetState( ResState state )
{
	m_State = state;
}

///////////////////////////////////////////////////////////////////////////////
#define SAFE_RELEASE_RES( p )	{ if ( p ) { ( p )->Release(); ( p ) = NULL; } }

#endif

