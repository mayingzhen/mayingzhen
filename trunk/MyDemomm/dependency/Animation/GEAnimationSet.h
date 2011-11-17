#pragma once

//#include "GEResource.h"
#include"GEAnimation.h"
#include<map>
#include<string>
using namespace std;

/**
 * ����������ݺͲ����ļ���
 */
class GEAnimationSet /*: public GEResource*/
{
public:
	GEAnimationSet(void);
	virtual ~GEAnimationSet(void);
	
	void addAnimation(GEAnimation* animation);	///< ���һ���������ö�������
	GEAnimation* getAnimation(string name);		///< ��ö������е�һ������
	GEAnimation* getAnimation(int index);		///< ��ö������е�һ������
	unsigned int getNumberAnimation();			///< ���ظö������ж���������
	bool removeAnimation(string name);			///< �Ӹö��������Ƴ�һ������
	bool removeAnimation(int index);			///< �Ӹö��������Ƴ�һ������

	/** ��μ�����Դ,���� */
	virtual bool LoadImpl(void)					{ return true; }
	/** ���ж����Դ,���� */
	virtual bool UnloadImpl(void);
	/** ��εõ���Դ�ļ��Ĵ�С,���� */
	virtual size_t CalculateSize(void) const	{ return 0; }


protected:
	typedef map<int,    GEAnimation*> AniIndexMap;
	typedef map<string, GEAnimation*> AniNameMap;
	typedef pair<int,   GEAnimation*> AniIndexMapPair;
	typedef pair<string,GEAnimation*> AniNameMapPair;

	AniIndexMap mSetByIndex;	///< �����������źͶ�Ӧ�Ķ�����ָ��
	AniNameMap	mSetByName;		///< �������Ͷ�Ӧ�Ķ�����ָ��

};
