#pragma once

#include"GEKeyframe.h"
#include <map>
using namespace std;

/**
 * 封装一个骨头在一个动画中的所有关键帧及其操作.
 */
class GEAnimationTrack
{
public:
	typedef map<float, GEKeyframe> FrameMap; ///< 关键帧及其时间

	GEAnimationTrack(unsigned short boneIndex);
	virtual ~GEAnimationTrack(void);

	/// 该track保存的是哪个骨头的数据？
	inline unsigned short getBoneIndex() { return mBoneIndex; }
	/// 设置该track对应的骨头
	void   setBoneIndex(unsigned short index) { mBoneIndex=index; }
	/**
	 * 总共拥有多少关键帧.
	 * 注意：同一个动画的不同骨头可以有不同数量的关键帧
	 */
	inline int getFrameNumber()	{ return (int)mKeyframes.size(); }

	/**
	 * 获得某个时刻前后的两个关键帧.
	 * 如果时间恰巧为某个关键帧的时刻，则k1和k2都返回那个关键帧，函数返回值将为1
	 * @param k1 该时间之前最晚的关键帧
	 * @param k2 该时刻之后最早的关键帧
	 * @returns 返回k1的权重，k2权重为（1－k1的权重）
	 */
	float getKeyframe(float time, GEKeyframe& k1, GEKeyframe& k2);
	/**
	 * 获得某个时刻插好值的关键帧，返回的关键帧包含着这个时刻track所对应的骨头坐标变换的数据。
	 */
	GEKeyframe getInterpolatedKeyframe(float time);

	/**
	 * 添加一个关键帧到此track中 
	 */
	inline void addKeyframe(GEKeyframe& kf)
	{
		float t=kf.getTime();
		mKeyframes.insert( pair<float, GEKeyframe>(t,kf));
		if(kf.getTime()>mMaxTime) mMaxTime=kf.getTime();
	}

protected:
	unsigned short mBoneIndex; ///< 该track包含的是哪个骨头的动画信息
	float mMaxTime; ///< 该track所拥有的关键帧中最大的时间
	/// 时间以及对应的关键帧的容器
	FrameMap mKeyframes; // map<float, GEKeyframe>
};
