#include "Animation/Animation.h"
#include "Animation/Track.h"
#include "Animation/AnimationTracks.h"
#include "Animation/Skeleton.h"

namespace ma
{
	Animation::Animation()
	{
		m_nBoneNum = 0;
		m_nSocketNum = 0;
		m_nFrameNumber = 0;
		m_nFrameNumberDirty = false;
		m_pRawTracks = NULL; 
	}

	Animation::~Animation()
	{

	}

	void Animation::SampleSingleTrackByFrame(maNodeTransform* pTSF, BoneIndex nTrackID,float fFrame) const
	{
		//assert(m_bInit);

		const AnimationTracks* pAnimTracks = m_pRawTracks;//GetActiveAnimationTracks();
		
 		D3DXVECTOR3Track* scalTrack = pAnimTracks->m_scale[nTrackID];
 		if (fFrame > scalTrack->m_arrFrame.size())
 		{
 			maTransformSetIdentity(pTSF);
 			return;
 		}

		D3DXVECTOR3 vLocalScale;
		pAnimTracks->m_scale[nTrackID]->SampleFrame(fFrame,vLocalScale);
		pTSF->m_fScale = ( vLocalScale.x + vLocalScale.y + vLocalScale.z ) / 3.0f;

		//pTSF->m_vLocalScale = pTSF->m_fScale > F_EPS ? (pTSF->m_vLocalScale / pTSF->m_fScale) : xmVec3Zero();


		pAnimTracks->m_rot[nTrackID]->SampleFrame(fFrame,pTSF->m_qRot);
		pAnimTracks->m_pos[nTrackID]->SampleFrame(fFrame,pTSF->m_vPos);

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

// 	bool Animation::Load(const char* pszPath)
// 	{
// 		AnimationData* pAniData = ResourceBuilder::LoadAnimationFromBinaryFile(pszPath);
// 		assert(pAniData);
// 		if (pAniData == NULL)
// 			return false;
// 
// 		Init(pAniData);
// 
// 		return true;
// 	}

	void Animation::InitWithData(AnimationData* pAniData)
	{
		if (pAniData == NULL)
			return;
	
		//assert(nBoneNum+nSocketNum==nTransfTrackNum);
		m_nBoneNum = pAniData->m_nBoneNum;
		UINT nTransfTrackNum = m_nBoneNum + m_nSocketNum;

		//m_name = "unknown";
		//m_nGlobalSkeletonID = skelGUID;

		AnimationTracks* pTracks = new AnimationTracks;

		pTracks->m_scale.resize(nTransfTrackNum);
		pTracks->m_rot.resize(nTransfTrackNum);
		pTracks->m_pos.resize(nTransfTrackNum);

		//if (bCompressed)
		//{
		//	m_pCompTracks = pTracks;
		//}else{
			m_pRawTracks = pTracks;
		//}

		//m_compParam.m_fDiffThreshold = fDiffThreshold;
		//m_compParam.m_fVarianceThreshold = fVariThreshold;

		m_arrTransfTrackName.resize(nTransfTrackNum);


		m_nFrameNumber = pAniData->m_nFrameNum;
		m_nFrameNumberDirty = false;

		for (UINT nTrackCnt = 0; nTrackCnt < nTransfTrackNum; ++nTrackCnt)
		{
			m_arrTransfTrackName[nTrackCnt] = pAniData->m_arrTransfTrackName[nTrackCnt];

			D3DXVECTOR3Track* pScaleTrack = new D3DXVECTOR3Track;
			xmQuaternionTrack* pRotTrack = new xmQuaternionTrack;
			D3DXVECTOR3Track* pPosTrack = new D3DXVECTOR3Track;

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
				D3DXQuaternionNormalize(&pRotTrack->m_arrValue[nKeyFrameCnt],&rotTrackData.m_arrKey[nKeyFrameCnt]);
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


			//AnimationHelper::BuildTrack(pScaleTrack,&arrScaleTrack[nTrackCnt]);
			//AnimationHelper::BuildTrack(pRotTrack,&arrRotTrack[nTrackCnt]);
			//AnimationHelper::BuildTrack(pPosTrack,&arrPosTrack[nTrackCnt]);

			pTracks->m_scale[nTrackCnt] = pScaleTrack;
			pTracks->m_rot[nTrackCnt] = pRotTrack;
			pTracks->m_pos[nTrackCnt] = pPosTrack;
		}

// 		UINT nFloatTrackNum = pAniData->m_arrFloatLSTrack.size();
// 		pTracks->m_float.resize(nFloatTrackNum);
// 		m_arrFloatTrackName.resize(nFloatTrackNum);
// 		for (UINT nFloatTrackCnt = 0; nFloatTrackCnt < nFloatTrackNum; ++nFloatTrackCnt)
// 		{
// 			xmFloatTrack* pFloatTrack = new xmFloatTrack;
// 			AnimationHelper::BuildTrack(pFloatTrack,&arrFloatTrack[nFloatTrackCnt]);
// 			pTracks->m_float[nFloatTrackCnt] = pFloatTrack;
// 		}


// 
// 		AnimationHelper::BuildTrack(&pTracks->m_rootMotionScale,pRootMotionScaleTrack);
// 		if (pTracks->m_rootMotionScale.m_arrFrame.size() == 0)
// 		{
// 			pTracks->m_rootMotionScale.m_arrFrame.push_back(0);
// 			pTracks->m_rootMotionScale.m_arrValue.push_back(D3DXVECTOR3(1.0f,1.0f,1.0f));
// 		}
// 
// 		AnimationHelper::BuildTrack(&pTracks->m_rootMotionRot,pRootMotionRotTrack);
// 		if (pTracks->m_rootMotionRot.m_arrFrame.size() == 0)
// 		{
// 			pTracks->m_rootMotionRot.m_arrFrame.push_back(0);
// 			pTracks->m_rootMotionRot.m_arrValue.push_back(D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f));
// 		}
// 
// 		AnimationHelper::BuildTrack(&pTracks->m_rootMotionPos,pRootMotionPosTrack);
// 		if(pTracks->m_rootMotionPos.m_arrFrame.size() == 0)
// 		{
// 			pTracks->m_rootMotionPos.m_arrFrame.push_back(0);
// 			pTracks->m_rootMotionPos.m_arrValue.push_back(D3DXVECTOR3(0.0f,0.0f,0.0f));
// 		}
// 
// 		m_strMaxFile = pszMaxFilename;
// 
// 
// 
// 		m_bInit = false;


	}

}
