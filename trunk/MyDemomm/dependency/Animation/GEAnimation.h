#pragma once
#include"GEAnimationTrack.h"
#include<map>
#include<string>
using namespace std;

/**
 * ��װһ��������������ݺͲ�����.
 * һ�������ļ��пɰ����Ǽ����ݺͶ�����������ݡ�һ����������Ϊһ���������磺�ܶ�����Ծ�ȡ�
 * ���������������֯�ɶ�������GEAnimationSet�����Է�װ����������صĲ�����
 */
class GEAnimation
{
public:
	inline GEAnimation(void) {}
	/**
	 * @param name ������
	 * @param length ��������
	 * @param index ����������
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
	inline float  getLength() { return mLength; }	///< ������ζ����ĳ��ȣ�ʱ�䣩

	/**
	 * ���ĳ��ͷ��track
	 * @param boneIndex Ҫ��ȡ�Ĺ�ͷ��������
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
	 * ���ĳ����ͷ��track��������
	 */
	inline void addTrack(string boneName, GEAnimationTrack* tr)
	{
		int index=tr->getBoneIndex();
		mTracks.insert( pair<int, GEAnimationTrack*>(index, tr));		
		mTracksByName.insert( pair<string, GEAnimationTrack*>(boneName, tr));
	}

	/**
	 * ���ĳ��ͷ��ĳ��ʱ�̵���̬.
	 * ������ʱ����ǰ����֡�ؼ�֡����ֵ�󷵻ص�ǰ������任��
	 * @param boneIndex Ҫ��ȡ�Ĺ�ͷ��������
	 * @param time Ҫ��ȡʲôʱ�������任��
	 */
	inline  GEKeyframe getInterpolatedKeyframe(int boneIndex, float time)
	{
        return this->getTrack(boneIndex)->getInterpolatedKeyframe(time);
	}

protected:
	typedef map<int, GEAnimationTrack*> TrackIndexMap;	///< ��ͷ�����ż���track������
	typedef map<string, GEAnimationTrack*> TrackNameMap;			///< ��ͷ�����ּ���track������

	string			mName;			///<������
	int				mIndex;			///<������������
	float			mLength;		///<�ö����ĳ���
	TrackIndexMap	mTracks;		///< ���й�ͷ�������ż����ڸö����е�track
	TrackNameMap	mTracksByName;	///< ���й�ͷ�����ּ����ڸö����е�track
};
