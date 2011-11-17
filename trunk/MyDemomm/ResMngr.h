#ifndef __RES_MNGR_H__
#define __RES_MNGR_H__

#include <hash_map>
#include <map>
#include <list>
#include <string>

#define	RM_TYPE_FORBID_REUSE	1

class CRes;

typedef std::map<const std::string, CRes*>	ResMap_t;
typedef std::list< ResMap_t::iterator >	ResList_t;

class CResMngr
{
	friend class CResLoader;	//资源加载线程需要调用BuildRes

public:
	CResMngr();
	//CResMngr( DWORD dwType );
	virtual ~CResMngr();

public:
	//初始化
	virtual bool Init();

	//关闭
	virtual void Shutdown();

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//作用：发送资源加载请求
	//参数：1.资源名字。2.是否同步,true则同步加载，false则交给资源线程加载。3.参数。4.强制请求(忽略优化，就是更新，说别的没用)
	CRes* Query(const std::string &strName, bool buildNow = true, void* pParam = NULL);

	//释放资源
	void Release( CRes* pRes );

	//修改所有资源
	void FixAll( void* pParam );

	//刷新维护
	void Refresh( double fTime );

	//清除Free链表中的资源
	void ClearFree();

	void CLearAll();

	// 查询资源是否有效
	bool Find( const std::string& strName );

protected:
	//创建资源，派生类必须实现。这里必须考虑线程安全
	//bBuildNow	是否立即加载
	virtual CRes* CreateRes( const std::string& strName, bool bBuildNow, void* pParam ) = 0;

	//加载资源，加载线程调用，这里必须考虑线程安全
	virtual bool BuildRes( CRes* pRes );

	//复制资源
	virtual CRes* DuplicateRes( const CRes* pRes, void* pParam );

	//销毁资源，派生类必须实现
	virtual void DestroyRes( CRes* pRes ) = 0;

	//修改资源，派生类如果需要可重载
	virtual bool FixRes( CRes* pRes, bool bFree, void* pParam );

private:
	CRes* FindFirstFreeRes( ResMap_t::iterator& iter, const std::string& strName );

	ResMap_t::iterator FindRes( CRes* pRes );

private:
	ResMap_t		m_mapRes;		//资源映射表
	double			m_fTimeCur;		//当前时间
	ResList_t		m_listFree;		//自由（待删除）资源链表
	//DWORD			m_dwType;
};

#endif