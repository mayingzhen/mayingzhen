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
	if(time<=0.0f) // ���time<0�Ļ���Ϊ0
	{
		k1=k2=it->second;
		return 1.0f;
	}
	if(time>mMaxTime) { time=mMaxTime; }  // ���time>��track���ȵĻ���Ϊ��track�ĳ���

	while(it!=mKeyframes.end() && (it->second).getTime()<=time)
	{
		k1=it->second;
		t1=k1.getTime();
		++it;
	}

	if(k1.getTime()==time)	// ���time�պþ���ĳ֡��ʱ��
	{
		k2=k1;
		return 1.0f;
	}
	else
	{
		if(k1.getTime()==mMaxTime)	// ���time==��track�ĳ��ȵĻ�
		{		
			k2=k1;
			return 1.0f;
		}
		else	// ��ͨ�����time����֮֡��
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



