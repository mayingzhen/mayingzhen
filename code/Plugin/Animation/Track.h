#ifndef __Track_H__
#define __Track_H__


namespace ma
{
	class Vector3Track : public Referenced
	{
	public:
		void	Clear();

		void	Clone(const Vector3Track* pTrack);

		void	SetKeyNumber(UINT keyNum);

		UINT	GetKeyNumber() const;

		UINT	GetFrameNumber() const;

		void	SetKeyFrame(UINT nKeyInd,UINT nFrame);

		UINT	GetKeyFrame(UINT nKeyInd) const;

		void	SetKeyValue(UINT key,const Vector3& val);

		const Vector3&	GetKeyValue(UINT nKeyIndex)const;

		void	SetKey(UINT key, UINT frame, const Vector3& val);

		void	Pushback(UINT frame,const Vector3& val);

		void	SampleFrame(float frame,Vector3& val) const ;

		void	DbgDump() const;

	private:
		std::vector<UINT>		m_arrFrame; //frame number of each key
		std::vector<Vector3>	m_arrValue;

		friend class Animation;
	};

	RefPtr<Vector3Track> CreateVector3Track();

	class QuaternionTrack : public Referenced
	{
	public:
		void	Clone(const QuaternionTrack* pTrack);

		void	Clear();

		void	SetKeyNumber(UINT keyNum);

		UINT	GetKeyNumber() const;

		UINT	GetFrameNumber() const;

		void 	SetKeyFrame(UINT nKeyInd,UINT nFrame);

		UINT	GetKeyFrame(UINT nKeyInd) const;

		void 	SetKeyValue(UINT key,const Quaternion& val);

		const Quaternion&	GetKeyValue(UINT nKeyIndex)const;

		void 	SetKey(UINT key,UINT frame,const Quaternion& val);

		void 	Pushback(UINT frame,const Quaternion& val);

		void 	SampleFrame(float frame,Quaternion& val) const;

		void	DbgDump() const;

	private:
		std::vector<UINT>		m_arrFrame; //frame number of each key
		std::vector<Quaternion>	m_arrValue;

		friend class Animation;
	};

	RefPtr<QuaternionTrack> CreateQuaternionTrack();
}


#endif