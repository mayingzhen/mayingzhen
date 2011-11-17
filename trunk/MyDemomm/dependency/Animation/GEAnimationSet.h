#pragma once

//#include "GEResource.h"
#include"GEAnimation.h"
#include<map>
#include<string>
using namespace std;

/**
 * 多个动画数据和操作的集合
 */
class GEAnimationSet /*: public GEResource*/
{
public:
	GEAnimationSet(void);
	virtual ~GEAnimationSet(void);
	
	void addAnimation(GEAnimation* animation);	///< 添加一个动画到该动画集中
	GEAnimation* getAnimation(string name);		///< 获得动画集中的一个动画
	GEAnimation* getAnimation(int index);		///< 获得动画集中的一个动画
	unsigned int getNumberAnimation();			///< 返回该动画集中动画的数量
	bool removeAnimation(string name);			///< 从该动画集中移除一个动画
	bool removeAnimation(int index);			///< 从该动画集中移除一个动画

	/** 如何加载资源,重载 */
	virtual bool LoadImpl(void)					{ return true; }
	/** 如何卸载资源,重载 */
	virtual bool UnloadImpl(void);
	/** 如何得到资源文件的大小,重载 */
	virtual size_t CalculateSize(void) const	{ return 0; }


protected:
	typedef map<int,    GEAnimation*> AniIndexMap;
	typedef map<string, GEAnimation*> AniNameMap;
	typedef pair<int,   GEAnimation*> AniIndexMapPair;
	typedef pair<string,GEAnimation*> AniNameMapPair;

	AniIndexMap mSetByIndex;	///< 动画的索引号和对应的动画的指针
	AniNameMap	mSetByName;		///< 动画名和对应的动画的指针

};
