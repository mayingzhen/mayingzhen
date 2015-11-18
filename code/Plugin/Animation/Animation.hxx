#include "Animation/Animation.h"
#include "Animation/Track.h"
#include "Animation/AnimationTracks.h"
#include "Animation/Skeleton.h"

//#include "AnimationSerializer.hxx"

namespace ma
{
	Animation::Animation()
	{
		m_nFrameNumber = 0;	
	}

	Animation::~Animation()
	{
	}

	void Animation::SampleSingleTrackByFrame(Transform* pTSF, BoneIndex nTrackID,float fFrame) const
	{
		profile_code();

		m_arrScaleTrack[nTrackID].SampleFrame(fFrame,pTSF->m_vScale);
		m_arrRotTrack[nTrackID].SampleFrame(fFrame,pTSF->m_qRot);
		m_arrPosTrack[nTrackID].SampleFrame(fFrame,pTSF->m_vPos);
	}

	UINT  Animation::GetTrackNumber() const
	{
		return m_arrTrackName.size();
	}

	const char*	 Animation::GetTrackNameByIndex(UINT index) const
	{
		return m_arrTrackName[index].c_str();
	}

	UINT Animation::GetTrackIndexByName(const char* pszName) const
	{
		if (pszName == NULL)
			return Math::InvalidID<BoneIndex>();

		for (UINT i = 0; i < m_arrTrackName.size(); ++i)
		{
			const char* pszTrackName = m_arrTrackName[i].c_str();
			if ( _stricmp(pszTrackName,pszName) == 0)
			{
				return i;
			}
		}

		return Math::InvalidID<BoneIndex>();
	}

	void Animation::AddTrack(const char* pName,const Vector3Track& scale,const QuaternionTrack& rot,const Vector3Track& pos)
	{
		m_arrTrackName.push_back(pName);
		m_arrScaleTrack.push_back(scale);
		m_arrRotTrack.push_back(rot);
		m_arrPosTrack.push_back(pos);

		UINT nFrame = pos.GetFrameNumber();
		m_nFrameNumber = m_nFrameNumber < nFrame ? nFrame : m_nFrameNumber;
	}

// 	void Animation::Serialize(Serializer& sl, const char* pszLable)
// 	{
// 		AnimationHeader header;
// 		sl.Serialize(header,"AnimationHeader");
// 		if (header.m_nIden != 'MAAD')
// 			return;
// 
// 		if (header.m_nVersion == EXP_ANIM_VER_INITIAL)
// 		{
// 			SerializeDataV0(sl,pszLable);
// 		}
// 	}

	RefPtr<Animation> CreateAnimation()
	{
		return new Animation;
	}

	RefPtr<Animation> CreateAnimation(const char* pszFile)
	{
		return g_pAnimationManager->CreateResource(pszFile);
	}

	ResourceSystem<Animation>* g_pAnimationManager = NULL;
}
