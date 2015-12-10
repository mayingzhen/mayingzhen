#ifndef  _Animation__H__
#define  _Animation__H__


#include "Animation/Track.h"

namespace ma
{
	class Animation : public Resource
	{
		
	public: 
		Animation();

		~Animation();

		UINT							GetFrameNumber() {return m_nFrameNumber;}

		void							SampleSingleTrackByFrame(Transform* pTSF, BoneIndex nTrackID,float fFrame) const;

		void							Serialize(Serializer& sl, const char* pszLable);

		UINT							GetTrackNumber() const;
		const char*						GetTrackNameByIndex(UINT index) const;
		UINT							GetTrackIndexByName(const char* pszName) const;

		void							AddTrack(const char* pName,const Vector3Track& scale,const QuaternionTrack& rot,const Vector3Track& pos);

		bool							ConverteAnimDataObjectToLocalSpace(const Skeleton* pSkeleton);
		bool							ConverteAnimDataParentToLocalSpace(const Skeleton* pSkeleton);
		
		bool							SaveToFile(const char* pszFile);

	private:

		virtual bool					InitRes();

		void							ReadData();
		void							ReadS3Data();

	private:
		std::vector<std::string>		m_arrTrackName;
		UINT							m_nFrameNumber;
		std::vector<Vector3Track>		m_arrScaleTrack;
		std::vector<QuaternionTrack>	m_arrRotTrack;
		std::vector<Vector3Track>		m_arrPosTrack;
	};

	RefPtr<Animation> CreateAnimation();
	RefPtr<Animation> CreateAnimation(const char* pszFile);

	extern ResourceSystem<Animation>* g_pAnimationManager;
}

#endif
