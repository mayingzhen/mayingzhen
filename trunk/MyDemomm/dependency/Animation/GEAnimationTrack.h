#pragma once

#include"GEKeyframe.h"
#include <map>
using namespace std;

/**
 * ��װһ����ͷ��һ�������е����йؼ�֡�������.
 */
class GEAnimationTrack
{
public:
	typedef map<float, GEKeyframe> FrameMap; ///< �ؼ�֡����ʱ��

	GEAnimationTrack(unsigned short boneIndex);
	virtual ~GEAnimationTrack(void);

	/// ��track��������ĸ���ͷ�����ݣ�
	inline unsigned short getBoneIndex() { return mBoneIndex; }
	/// ���ø�track��Ӧ�Ĺ�ͷ
	void   setBoneIndex(unsigned short index) { mBoneIndex=index; }
	/**
	 * �ܹ�ӵ�ж��ٹؼ�֡.
	 * ע�⣺ͬһ�������Ĳ�ͬ��ͷ�����в�ͬ�����Ĺؼ�֡
	 */
	inline int getFrameNumber()	{ return (int)mKeyframes.size(); }

	/**
	 * ���ĳ��ʱ��ǰ��������ؼ�֡.
	 * ���ʱ��ǡ��Ϊĳ���ؼ�֡��ʱ�̣���k1��k2�������Ǹ��ؼ�֡����������ֵ��Ϊ1
	 * @param k1 ��ʱ��֮ǰ����Ĺؼ�֡
	 * @param k2 ��ʱ��֮������Ĺؼ�֡
	 * @returns ����k1��Ȩ�أ�k2Ȩ��Ϊ��1��k1��Ȩ�أ�
	 */
	float getKeyframe(float time, GEKeyframe& k1, GEKeyframe& k2);
	/**
	 * ���ĳ��ʱ�̲��ֵ�Ĺؼ�֡�����صĹؼ�֡���������ʱ��track����Ӧ�Ĺ�ͷ����任�����ݡ�
	 */
	GEKeyframe getInterpolatedKeyframe(float time);

	/**
	 * ���һ���ؼ�֡����track�� 
	 */
	inline void addKeyframe(GEKeyframe& kf)
	{
		float t=kf.getTime();
		mKeyframes.insert( pair<float, GEKeyframe>(t,kf));
		if(kf.getTime()>mMaxTime) mMaxTime=kf.getTime();
	}

protected:
	unsigned short mBoneIndex; ///< ��track���������ĸ���ͷ�Ķ�����Ϣ
	float mMaxTime; ///< ��track��ӵ�еĹؼ�֡������ʱ��
	/// ʱ���Լ���Ӧ�Ĺؼ�֡������
	FrameMap mKeyframes; // map<float, GEKeyframe>
};
