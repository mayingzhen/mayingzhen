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
	friend class CResLoader;	//��Դ�����߳���Ҫ����BuildRes

public:
	CResMngr();
	//CResMngr( DWORD dwType );
	virtual ~CResMngr();

public:
	//��ʼ��
	virtual bool Init();

	//�ر�
	virtual void Shutdown();

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���ã�������Դ��������
	//������1.��Դ���֡�2.�Ƿ�ͬ��,true��ͬ�����أ�false�򽻸���Դ�̼߳��ء�3.������4.ǿ������(�����Ż������Ǹ��£�˵���û��)
	CRes* Query(const std::string &strName, bool buildNow = true, void* pParam = NULL);

	//�ͷ���Դ
	void Release( CRes* pRes );

	//�޸�������Դ
	void FixAll( void* pParam );

	//ˢ��ά��
	void Refresh( double fTime );

	//���Free�����е���Դ
	void ClearFree();

	void CLearAll();

	// ��ѯ��Դ�Ƿ���Ч
	bool Find( const std::string& strName );

protected:
	//������Դ�����������ʵ�֡�������뿼���̰߳�ȫ
	//bBuildNow	�Ƿ���������
	virtual CRes* CreateRes( const std::string& strName, bool bBuildNow, void* pParam ) = 0;

	//������Դ�������̵߳��ã�������뿼���̰߳�ȫ
	virtual bool BuildRes( CRes* pRes );

	//������Դ
	virtual CRes* DuplicateRes( const CRes* pRes, void* pParam );

	//������Դ�����������ʵ��
	virtual void DestroyRes( CRes* pRes ) = 0;

	//�޸���Դ�������������Ҫ������
	virtual bool FixRes( CRes* pRes, bool bFree, void* pParam );

private:
	CRes* FindFirstFreeRes( ResMap_t::iterator& iter, const std::string& strName );

	ResMap_t::iterator FindRes( CRes* pRes );

private:
	ResMap_t		m_mapRes;		//��Դӳ���
	double			m_fTimeCur;		//��ǰʱ��
	ResList_t		m_listFree;		//���ɣ���ɾ������Դ����
	//DWORD			m_dwType;
};

#endif