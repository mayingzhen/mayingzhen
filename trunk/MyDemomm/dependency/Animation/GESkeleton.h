#pragma once

#include<map>
using namespace std;

#include"GEBone.h"
//#include "GEResource.h"
class GEAnimationController;

/**
 * �Ǽܣ���֯��ͷ�Ĳ�νṹ����װ��ͷ��ȡ�ؼ�֡�����¹ǼܵȵĲ���
 */
class GESkeleton /*: public GEResource*/
{
public:
	GESkeleton(void);
	virtual ~GESkeleton(void);

	/// ��ø���������
	inline size_t getRootNumber() const { return mRoots.size(); }	
	/// ��ù�ͷ����
	inline size_t getBoneNumber() const { return mBones.size(); }

	/// ��ø�����vector
	vector<GEBone*>& getRoot() { return mRoots; }	
	/// ���ָ�������ŵĹ�ͷ
	GEBone* getBone(unsigned short index);	
	/// ���ָ�����ֵĹ�ͷ
	GEBone* getBone(const string& name);
	

	/**
	 * ������skeleton���һ��bone���Զ�����ȫ��Ψһ��������
	 * @param parent Ĭ��ֵ-1��ʾ�ù�ͷû�и��ڵ㣨�Ǹ��ڵ㣩
	 * @returns ������������ͷ��������
	 */
	GEBone* createBone(const string& name, int parent=-1);

	/**
	* ������skeleton���һ��bone
	* @param index �����ù�ͷ��������
	* @param parent Ĭ��ֵ-1��ʾ�ù�ͷû�и��ڵ㣨�Ǹ��ڵ㣩
	* @returns ������������ͷ��������
	*/
	GEBone* createBone(unsigned short index, const string& name, int parent=-1);
	
	/**
	 * ��ʱ��ͻ�����Ա�ڽ����Ǽ��ļ���ʱ�򾭳���createBoneʱ��ָ�����ڵ㣬�����
	 * ��GEBone��setParent()ָ�����ڵ㣻���������Ҫ������bone����ʼ����ɺ���ô�
	 * �������Ը���GESkeleton�еĸ��ڵ���Ϣ��
	 */
	inline void findRoots()
	{
		mRoots.clear();
		BoneIndexMap::iterator it;
		for(it=mBones.begin(); it!=mBones.end(); ++it)
		{
			if(it->second->getParent()<0)
				mRoots.push_back(it->second);
		}
	}



	/**
	 * ��õ�ǰ֡�Ķ�����Ϣ�����¸���ͷ����Ը��ڵ�ı任
	 */	
	void setCurrentPose(GEAnimationController* controller);

	/**
	 * ������Ը��ڵ�ı任���������Ǽܣ��������ͷ�ľ�������任
	 */
	inline void update()
	{
		for(unsigned int i=0; i<mRoots.size(); i++)
		{
			mRoots[i]->update();
		}
	}

	/**
	 * ʹ�ó�ʼ�任������Ա任�������ʼ��任ʱ��Ҫ�õ���
	 */
	inline void updateWithInitialTransform()
	{
		for(unsigned int i=0; i<mRoots.size(); i++)
		{
			mRoots[i]->updateWithInitialTransform();
		}
	}

	/**
	 * ��������й�ͷ�ĳ�ʼ������������ͷ��Ը��ڵ�ĳ�ʼ����任����Ҫ���ô˺���
	 * �������ͷ�ľ��Գ�ʼ��任����Ժ�����������ļ������Ҫ��
	 */
	void computeInverseInitialTransform();


	/** ��μ�����Դ,���� */
	virtual bool LoadImpl(void)					{ return true; }
	/** ���ж����Դ,���� */
	virtual bool UnloadImpl(void);
	/** ��εõ���Դ�ļ��Ĵ�С,���� */
	virtual size_t CalculateSize(void) const	{ return 0; }



protected:
	typedef vector<GEBone*> BoneVector;
	typedef map<int, GEBone*> BoneIndexMap;
	typedef map<string, GEBone*> BoneNameMap;
	typedef pair<int, GEBone*> BoneIndexPair;
	typedef pair<string, GEBone*> BoneNamePair;

	vector<GEBone*> mRoots;			///< �������и���������
	BoneIndexMap mBones;			///< ���й�ͷ����������
	BoneNameMap mBonesByName;		///< ���й�ͷ��������
	unsigned short mCurrentMaxIndex;

};
