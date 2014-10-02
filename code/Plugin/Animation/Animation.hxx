#include "Animation/Animation.h"
#include "Animation/Track.h"
#include "Animation/AnimationTracks.h"
#include "Animation/Skeleton.h"

#include "AnimationSerializer.hxx"

namespace ma
{
	IMPL_OBJECT(Animation,Resource)

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

	UINT Animation::GetTransfTrackIndexByName(const char* pszName)
	{
		if (pszName == NULL)
			return Math::InvalidID<BoneIndex>();

		for (UINT i = 0; i < m_arrTransfTrackName.size(); ++i)
		{
			const char* pszTrackName = m_arrTransfTrackName[i].c_str();
			if ( _stricmp(pszTrackName,pszName) == 0)
			{
				return i;
			}
		}

		return Math::InvalidID<BoneIndex>();
	}


	void Animation::Serialize(Serializer& sl, const char* pszLable)
	{
		AnimationHeader header;
		sl.Serialize(header,"AnimationHeader");
		if (header.m_nIden != 'MAAD')
			return;

		if (header.m_nVersion == EXP_ANIM_VER_INITIAL)
		{
			SerializeDataV0(sl,*this,pszLable);
		}
	}
}
