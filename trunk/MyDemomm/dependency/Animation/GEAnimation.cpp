#include "GEAnimation.h"


GEAnimation::~GEAnimation(void)
{
	TrackIndexMap::iterator it=mTracks.begin();
	while(it!=mTracks.end())
	{
		delete it->second;
		++it;
	}
	mTracks.clear();
	mTracksByName.clear();
}


void GEAnimation::LoadFromData(const BYTE* data)
{

}

/*
void GEAnimation::addFrame(GEAnimationFrame* frame)
{
	float time=frame->getTime();
	mFrames.insert(time, frame);
}

float GEAnimation::getFrame(float time, GEAnimationFrame*& p1, GEAnimationFrame*& p2)
{
	float t1,t2;	
	FrameMap::iterator it=mFrames.begin();
	if(time<=0.0f) // ���time<0�Ļ���Ϊ0
	{
		p1=p2=*(it->second);
		return 1.0f;
	}
	if(time>mLength) { time=mLength; }  // ���time>�ö������ȵĻ���ΪmLength

	while(it!=mFrames.end() && it->second->getTime()<=time)
	{
		t1=it->second->getTime();
		p1=*(it->second);
		++it;
	}

	if(p1->getTime()==time)	// ���time�պþ���ĳ֡��ʱ��
	{
		p2=p1;
		return 1.0f;
	}
	else
	{
		if(p1->getTime()==mLength)	// ���time==mLength�Ļ�
		{		
			p2=p1;
            return 1.0f;
		}
		else	// ��ͨ�����time����֮֡��
		{
			p2=*(it->second);
			t2=p2->getTime();
			return (t2-time)/(t2-t1);
		}
	}
	
}
*/