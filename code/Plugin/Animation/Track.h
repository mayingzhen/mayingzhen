#ifndef __Track_H__
#define __Track_H__


namespace ma
{
	class Vector3Track : public Referenced
	{
	public:
		void	Clear();

		void	Clone(const Vector3Track* pTrack);

		void	SetKeyNumber(uint32_t keyNum);

		uint32_t	GetKeyNumber() const;

		uint32_t	GetFrameNumber() const;

		void	SetKeyFrame(uint32_t nKeyInd,uint32_t nFrame);

		uint32_t	GetKeyFrame(uint32_t nKeyInd) const;

		void	SetKeyValue(uint32_t key,const Vector3& val);

		const Vector3&	GetKeyValue(uint32_t nKeyIndex)const;

		void	SetKey(uint32_t key, uint32_t frame, const Vector3& val);

		void	Pushback(uint32_t frame,const Vector3& val);

		void	SampleFrame(float frame,Vector3& val) const ;

		void	DbgDump() const;

	private:
		std::vector<uint32_t>		m_arrFrame; //frame number of each key
		std::vector<Vector3>	m_arrValue;

		friend class Animation;
	};

	RefPtr<Vector3Track> CreateVector3Track();

	class QuaternionTrack : public Referenced
	{
	public:
		void	Clone(const QuaternionTrack* pTrack);

		void	Clear();

		void	SetKeyNumber(uint32_t keyNum);

		uint32_t	GetKeyNumber() const;

		uint32_t	GetFrameNumber() const;

		void 	SetKeyFrame(uint32_t nKeyInd,uint32_t nFrame);

		uint32_t	GetKeyFrame(uint32_t nKeyInd) const;

		void 	SetKeyValue(uint32_t key,const Quaternion& val);

		const Quaternion&	GetKeyValue(uint32_t nKeyIndex)const;

		void 	SetKey(uint32_t key,uint32_t frame,const Quaternion& val);

		void 	Pushback(uint32_t frame,const Quaternion& val);

		void 	SampleFrame(float frame,Quaternion& val) const;

		void	DbgDump() const;

	private:
		std::vector<uint32_t>		m_arrFrame; //frame number of each key
		std::vector<Quaternion>	m_arrValue;

		friend class Animation;
	};

	RefPtr<QuaternionTrack> CreateQuaternionTrack();
}


#endif