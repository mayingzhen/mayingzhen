#include ".\geanimationtrack.h"

GEAnimationTrack::GEAnimationTrack(unsigned short boneIndex) 
	: mBoneIndex(boneIndex), mMaxTime(0)
{
}

GEAnimationTrack::~GEAnimationTrack(void)
{
}


float GEAnimationTrack::getKeyframe(float time, GEKeyframe& k1, GEKeyframe& k2)
{	
	float t1,t2;	
	FrameMap::iterator it=mKeyframes.begin();
	if(time<=0.0f) // 如果time<0的话视为0
	{
		k1=k2=it->second;
		return 1.0f;
	}
	if(time>mMaxTime) { time=mMaxTime; }  // 如果time>该track长度的话视为该track的长度

	while(it!=mKeyframes.end() && (it->second).getTime()<=time)
	{
		k1=it->second;
		t1=k1.getTime();
		++it;
	}

	if(k1.getTime()==time)	// 如果time刚好就是某帧的时间
	{
		k2=k1;
		return 1.0f;
	}
	else
	{
		if(k1.getTime()==mMaxTime)	// 如果time==该track的长度的话
		{		
			k2=k1;
			return 1.0f;
		}
		else	// 普通情况，time在两帧之间
		{
			k2=it->second;
			t2=k2.getTime();
			return (t2-time)/(t2-t1);
		}
	}
}

GEKeyframe GEAnimationTrack::getInterpolatedKeyframe(float time)
{
	GEKeyframe k1,k2, k3;
	float weight;
	weight=this->getKeyframe(time, k1,k2);
	k3=k1.interpolate(k2, 1-weight);
	k3.setTime(time);

	return k3;
}



