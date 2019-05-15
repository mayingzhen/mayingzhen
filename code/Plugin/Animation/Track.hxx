#include "Track.h"


namespace ma
{

	void WrapFrameClamp(float& frame,float maxFrame)
	{
		frame = Math::Clamp(frame,0.0f,maxFrame);
	}

	void DecomposeFrame( float frame,const std::vector<uint32_t>& keyFrame,uint32_t& key0,uint32_t& key1,float& factor )
	{
		//ASSERT(keyFrame.size() > 0);
		WrapFrameClamp(frame,(float)keyFrame.back());
		std::vector<uint32_t>::const_iterator iter = std::lower_bound(keyFrame.begin(),keyFrame.end(),(uint32_t)(frame+1.0f));
		if (iter != keyFrame.end())
		{
			key1 = iter - keyFrame.begin();
			key0 = key1 > 0 ? key1 - 1 : key1;
			factor = key0 < key1 ? ((frame - keyFrame[key0]) / (keyFrame[key1]-keyFrame[key0])) : 0.0f;	

		}else{
			key0 = (uint32_t)keyFrame.size() - 1;
			key1 = key0;
			factor = 0.0f;
		}
	}

	void Vector3Track::Clear()
	{
		m_arrFrame.clear();
		m_arrValue.clear();
	}

	void Vector3Track::Clone(const Vector3Track* pTrack)
	{
		m_arrFrame = pTrack->m_arrFrame;
		m_arrValue = pTrack->m_arrValue;
	}

	void Vector3Track::SetKeyNumber( uint32_t keyNum )
	{
		m_arrFrame.resize(keyNum);
		m_arrValue.resize(keyNum);

	}

	uint32_t Vector3Track::GetKeyNumber() const
	{
		return (uint32_t)m_arrValue.size();
	}

	void Vector3Track::SetKeyFrame( uint32_t key,uint32_t frame )
	{
		m_arrFrame[key] = frame;
	}

	uint32_t Vector3Track::GetKeyFrame(uint32_t nKeyInd) const
	{
		return m_arrFrame[nKeyInd];
	}

	uint32_t Vector3Track::GetFrameNumber() const
	{
		return m_arrFrame.size() > 0 ? m_arrFrame[m_arrFrame.size()-1] + 1 : 0;
	}

	void Vector3Track::SetKey( uint32_t key,uint32_t frame,const Vector3& val )
	{
		m_arrValue[key]	= val;
		m_arrFrame[key]	= frame;
	}

	void Vector3Track::SetKeyValue( uint32_t key,const Vector3& val )
	{
		m_arrValue[key] = val;
	}

	const Vector3& Vector3Track::GetKeyValue(uint32_t nKeyIndex)const
	{
		return m_arrValue[nKeyIndex];
	}

	void Vector3Track::Pushback(uint32_t frame,const Vector3& val)
	{
		m_arrValue.push_back(val);
		m_arrFrame.push_back(frame);
	}

	void Vector3Track::SampleFrame( float frame,Vector3& val ) const
	{
		uint32_t key0;
		uint32_t key1;
		float factor; 

		DecomposeFrame(frame,m_arrFrame,key0,key1,factor);
		val = Math::Lerp(m_arrValue[key0],m_arrValue[key1],factor);
	}

	void Vector3Track::DbgDump() const
	{
		//Log("Total Key %u ",m_arrValue.size());
		for (uint32_t keyCnt = 0; keyCnt < m_arrValue.size(); ++keyCnt)
		{
			//Log(" %d(%.5f,%.5f,%.5f)",m_arrFrame[keyCnt],m_arrValue[keyCnt].x,m_arrValue[keyCnt].y,m_arrValue[keyCnt].z);
		}
	}


	RefPtr<Vector3Track> CreateVector3Track()
	{
		return new Vector3Track();
	}


	void QuaternionTrack::Clone(const QuaternionTrack* pTrack)
	{
		m_arrFrame = pTrack->m_arrFrame;
		m_arrValue = pTrack->m_arrValue;
	}

	void QuaternionTrack::Clear()
	{
		m_arrFrame.clear();
		m_arrValue.clear();
	}

	void QuaternionTrack::SetKeyNumber( uint32_t keyNum )
	{
		m_arrFrame.resize(keyNum);
		m_arrValue.resize(keyNum);

	}

	uint32_t QuaternionTrack::GetKeyNumber() const
	{
		return (uint32_t)m_arrValue.size();
	}

	void QuaternionTrack::SetKeyFrame( uint32_t key,uint32_t frame )
	{
		m_arrFrame[key] = frame;
	}

	uint32_t QuaternionTrack::GetKeyFrame(uint32_t nKeyInd) const
	{
		return m_arrFrame[nKeyInd];
	}

	uint32_t QuaternionTrack::GetFrameNumber() const
	{
		return m_arrFrame.size() > 0 ? m_arrFrame[m_arrFrame.size()-1]+1 : 0;
	}


	void QuaternionTrack::SetKey( uint32_t key,uint32_t frame,const Quaternion& val )
	{
		m_arrValue[key]	= val;
		m_arrFrame[key]	= frame;
	}

	const Quaternion& QuaternionTrack::GetKeyValue( uint32_t nKeyIndex )const
	{
		return m_arrValue[nKeyIndex];
	}

	void QuaternionTrack::SetKeyValue( uint32_t key,const Quaternion& val )
	{
		m_arrValue[key] = val;
	}

	void QuaternionTrack::Pushback( uint32_t frame,const Quaternion& val )
	{
		m_arrValue.push_back(val);
		m_arrFrame.push_back(frame);
	}

	void QuaternionTrack::SampleFrame( float frame,Quaternion& val ) const
	{
		uint32_t key0;
		uint32_t key1;
		float factor; 
		DecomposeFrame(frame,m_arrFrame,key0,key1,factor);
		val = Quaternion::Slerp(factor,m_arrValue[key0],m_arrValue[key1],true);
	}

	void QuaternionTrack::DbgDump() const
	{
		//Log("Total Key %u ",m_arrValue.size());

		for (uint32_t keyCnt = 0; keyCnt < m_arrValue.size(); ++keyCnt)
		{
			//Log(" %d(%.5f,%.5f,%.5f,%.5f)",m_arrFrame[keyCnt],m_arrValue[keyCnt].x,m_arrValue[keyCnt].y,m_arrValue[keyCnt].z,m_arrValue[keyCnt].w);
		}
	}


	RefPtr<QuaternionTrack> CreateQuaternionTrack()
	{
		return new QuaternionTrack();
	}


}
