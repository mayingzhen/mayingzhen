#ifndef	__RES_LOADER_H__
#define	__RES_LOADER_H__

#include <Windows.h>
#include <list>

using namespace std;

class CRes;

class CResLoader
{
public:
	//获取单件指针
	static CResLoader* GetTheOne();

public:
	//初始化
	bool Init(); 
	void Shutdown();

	void SetPath( const char* pInPath );

	//添加资源加载请求
	void AddRequest( CRes* pRes );
	//删除资源加载请求。
	//返回:是否找到
	bool RemRequest( CRes* pRes );

private:
	CResLoader();

private:
	//资源加载线程主函数
	static unsigned int WINAPI ResThreadProc( LPVOID pParam );

	//取链表中的第一个加载请求，同时从链表中删除
	CRes* GetRequest();

private:
	typedef list< CRes* >	RequestList_t;

	RequestList_t		m_listRequest;			//资源加载请求链表
	CRITICAL_SECTION	m_CriticalSection;		//关键区
	HANDLE				m_hThread;				//加载线程句柄
	HANDLE				m_hEventRequest;		//资源加载事件
	std::string			m_strPath;

	bool				m_bThreadRunning;		//加载线程运行标志
};

///////////////////////////////////////////////////////////////////////////////
inline CResLoader* ResLoader()
{
	return CResLoader::GetTheOne();
}

#endif