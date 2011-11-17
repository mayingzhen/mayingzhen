#pragma once
#include"GEAnimationTrack.h"
#include<map>
#include<string>
using namespace std;

/**
 * 封装一个动画的相关数据和操作。.
 * 一个动画文件中可包含骨架数据和多个动画的数据。一个动作序列为一个动画，如：跑动、跳跃等。
 * 多个动画的数据组织成动画集（GEAnimationSet），以封装与多个动画相关的操作。
 */
class GEAnimation
{
public:
	inline GEAnimation(void) {}
	/**
	 * @param name 动画名
	 * @param length 动画长度
	 * @param index 动画索引号
	 */
	inline GEAnimation(string name, float length, int index)
		: mName(name), mLength(length), mIndex(index) {}	
	virtual ~GEAnimation(void);

	void LoadFromData(const BYTE* data);

	inline void   setIndex(int index) { mIndex=index; }
	inline int    getIndex() { return mIndex; }
	inline void   setName(string s) { mName=s; }
	inline string getName() { return mName; }
	inline void   setLength(float l) { mLength=l; }
	inline float  getLength() { return mLength; }	///< 获得整段动画的长度（时间）

	/**
	 * 获得某骨头的track
	 * @param boneIndex 要获取的骨头的索引号
	 */
	inline GEAnimationTrack* getTrack(int boneIndex)
	{
		TrackIndexMap::iterator it=mTracks.find(boneIndex);
		if(it!=mTracks.end())
			return it->second;
		else
			return NULL;
	}

	/**
	 * 添加某个骨头的track到动画中
	 */
	inline void addTrack(string boneName, GEAnimationTrack* tr)
	{
		int index=tr->getBoneIndex();
		mTracks.insert( pair<int, GEAnimationTrack*>(index, tr));		
		mTracksByName.insert( pair<string, GEAnimationTrack*>(boneName, tr));
	}

	/**
	 * 获得某骨头在某个时刻的姿态.
	 * 将根据时间获得前后两帧关键帧并插值后返回当前的坐标变换。
	 * @param boneIndex 要获取的骨头的索引号
	 * @param time 要获取什么时间的坐标变换？
	 */
	inline  GEKeyframe getInterpolatedKeyframe(int boneIndex, float time)
	{
        return this->getTrack(boneIndex)->getInterpolatedKeyframe(time);
	}

protected:
	typedef map<int, GEAnimationTrack*> TrackIndexMap;	///< 骨头索引号及其track的容器
	typedef map<string, GEAnimationTrack*> TrackNameMap;			///< 骨头的名字及其track的容器

	string			mName;			///<动画名
	int				mIndex;			///<动画的索引号
	float			mLength;		///<该动画的长度
	TrackIndexMap	mTracks;		///< 所有骨头的索引号及其在该动画中的track
	TrackNameMap	mTracksByName;	///< 所有骨头的名字及其在该动画中的track
};
