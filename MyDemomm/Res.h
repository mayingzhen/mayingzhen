#ifndef __RES_H__
#define __RES_H__

#include "ResMngr.h"	//土，但没办法。为了看见ResMap_t和ResList_t


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
	//释放
	void Release();

	//获取名字
	inline const std::string& GetName() const;

	//是否可用
	inline bool IsValid() const;

	//是否加载失败
	inline bool IsLoadFailed() const;

	// Pointer 08.01.27 有时候必须要手工AddRef
	inline void AddRef();

protected:
	//设置资源延迟销毁时间，资源派生类可调用
	inline void SetDelayTime( float fTimeOut );

private:
	///////////////////////////////////////////////////////////////////////////////
	//以下接口仅供CResMngr或CResLoader调用
	///////////////////////////////////////////////////////////////////////////////
	//增加引用计数
	inline int IncRef();
	//减少引用计数
	inline int DecRef();
	//是否自由（引用计数为0）
	inline bool IsFree() const;

	//获取销毁延迟时长
	inline float GetDelayTime() const;
	//设置销毁时间
	inline void SetDeleteTime( double fDeleteTime );
	//获取销毁时间
	inline double GetDeleteTime() const;
	//设置名字
	inline const std::string& SetName( const std::string &pStrName );
	//设置iterator
	inline void SetIterFree( const ResList_t::iterator &iter );
	//获取iterator
	inline const ResList_t::iterator& GetIterFree() const;
	//设置管理器指针
	inline void SetMngr( CResMngr* pMngr );
	//获取管理器指针
	inline CResMngr* GetMngr() const;
	//设置是否有效
	inline void SetState( ResState state );

private:
	std::string				m_strName;		//名称
	int					m_dwRefCount;	//引用计数
	float					m_fTimeDelay;	//删除缓冲时长（秒）
	double					m_fTimeDelete;	//将被删除时刻（秒）
	ResList_t::iterator		m_iterListFree;	//在Free链表中（如果在的话）的iterator
	CResMngr*				m_pMngr;		//所在管理器指针
	ResState				m_State;		//状态
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

