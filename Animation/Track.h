#ifndef __Track_H__
#define __Track_H__

// #include <Animation/Animation/KeyFrameHelper.h>
// #include <Animation/Animation/AnimationConfig.h>
// #include <Animation/Interface/ITrack.h>
// #include <Animation/Animation/KeyFrameHelper.h>

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
		//assert(keyFrame.size() > 0);
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


	class ITrack
	{
	public:

		virtual xmUint GetKeyNumber() const = 0;

		virtual xmUint GetKeyFrame(xmUint nKeyInd) const = 0;
	};

	template<class T>
	class DataTrack
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

	typedef DataTrack<float> xmFloatTrack;


	class D3DXVECTOR3Track : public ITrack
	{
	public:

		std::vector<xmUint>			m_arrFrame; //frame number of each key
		std::vector<D3DXVECTOR3>	m_arrValue;


		D3DXVECTOR3Track()
		{

		}

		void	Clear();

		void	Clone(const D3DXVECTOR3Track* pTrack);

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

	class xmQuaternionTrack : public ITrack
	{
	public:


		std::vector<xmUint>				m_arrFrame; //frame number of each key
		std::vector<D3DXQUATERNION>	m_arrValue;


		xmQuaternionTrack();

		void	Clone(const xmQuaternionTrack* pTrack);

		void	Clear();

		void	SetKeyNumber(xmUint keyNum);

		xmUint	GetKeyNumber() const;

		xmUint	GetFrameNumber() const;


		void 	SetKeyFrame(xmUint nKeyInd,xmUint nFrame);

		xmUint	GetKeyFrame(xmUint nKeyInd) const;


		void 					SetKeyValue(xmUint key,const D3DXQUATERNION& val);

		const D3DXQUATERNION&	GetKeyValue(xmUint nKeyIndex)const;


		void 					SetKey(xmUint key,xmUint frame,const D3DXQUATERNION& val);


		void 	Pushback(xmUint frame,const D3DXQUATERNION& val);

		void 	SampleFrame(float frame,D3DXQUATERNION& val) const;

		void	DbgDump() const;
	};




	//------------------------------------------------------------------------------
	//D3DXVECTOR3Track
	//------------------------------------------------------------------------------

// 	inline D3DXVECTOR3Track::D3DXVECTOR3Track()
// 	{
// 
// 	}

	inline void	D3DXVECTOR3Track::Clear()
	{
		m_arrFrame.clear();
		m_arrValue.clear();
	}

	inline void	D3DXVECTOR3Track::Clone(const D3DXVECTOR3Track* pTrack)
	{
		m_arrFrame = pTrack->m_arrFrame;
		m_arrValue = pTrack->m_arrValue;
	}

	inline void D3DXVECTOR3Track::SetKeyNumber( xmUint keyNum )
	{
		m_arrFrame.resize(keyNum);
		m_arrValue.resize(keyNum);

	}

	inline xmUint D3DXVECTOR3Track::GetKeyNumber() const
	{
		return (xmUint)m_arrValue.size();

	}

	inline void D3DXVECTOR3Track::SetKeyFrame( xmUint key,xmUint frame )
	{
		m_arrFrame[key] = frame;
	}

	inline xmUint	D3DXVECTOR3Track::GetKeyFrame(xmUint nKeyInd) const
	{
		return m_arrFrame[nKeyInd];
	}


	inline xmUint D3DXVECTOR3Track::GetFrameNumber() const
	{
		return m_arrFrame.size() > 0 ? m_arrFrame[m_arrFrame.size()-1] + 1 : 0;
	}

	inline void D3DXVECTOR3Track::SetKey( xmUint key,xmUint frame,const D3DXVECTOR3& val )
	{
		m_arrValue[key]		= val;
		m_arrFrame[key]	= frame;
	}


	inline void D3DXVECTOR3Track::SetKeyValue( xmUint key,const D3DXVECTOR3& val )
	{
		m_arrValue[key] = val;
	}

	inline const D3DXVECTOR3& D3DXVECTOR3Track::GetKeyValue(xmUint nKeyIndex)const
	{
		return m_arrValue[nKeyIndex];
	}

	inline void D3DXVECTOR3Track::Pushback(xmUint frame,const D3DXVECTOR3& val)
	{
		m_arrValue.push_back(val);
		m_arrFrame.push_back(frame);
	}

	inline void D3DXVECTOR3Track::SampleFrame( float frame,D3DXVECTOR3& val ) const
	{
		xmUint key0;
		xmUint key1;
		float factor; 

		DecomposeFrame(frame,m_arrFrame,key0,key1,factor);
		D3DXVec3Lerp(&val,&m_arrValue[key0],&m_arrValue[key1],factor);

	}

	inline void	D3DXVECTOR3Track::DbgDump() const
	{
		//Log("Total Key %u ",m_arrValue.size());
		for (xmUint keyCnt = 0; keyCnt < m_arrValue.size(); ++keyCnt)
		{
			//Log(" %d(%.5f,%.5f,%.5f)",m_arrFrame[keyCnt],m_arrValue[keyCnt].x,m_arrValue[keyCnt].y,m_arrValue[keyCnt].z);
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


	inline void xmQuaternionTrack::SetKey( xmUint key,xmUint frame,const D3DXQUATERNION& val )
	{
		m_arrValue[key]		= val;
		m_arrFrame[key]	= frame;
	}

	inline const D3DXQUATERNION& xmQuaternionTrack::GetKeyValue( xmUint nKeyIndex )const
	{
		return m_arrValue[nKeyIndex];
	}

	inline void xmQuaternionTrack::SetKeyValue( xmUint key,const D3DXQUATERNION& val )
	{
		m_arrValue[key] = val;
	}

	inline void xmQuaternionTrack::Pushback( xmUint frame,const D3DXQUATERNION& val )
	{
		m_arrValue.push_back(val);
		m_arrFrame.push_back(frame);
	}

	inline void xmQuaternionTrack::SampleFrame( float frame,D3DXQUATERNION& val ) const
	{
		xmUint key0;
		xmUint key1;
		float factor; 
		DecomposeFrame(frame,m_arrFrame,key0,key1,factor);
//#ifdef _TRACK_SAMPLE_USE_SLERP
		D3DXQuaternionSlerp(&val,&m_arrValue[key0],&m_arrValue[key1],factor);
//#else
//		maQuaternionLerp(&val,&m_arrValue[key0],&m_arrValue[key1],factor);
//#endif
	}

	inline void	xmQuaternionTrack::DbgDump() const
	{
		//Log("Total Key %u ",m_arrValue.size());

		for (xmUint keyCnt = 0; keyCnt < m_arrValue.size(); ++keyCnt)
		{
			//Log(" %d(%.5f,%.5f,%.5f,%.5f)",m_arrFrame[keyCnt],m_arrValue[keyCnt].x,m_arrValue[keyCnt].y,m_arrValue[keyCnt].z,m_arrValue[keyCnt].w);
		}
	}
//}


#endif