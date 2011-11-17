#pragma once

#include<map>
using namespace std;

#include"GEBone.h"
//#include "GEResource.h"
class GEAnimationController;

/**
 * 骨架，组织骨头的层次结构并封装骨头获取关键帧、更新骨架等的操作
 */
class GESkeleton /*: public GEResource*/
{
public:
	GESkeleton(void);
	virtual ~GESkeleton(void);

	/// 获得根结点的数量
	inline size_t getRootNumber() const { return mRoots.size(); }	
	/// 获得骨头数量
	inline size_t getBoneNumber() const { return mBones.size(); }

	/// 获得根结点的vector
	vector<GEBone*>& getRoot() { return mRoots; }	
	/// 获得指定索引号的骨头
	GEBone* getBone(unsigned short index);	
	/// 获得指定名字的骨头
	GEBone* getBone(const string& name);
	

	/**
	 * 创建该skeleton里的一个bone，自动赋予全局唯一的索引号
	 * @param parent 默认值-1表示该骨头没有父节点（是根节点）
	 * @returns 返回所创建骨头的索引号
	 */
	GEBone* createBone(const string& name, int parent=-1);

	/**
	* 创建该skeleton里的一个bone
	* @param index 赋给该骨头的索引号
	* @param parent 默认值-1表示该骨头没有父节点（是根节点）
	* @returns 返回所创建骨头的索引号
	*/
	GEBone* createBone(unsigned short index, const string& name, int parent=-1);
	
	/**
	 * 有时候客户程序员在解析骨架文件的时候经常在createBone时不指明父节点，而后才
	 * 用GEBone的setParent()指明父节点；如果这样，要在所有bone都初始化完成后调用此
	 * 函数，以更新GESkeleton中的根节点信息。
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
	 * 获得当前帧的动画信息，更新各骨头的相对父节点的变换
	 */	
	void setCurrentPose(GEAnimationController* controller);

	/**
	 * 根据相对父节点的变换更新整个骨架，算出各骨头的绝对坐标变换
	 */
	inline void update()
	{
		for(unsigned int i=0; i<mRoots.size(); i++)
		{
			mRoots[i]->update();
		}
	}

	/**
	 * 使用初始变换计算绝对变换（计算初始逆变换时需要用到）
	 */
	inline void updateWithInitialTransform()
	{
		for(unsigned int i=0; i<mRoots.size(); i++)
		{
			mRoots[i]->updateWithInitialTransform();
		}
	}

	/**
	 * 在完成所有骨头的初始化（包括各骨头相对父节点的初始坐标变换）后，要调用此函数
	 * 计算各骨头的绝对初始逆变换。这对后面骨骼动画的计算很重要。
	 */
	void computeInverseInitialTransform();


	/** 如何加载资源,重载 */
	virtual bool LoadImpl(void)					{ return true; }
	/** 如何卸载资源,重载 */
	virtual bool UnloadImpl(void);
	/** 如何得到资源文件的大小,重载 */
	virtual size_t CalculateSize(void) const	{ return 0; }



protected:
	typedef vector<GEBone*> BoneVector;
	typedef map<int, GEBone*> BoneIndexMap;
	typedef map<string, GEBone*> BoneNameMap;
	typedef pair<int, GEBone*> BoneIndexPair;
	typedef pair<string, GEBone*> BoneNamePair;

	vector<GEBone*> mRoots;			///< 持有所有根结点的容器
	BoneIndexMap mBones;			///< 所有骨头及其索引号
	BoneNameMap mBonesByName;		///< 所有骨头及其名字
	unsigned short mCurrentMaxIndex;

};
