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

	void Animation::SampleSingleTrackByFrame(NodeTransform* pTSF, BoneIndex nTrackID,float fFrame) const
	{
		profile_code();

		//Vector3 vLocalScale;
		//m_arrScaleTrack[nTrackID].SampleFrame(fFrame,vLocalScale);
		//pTSF->m_fScale = ( vLocalScale.x + vLocalScale.y + vLocalScale.z ) / 3.0f;
		pTSF->m_fScale = 1.0f;

		//pTSF->m_vLocalScale = pTSF->m_fScale > FEPS ? (pTSF->m_vLocalScale / pTSF->m_fScale) : Vec3Zero();

		m_arrRotTrack[nTrackID].SampleFrame(fFrame,pTSF->m_qRot);
		m_arrPosTrack[nTrackID].SampleFrame(fFrame,pTSF->m_vPos);
	}

	UINT Animation::GetTransfTrackIndexByName(const char* pszName)
	{
		if (pszName == NULL)
			return InvalidID<UINT>();

		for (UINT i = 0; i < m_arrTransfTrackName.size(); ++i)
		{
			const char* pszTrackName = m_arrTransfTrackName[i].c_str();
			if ( _stricmp(pszTrackName,pszName) == 0)
			{
				return i;
			}
		}

		return InvalidID<UINT>();
	}


	void Animation::Serialize(Serializer& sl, const char* pszLable)
	{
		AnimationHeader header;
		sl.Serialize(header);
		if (header.m_nIden != 'MAAD')
			return;

		if (header.m_nVersion == EXP_ANIM_VER_INITIAL)
		{
			SerializeDataV0(sl,*this,pszLable);
		}
	}
}
