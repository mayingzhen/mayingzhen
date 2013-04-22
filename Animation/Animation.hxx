#include "Animation/Animation.h"
#include "Animation/Track.h"
#include "Animation/AnimationTracks.h"
#include "Animation/Skeleton.h"

namespace ma
{
	Animation::Animation()
	{
		m_nBoneNum = 0;
		m_nFrameNumber = 0;
		m_nFrameNumberDirty = false;
		m_pTracks = NULL; 
	}

	Animation::~Animation()
	{

	}

	void Animation::SampleSingleTrackByFrame(NodeTransform* pTSF, BoneIndex nTrackID,float fFrame) const
	{
		//assert(m_bInit);
 		VECTOR3Track* scalTrack = m_pTracks->m_scale[nTrackID];
 		if (fFrame > scalTrack->m_arrFrame.size())
 		{
 			TransformSetIdentity(pTSF);
 			return;
 		}

		Vector3 vLocalScale;
		m_pTracks->m_scale[nTrackID]->SampleFrame(fFrame,vLocalScale);
		pTSF->m_fScale = ( vLocalScale.x + vLocalScale.y + vLocalScale.z ) / 3.0f;

		//pTSF->m_vLocalScale = pTSF->m_fScale > FEPS ? (pTSF->m_vLocalScale / pTSF->m_fScale) : Vec3Zero();

		m_pTracks->m_rot[nTrackID]->SampleFrame(fFrame,pTSF->m_qRot);
		m_pTracks->m_pos[nTrackID]->SampleFrame(fFrame,pTSF->m_vPos);

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

	void Animation::InitWithData(AnimationData* pAniData)
	{
		if (pAniData == NULL)
			return;
	
		//assert(nBoneNum+nSocketNum==nTransfTrackNum);
		m_nBoneNum = pAniData->m_nBoneNum;
		UINT nTransfTrackNum = m_nBoneNum /*+ m_nSocketNum*/;

		AnimationTracks* m_pTracks = new AnimationTracks;

		m_pTracks->m_scale.resize(nTransfTrackNum);
		m_pTracks->m_rot.resize(nTransfTrackNum);
		m_pTracks->m_pos.resize(nTransfTrackNum);

		m_arrTransfTrackName.resize(nTransfTrackNum);

		m_nFrameNumber = pAniData->m_nFrameNum;
		m_nFrameNumberDirty = false;

		for (UINT nTrackCnt = 0; nTrackCnt < nTransfTrackNum; ++nTrackCnt)
		{
			m_arrTransfTrackName[nTrackCnt] = pAniData->m_arrTransfTrackName[nTrackCnt];

			VECTOR3Track* pScaleTrack = new VECTOR3Track;
			QuaternionTrack* pRotTrack = new QuaternionTrack;
			VECTOR3Track* pPosTrack = new VECTOR3Track;

			Vector3TrackData& scaleTrackData = pAniData->m_arrScaleTrack[nTrackCnt];
			QuaternionTrackData& rotTrackData = pAniData->m_arrRotTrack[nTrackCnt];
			Vector3TrackData& posTrackData = pAniData->m_arrPosTrack[nTrackCnt];

			UINT nKeyFrameNum = scaleTrackData.m_arrFrame.size();
			pScaleTrack->m_arrFrame.resize(nKeyFrameNum);
			pScaleTrack->m_arrValue.resize(nKeyFrameNum);
			for (UINT nKeyFrameCnt = 0; nKeyFrameCnt < nKeyFrameNum; ++nKeyFrameCnt)
			{
				pScaleTrack->m_arrFrame[nKeyFrameCnt] = scaleTrackData.m_arrFrame[nKeyFrameCnt];
				pScaleTrack->m_arrValue[nKeyFrameCnt] = scaleTrackData.m_arrKey[nKeyFrameCnt];
			}

			nKeyFrameNum = rotTrackData.m_arrFrame.size();
			pRotTrack->m_arrFrame.resize(nKeyFrameNum);
			pRotTrack->m_arrValue.resize(nKeyFrameNum);
			for (UINT nKeyFrameCnt = 0; nKeyFrameCnt < nKeyFrameNum; ++nKeyFrameCnt)
			{
				pRotTrack->m_arrFrame[nKeyFrameCnt] = rotTrackData.m_arrFrame[nKeyFrameCnt];
				QuaternionNormalize(&pRotTrack->m_arrValue[nKeyFrameCnt],&rotTrackData.m_arrKey[nKeyFrameCnt]);
				//pRotTrack->m_arrValue[nKeyFrameCnt] = rotTrackData.m_arrKey[nKeyFrameCnt];
			}
			
			nKeyFrameNum = posTrackData.m_arrFrame.size();
			pPosTrack->m_arrFrame.resize(nKeyFrameNum);
			pPosTrack->m_arrValue.resize(nKeyFrameNum);
			for (UINT nKeyFrameCnt = 0; nKeyFrameCnt < nKeyFrameNum; ++nKeyFrameCnt)
			{
				pPosTrack->m_arrFrame[nKeyFrameCnt] = posTrackData.m_arrFrame[nKeyFrameCnt];
				pPosTrack->m_arrValue[nKeyFrameCnt] = posTrackData.m_arrKey[nKeyFrameCnt];
			}

			m_pTracks->m_scale[nTrackCnt] = pScaleTrack;
			m_pTracks->m_rot[nTrackCnt] = pRotTrack;
			m_pTracks->m_pos[nTrackCnt] = pPosTrack;
		}

	}

}
