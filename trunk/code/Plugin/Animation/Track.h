#ifndef __Track_H__
#define __Track_H__


namespace ma
{

	inline void WrapFrameClamp(float& frame,float maxFrame)
	{
		frame = Math::Clamp(frame,0.0f,maxFrame);
	}

	inline void DecomposeFrame( float frame,const std::vector<UINT>& keyFrame,UINT& key0,UINT& key1,float& factor )
	{
		profile_code();

		//ASSERT(keyFrame.size() > 0);
		WrapFrameClamp(frame,(float)keyFrame.back());
		std::vector<UINT>::const_iterator iter = std::lower_bound(keyFrame.begin(),keyFrame.end(),(UINT)(frame+1.0f));
		if (iter != keyFrame.end())
		{
			key1 = iter - keyFrame.begin();
			key0 = key1 > 0 ? key1 - 1 : key1;
			factor = key0 < key1 ? ((frame - keyFrame[key0]) / (keyFrame[key1]-keyFrame[key0])) : 0.0f;	

		}else{
			key0 = (UINT)keyFrame.size() - 1;
			key1 = key0;
			factor = 0.0f;
		}
	}


	class ITrack
	{
	public:

		virtual UINT GetKeyNumber() const = 0;

		virtual UINT GetKeyFrame(UINT nKeyInd) const = 0;
	};

	template<class T>
	class DataTrack
	{
	public:
		std::vector<UINT>	m_arrFrame; //frame number of each key
		std::vector<T>		m_arrValue;

	public:

		inline void	SampleFrame(float frame,T& val) const
		{
			UINT nKey0,nKey1;
			float fFactor;
			DecomposeFrame(frame,m_arrFrame,nKey0,nKey1,fFactor);
			if (nKey0 != nKey1)
			{
				Lerp(val,m_arrValue[nKey0],m_arrValue[nKey1],fFactor);
			}else{
				val = m_arrValue[nKey0];
			}
		}

		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		inline void	Init(UINT nKeyFrameNum)
		{
			m_arrFrame.resize(nKeyFrameNum);
			m_arrValue.resize(nKeyFrameNum);
		}

		inline void SetKeyFrame(UINT nKeyFrameInd,UINT nFrame,const T& nVal)
		{
			m_arrFrame[nKeyFrameInd] = nFrame;
			m_arrValue[nKeyFrameInd] = nVal;
		}

		inline UINT	GetFrameNumber() const
		{
			return m_arrFrame.size() > 0 ? m_arrFrame.back() : 0;
		}


		void	DbgDump() const;

		//deprecated, use init instead
		void	Pushback(UINT frame,const T& val)
		{
			m_arrFrame.push_back(frame);
			m_arrValue.push_back(val);
		}		
	};

	typedef DataTrack<float> FloatTrack;


	class Vector3Track : public ITrack
	{
	public:

		std::vector<UINT>		m_arrFrame; //frame number of each key
		std::vector<Vector3>	m_arrValue;


		Vector3Track()
		{

		}

		void	Clear();

		void	Clone(const Vector3Track* pTrack);

		void	SetKeyNumber(UINT keyNum);

		UINT	GetKeyNumber() const;

		UINT	GetFrameNumber() const;


		void	SetKeyFrame(UINT nKeyInd,UINT nFrame);

		UINT	GetKeyFrame(UINT nKeyInd) const;


		void				SetKeyValue(UINT key,const Vector3& val);

		const Vector3&	GetKeyValue(UINT nKeyIndex)const;


		void	SetKey(UINT key, UINT frame, const Vector3& val);


		void	Pushback(UINT frame,const Vector3& val);

		void	SampleFrame(float frame,Vector3& val) const ;

		void	DbgDump() const;
	};

	class QuaternionTrack : public ITrack
	{
	public:


		std::vector<UINT>				m_arrFrame; //frame number of each key
		std::vector<Quaternion>	m_arrValue;


		QuaternionTrack();

		void	Clone(const QuaternionTrack* pTrack);

		void	Clear();

		void	SetKeyNumber(UINT keyNum);

		UINT	GetKeyNumber() const;

		UINT	GetFrameNumber() const;


		void 	SetKeyFrame(UINT nKeyInd,UINT nFrame);

		UINT	GetKeyFrame(UINT nKeyInd) const;


		void 					SetKeyValue(UINT key,const Quaternion& val);

		const Quaternion&	GetKeyValue(UINT nKeyIndex)const;


		void 					SetKey(UINT key,UINT frame,const Quaternion& val);


		void 	Pushback(UINT frame,const Quaternion& val);

		void 	SampleFrame(float frame,Quaternion& val) const;

		void	DbgDump() const;
	};




	//------------------------------------------------------------------------------
	//Vector3Track
	//------------------------------------------------------------------------------

// 	inline Vector3Track::Vector3Track()
// 	{
// 
// 	}

	inline void	Vector3Track::Clear()
	{
		m_arrFrame.clear();
		m_arrValue.clear();
	}

	inline void	Vector3Track::Clone(const Vector3Track* pTrack)
	{
		m_arrFrame = pTrack->m_arrFrame;
		m_arrValue = pTrack->m_arrValue;
	}

	inline void Vector3Track::SetKeyNumber( UINT keyNum )
	{
		m_arrFrame.resize(keyNum);
		m_arrValue.resize(keyNum);

	}

	inline UINT Vector3Track::GetKeyNumber() const
	{
		return (UINT)m_arrValue.size();

	}

	inline void Vector3Track::SetKeyFrame( UINT key,UINT frame )
	{
		m_arrFrame[key] = frame;
	}

	inline UINT	Vector3Track::GetKeyFrame(UINT nKeyInd) const
	{
		return m_arrFrame[nKeyInd];
	}


	inline UINT Vector3Track::GetFrameNumber() const
	{
		return m_arrFrame.size() > 0 ? m_arrFrame[m_arrFrame.size()-1] + 1 : 0;
	}

	inline void Vector3Track::SetKey( UINT key,UINT frame,const Vector3& val )
	{
		m_arrValue[key]		= val;
		m_arrFrame[key]	= frame;
	}


	inline void Vector3Track::SetKeyValue( UINT key,const Vector3& val )
	{
		m_arrValue[key] = val;
	}

	inline const Vector3& Vector3Track::GetKeyValue(UINT nKeyIndex)const
	{
		return m_arrValue[nKeyIndex];
	}

	inline void Vector3Track::Pushback(UINT frame,const Vector3& val)
	{
		m_arrValue.push_back(val);
		m_arrFrame.push_back(frame);
	}

	inline void Vector3Track::SampleFrame( float frame,Vector3& val ) const
	{
		profile_code();

		UINT key0;
		UINT key1;
		float factor; 

		DecomposeFrame(frame,m_arrFrame,key0,key1,factor);
		val = Math::Lerp(m_arrValue[key0],m_arrValue[key1],factor);
	}

	inline void	Vector3Track::DbgDump() const
	{
		//Log("Total Key %u ",m_arrValue.size());
		for (UINT keyCnt = 0; keyCnt < m_arrValue.size(); ++keyCnt)
		{
			//Log(" %d(%.5f,%.5f,%.5f)",m_arrFrame[keyCnt],m_arrValue[keyCnt].x,m_arrValue[keyCnt].y,m_arrValue[keyCnt].z);
		}
	}


	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------


	inline QuaternionTrack::QuaternionTrack()
	{

	}

	inline void	QuaternionTrack::Clone(const QuaternionTrack* pTrack)
	{
		m_arrFrame = pTrack->m_arrFrame;
		m_arrValue = pTrack->m_arrValue;
	}

	inline void	QuaternionTrack::Clear()
	{
		m_arrFrame.clear();
		m_arrValue.clear();
	}

	inline void QuaternionTrack::SetKeyNumber( UINT keyNum )
	{
		m_arrFrame.resize(keyNum);
		m_arrValue.resize(keyNum);

	}

	inline UINT QuaternionTrack::GetKeyNumber() const
	{
		return (UINT)m_arrValue.size();

	}

	inline void QuaternionTrack::SetKeyFrame( UINT key,UINT frame )
	{
		m_arrFrame[key] = frame;
	}

	inline UINT	QuaternionTrack::GetKeyFrame(UINT nKeyInd) const
	{
		return m_arrFrame[nKeyInd];
	}

	inline UINT QuaternionTrack::GetFrameNumber() const
	{
		return m_arrFrame.size() > 0 ? m_arrFrame[m_arrFrame.size()-1]+1 : 0;
	}


	inline void QuaternionTrack::SetKey( UINT key,UINT frame,const Quaternion& val )
	{
		m_arrValue[key]		= val;
		m_arrFrame[key]	= frame;
	}

	inline const Quaternion& QuaternionTrack::GetKeyValue( UINT nKeyIndex )const
	{
		return m_arrValue[nKeyIndex];
	}

	inline void QuaternionTrack::SetKeyValue( UINT key,const Quaternion& val )
	{
		m_arrValue[key] = val;
	}

	inline void QuaternionTrack::Pushback( UINT frame,const Quaternion& val )
	{
		m_arrValue.push_back(val);
		m_arrFrame.push_back(frame);
	}

	inline void QuaternionTrack::SampleFrame( float frame,Quaternion& val ) const
	{
		profile_code();

		UINT key0;
		UINT key1;
		float factor; 
		DecomposeFrame(frame,m_arrFrame,key0,key1,factor);
		val = Quaternion::Slerp(factor,m_arrValue[key0],m_arrValue[key1],true);
	}

	inline void	QuaternionTrack::DbgDump() const
	{
		//Log("Total Key %u ",m_arrValue.size());

		for (UINT keyCnt = 0; keyCnt < m_arrValue.size(); ++keyCnt)
		{
			//Log(" %d(%.5f,%.5f,%.5f,%.5f)",m_arrFrame[keyCnt],m_arrValue[keyCnt].x,m_arrValue[keyCnt].y,m_arrValue[keyCnt].z,m_arrValue[keyCnt].w);
		}
	}
}


#endif