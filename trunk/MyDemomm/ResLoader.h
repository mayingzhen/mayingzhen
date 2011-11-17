#ifndef	__RES_LOADER_H__
#define	__RES_LOADER_H__

#include <Windows.h>
#include <list>

using namespace std;

class CRes;

class CResLoader
{
public:
	//��ȡ����ָ��
	static CResLoader* GetTheOne();

public:
	//��ʼ��
	bool Init(); 
	void Shutdown();

	void SetPath( const char* pInPath );

	//�����Դ��������
	void AddRequest( CRes* pRes );
	//ɾ����Դ��������
	//����:�Ƿ��ҵ�
	bool RemRequest( CRes* pRes );

private:
	CResLoader();

private:
	//��Դ�����߳�������
	static unsigned int WINAPI ResThreadProc( LPVOID pParam );

	//ȡ�����еĵ�һ����������ͬʱ��������ɾ��
	CRes* GetRequest();

private:
	typedef list< CRes* >	RequestList_t;

	RequestList_t		m_listRequest;			//��Դ������������
	CRITICAL_SECTION	m_CriticalSection;		//�ؼ���
	HANDLE				m_hThread;				//�����߳̾��
	HANDLE				m_hEventRequest;		//��Դ�����¼�
	std::string			m_strPath;

	bool				m_bThreadRunning;		//�����߳����б�־
};

///////////////////////////////////////////////////////////////////////////////
inline CResLoader* ResLoader()
{
	return CResLoader::GetTheOne();
}

#endif