#ifndef __Track_H__
#define __Track_H__

// #include <Animation/Animation/S3AKeyFrameHelper.h>
// #include <Animation/Animation/S3AAnimationConfig.h>
// #include <Animation/Interface/IS3ATrack.h>
// #include <Animation/Animation/S3AKeyFrameHelper.h>

// namespace ma
// {
	inline float xmClamp(float fVal,float fMin,float fMax)
	{
		fVal = fVal > fMin ? fVal : fMin;
		fVal = fVal < fMax ? fVal : fMax;
		return fVal;
	}

	template<class T>
	inline void xmLerp(T& out, const T& v0,const T& v1,float factor)
	{
		out = v0*(1.0f-factor)+v1*factor;
	}


	inline void WrapFrameClamp(float& frame,float maxFrame)
	{
		frame = xmClamp(frame,0.0f,maxFrame);
	}

	inline void DecomposeFrame( float frame,const std::vector<xmUint>& keyFrame,xmUint& key0,xmUint& key1,float& factor )
	{
		//S3ASSERT(keyFrame.size() > 0);
		WrapFrameClamp(frame,(float)keyFrame.back());
		std::vector<xmUint>::const_iterator iter = std::lower_bound(keyFrame.begin(),keyFrame.end(),(xmUint)(frame+1.0f));
		if (iter != keyFrame.end())
		{
			key1 = iter - keyFrame.begin();
			key0 = key1 > 0 ? key1 - 1 : key1;
			factor = key0 < key1 ? ((frame - keyFrame[key0]) / (keyFrame[key1]-keyFrame[key0])) : 0.0f;	

		}else{
			key0 = (xmUint)keyFrame.size() - 1;
			key1 = key0;
			factor = 0.0f;
		}
	}


	class IS3ATrack
	{
	public:

		virtual xmUint GetKeyNumber() const = 0;

		virtual xmUint GetKeyFrame(xmUint nKeyInd) const = 0;
	};

	template<class T>
	class S3ADataTrack
	{
	public:
		std::vector<xmUint>	m_arrFrame; //frame number of each key
		std::vector<T>		m_arrValue;

	public:

		inline void	SampleFrame(float frame,T& val) const
		{
			xmUint nKey0,nKey1;
			float fFactor;
			DecomposeFrame(frame,m_arrFrame,nKey0,nKey1,fFactor);
			if (nKey0 != nKey1)
			{
				xmLerp(val,m_arrValue[nKey0],m_arrValue[nKey1],fFactor);
			}else{
				val = m_arrValue[nKey0];
			}
		}

		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		inline void	Init(xmUint nKeyFrameNum)
		{
			m_arrFrame.resize(nKeyFrameNum);
			m_arrValue.resize(nKeyFrameNum);
		}

		inline void SetKeyFrame(xmUint nKeyFrameInd,xmUint nFrame,const T& nVal)
		{
			m_arrFrame[nKeyFrameInd] = nFrame;
			m_arrValue[nKeyFrameInd] = nVal;
		}

		inline xmUint	GetFrameNumber() const
		{
			return m_arrFrame.size() > 0 ? m_arrFrame.back() : 0;
		}


		void	DbgDump() const;

		//deprecated, use init instead
		void	Pushback(xmUint frame,const T& val)
		{
			m_arrFrame.push_back(frame);
			m_arrValue.push_back(val);
		}		
	};

	typedef S3ADataTrack<float> xmFloatTrack;


	class xmVector3Track : public IS3ATrack
	{
	public:

		std::vector<xmUint>			m_arrFrame; //frame number of each key
		std::vector<D3DXVECTOR3>	m_arrValue;


		xmVector3Track()
		{

		}

		void	Clear();

		void	Clone(const xmVector3Track* pTrack);

		void	SetKeyNumber(xmUint keyNum);

		xmUint	GetKeyNumber() const;

		xmUint	GetFrameNumber() const;


		void	SetKeyFrame(xmUint nKeyInd,xmUint nFrame);

		xmUint	GetKeyFrame(xmUint nKeyInd) const;


		void				SetKeyValue(xmUint key,const D3DXVECTOR3& val);

		const D3DXVECTOR3&	GetKeyValue(xmUint nKeyIndex)const;


		void	SetKey(xmUint key, xmUint frame, const D3DXVECTOR3& val);


		void	Pushback(xmUint frame,const D3DXVECTOR3& val);

		void	SampleFrame(float frame,D3DXVECTOR3& val) const ;

		void	DbgDump() const;
	};

	class xmQuaternionTrack : public IS3ATrack
	{
	public:


		std::vector<xmUint>				m_arrFrame; //frame number of each key
		std::vector<xmQuaternion>	m_arrValue;


		xmQuaternionTrack();

		void	Clone(const xmQuaternionTrack* pTrack);

		void	Clear();

		void	SetKeyNumber(xmUint keyNum);

		xmUint	GetKeyNumber() const;

		xmUint	GetFrameNumber() const;


		void 	SetKeyFrame(xmUint nKeyInd,xmUint nFrame);

		xmUint	GetKeyFrame(xmUint nKeyInd) const;


		void 					SetKeyValue(xmUint key,const xmQuaternion& val);

		const xmQuaternion&	GetKeyValue(xmUint nKeyIndex)const;


		void 					SetKey(xmUint key,xmUint frame,const xmQuaternion& val);


		void 	Pushback(xmUint frame,const xmQuaternion& val);

		void 	SampleFrame(float frame,xmQuaternion& val) const;

		void	DbgDump() const;
	};




	//------------------------------------------------------------------------------
	//xmVector3Track
	//------------------------------------------------------------------------------

// 	inline xmVector3Track::xmVector3Track()
// 	{
// 
// 	}

	inline void	xmVector3Track::Clear()
	{
		m_arrFrame.clear();
		m_arrValue.clear();
	}

	inline void	xmVector3Track::Clone(const xmVector3Track* pTrack)
	{
		m_arrFrame = pTrack->m_arrFrame;
		m_arrValue = pTrack->m_arrValue;
	}

	inline void xmVector3Track::SetKeyNumber( xmUint keyNum )
	{
		m_arrFrame.resize(keyNum);
		m_arrValue.resize(keyNum);

	}

	inline xmUint xmVector3Track::GetKeyNumber() const
	{
		return (xmUint)m_arrValue.size();

	}

	inline void xmVector3Track::SetKeyFrame( xmUint key,xmUint frame )
	{
		m_arrFrame[key] = frame;
	}

	inline xmUint	xmVector3Track::GetKeyFrame(xmUint nKeyInd) const
	{
		return m_arrFrame[nKeyInd];
	}


	inline xmUint xmVector3Track::GetFrameNumber() const
	{
		return m_arrFrame.size() > 0 ? m_arrFrame[m_arrFrame.size()-1] + 1 : 0;
	}

	inline void xmVector3Track::SetKey( xmUint key,xmUint frame,const D3DXVECTOR3& val )
	{
		m_arrValue[key]		= val;
		m_arrFrame[key]	= frame;
	}


	inline void xmVector3Track::SetKeyValue( xmUint key,const D3DXVECTOR3& val )
	{
		m_arrValue[key] = val;
	}

	inline const D3DXVECTOR3& xmVector3Track::GetKeyValue(xmUint nKeyIndex)const
	{
		return m_arrValue[nKeyIndex];
	}

	inline void xmVector3Track::Pushback(xmUint frame,const D3DXVECTOR3& val)
	{
		m_arrValue.push_back(val);
		m_arrFrame.push_back(frame);
	}

	inline void xmVector3Track::SampleFrame( float frame,D3DXVECTOR3& val ) const
	{
		xmUint key0;
		xmUint key1;
		float factor; 

		DecomposeFrame(frame,m_arrFrame,key0,key1,factor);
		D3DXVec3Lerp(&val,&m_arrValue[key0],&m_arrValue[key1],factor);

	}

	inline void	xmVector3Track::DbgDump() const
	{
		//S3ALog("Total Key %u ",m_arrValue.size());
		for (xmUint keyCnt = 0; keyCnt < m_arrValue.size(); ++keyCnt)
		{
			//S3ALog(" %d(%.5f,%.5f,%.5f)",m_arrFrame[keyCnt],m_arrValue[keyCnt].x,m_arrValue[keyCnt].y,m_arrValue[keyCnt].z);
		}
	}


	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------


	inline xmQuaternionTrack::xmQuaternionTrack()
	{

	}

	inline void	xmQuaternionTrack::Clone(const xmQuaternionTrack* pTrack)
	{
		m_arrFrame = pTrack->m_arrFrame;
		m_arrValue = pTrack->m_arrValue;
	}

	inline void	xmQuaternionTrack::Clear()
	{
		m_arrFrame.clear();
		m_arrValue.clear();
	}

	inline void xmQuaternionTrack::SetKeyNumber( xmUint keyNum )
	{
		m_arrFrame.resize(keyNum);
		m_arrValue.resize(keyNum);

	}

	inline xmUint xmQuaternionTrack::GetKeyNumber() const
	{
		return (xmUint)m_arrValue.size();

	}

	inline void xmQuaternionTrack::SetKeyFrame( xmUint key,xmUint frame )
	{
		m_arrFrame[key] = frame;
	}

	inline xmUint	xmQuaternionTrack::GetKeyFrame(xmUint nKeyInd) const
	{
		return m_arrFrame[nKeyInd];
	}

	inline xmUint xmQuaternionTrack::GetFrameNumber() const
	{
		return m_arrFrame.size() > 0 ? m_arrFrame[m_arrFrame.size()-1]+1 : 0;
	}


	inline void xmQuaternionTrack::SetKey( xmUint key,xmUint frame,const xmQuaternion& val )
	{
		m_arrValue[key]		= val;
		m_arrFrame[key]	= frame;
	}

	inline const xmQuaternion& xmQuaternionTrack::GetKeyValue( xmUint nKeyIndex )const
	{
		return m_arrValue[nKeyIndex];
	}

	inline void xmQuaternionTrack::SetKeyValue( xmUint key,const xmQuaternion& val )
	{
		m_arrValue[key] = val;
	}

	inline void xmQuaternionTrack::Pushback( xmUint frame,const xmQuaternion& val )
	{
		m_arrValue.push_back(val);
		m_arrFrame.push_back(frame);
	}

	inline void xmQuaternionTrack::SampleFrame( float frame,xmQuaternion& val ) const
	{
		xmUint key0;
		xmUint key1;
		float factor; 
		DecomposeFrame(frame,m_arrFrame,key0,key1,factor);
//#ifdef S3A_TRACK_SAMPLE_USE_SLERP
		D3DXQuaternionSlerp(&val,&m_arrValue[key0],&m_arrValue[key1],factor);
//#else
//		maQuaternionLerp(&val,&m_arrValue[key0],&m_arrValue[key1],factor);
//#endif
	}

	inline void	xmQuaternionTrack::DbgDump() const
	{
		//S3ALog("Total Key %u ",m_arrValue.size());

		for (xmUint keyCnt = 0; keyCnt < m_arrValue.size(); ++keyCnt)
		{
			//S3ALog(" %d(%.5f,%.5f,%.5f,%.5f)",m_arrFrame[keyCnt],m_arrValue[keyCnt].x,m_arrValue[keyCnt].y,m_arrValue[keyCnt].z,m_arrValue[keyCnt].w);
		}
	}
//}


#endif