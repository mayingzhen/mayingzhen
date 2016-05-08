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

		UINT							GetTrackNumber() const;
		const char*						GetTrackNameByIndex(UINT index) const;
		UINT							GetTrackIndexByName(const char* pszName) const;

		void							AddTrack(const char* pName,const Vector3Track* pScale,const QuaternionTrack* pRot,const Vector3Track* pPos);

		bool							ConverteAnimDataObjectToLocalSpace(const Skeleton* pSkeleton);
		bool							ConverteAnimDataParentToLocalSpace(const Skeleton* pSkeleton);
		bool							ConverteAnimDataLocalToParentSpace(const Skeleton* pSkeleton);

		bool							Load(const char* pszFile, const char* pszSkeleton, const char* pszRefSkeleton);
		
		bool							SaveToFile(const char* pszFile);

	private:

		virtual bool					InitRes();

		void							ReadDataV1();
		void							ReadDataV0();

	private:
		std::vector<std::string>		m_arrTrackName;
		UINT							m_nFrameNumber;
		std::vector<Vector3Track>		m_arrScaleTrack;
		std::vector<QuaternionTrack>	m_arrRotTrack;
		std::vector<Vector3Track>		m_arrPosTrack;
		RefPtr<Skeleton>				m_pSkeleton;
		RefPtr<Skeleton>				m_pRefSkeleton;
		bool							m_bCompress;
	};

	RefPtr<Animation> CreateAnimation();
	RefPtr<Animation> CreateAnimation(const char* pszFile,const char* pszSkeletonFile, const char* pszRefSkeleton);
}

#endif
