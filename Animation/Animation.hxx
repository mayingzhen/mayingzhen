#include "Animation/Animation.h"
#include "Animation/Track.h"
#include "Animation/AnimationTracks.h"
#include "Animation/Skeleton.h"

namespace ma
{

	bool ConverteAnimDataParentToLocalSpaceAnimation(
		 const std::vector<std::string>& arrTrackName,
		 std::vector<xmVector3Track*>& arrScaleTrackPS,
		 std::vector<xmQuaternionTrack*>& arrRotTrackPS,
		 std::vector<xmVector3Track*>& arrPosTrackPS,
		 Skeleton* pSkel)
	{

		if (pSkel == NULL)
			return false;

		const NodePose* pRefPose = pSkel->GetResPose();
		if (pRefPose == NULL)
			return false;

		for (UINT i = 0; i < pSkel->GetBoneNumer(); ++i)
		{
			maNodeTransform tsfBonePSInv;
			const maNodeTransform& tsfBonePS = pRefPose->GetTransformPS(i);
			maTransformInverse(&tsfBonePSInv,&tsfBonePS);

			xmVector3Track& scaleTrack = *arrScaleTrackPS[i];
			xmQuaternionTrack& rotTrack = *arrRotTrackPS[i];
			xmVector3Track& posTrack = *arrPosTrackPS[i];

			xmUint nFrameNumber = maMax(scaleTrack.m_arrFrame.back(),rotTrack.m_arrFrame.back());
			nFrameNumber = maMax(nFrameNumber,posTrack.m_arrFrame.back());
			nFrameNumber = nFrameNumber + 1;

			for (xmUint nFrameCnt = 0; nFrameCnt < nFrameNumber; ++ nFrameCnt)
			{
				maNodeTransform tsfAnimPS;
				maNodeTransform tsfAnimLS;
				tsfAnimPS.m_vPos =  posTrack.m_arrValue[nFrameCnt];
				tsfAnimPS.m_qRot = rotTrack.m_arrValue[nFrameCnt];
				tsfAnimPS.m_fScale = 1.0f;//scaleTrack.m_arrValue[nFrameCnt];
				maTransfromMul(&tsfAnimLS,&tsfAnimPS,&tsfBonePSInv);
				scaleTrack.m_arrValue[nFrameCnt] = D3DXVECTOR3(1.0f,1.0f,1.0f);//tsfAnimLS.m_fScale;
				rotTrack.m_arrValue[nFrameCnt] = tsfAnimLS.m_qRot;
				posTrack.m_arrValue[nFrameCnt] = tsfAnimLS.m_vPos;	
			}
		}
		
		return true;

	}

	bool Animation::ConverteAnimDataParentToLocalSpaceAnimation(Skeleton* pSkel)
	{
		if (pSkel == NULL)
			return false;

		return ma::ConverteAnimDataParentToLocalSpaceAnimation(
			m_arrTransfTrackName,
			m_pRawTracks->m_scale,
			m_pRawTracks->m_rot,
			m_pRawTracks->m_pos,
			pSkel);
	}





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

	void Animation::SampleSingleTrackByFrame(xmNodeTransform* pTSF, BoneIndex nTrackID,float fFrame) const
	{
		//S3ASSERT(m_bInit);

		const AnimationTracks* pAnimTracks = m_pRawTracks;//GetActiveAnimationTracks();

		xmVector3 vLocalScale;
		pAnimTracks->m_scale[nTrackID]->SampleFrame(fFrame,vLocalScale);
		pTSF->m_fScale = ( fabsf(vLocalScale.x) + fabsf(vLocalScale.y) + fabsf(vLocalScale.z) ) / 3.0f;

		//pTSF->m_vLocalScale = pTSF->m_fScale > xm_EPS ? (pTSF->m_vLocalScale / pTSF->m_fScale) : xmVec3Zero();


		pAnimTracks->m_rot[nTrackID]->SampleFrame(fFrame,pTSF->m_qRot);
		pAnimTracks->m_pos[nTrackID]->SampleFrame(fFrame,pTSF->m_vPos);

	}

	void Animation::SampleAndAddSingleTrackByFrame(xmNodeTransform* pTSF,BoneIndex nTrackID,float fWeight,float fFrame) const
	{
		xmNodeTransform tsfLS;
		SampleSingleTrackByFrame(&tsfLS,nTrackID,fFrame);

		pTSF->m_fScale += tsfLS.m_fScale * fWeight;
		//pTSF->m_vLocalScale += tsfLS.m_vLocalScale * fWeight;

		maQuaternionMad(&pTSF->m_qRot,&pTSF->m_qRot,&tsfLS.m_qRot,fWeight);

		pTSF->m_vPos += tsfLS.m_vPos * fWeight;
	}

	bool Animation::Load(const char* pszPath)
	{
		AnimationData* pAniData = ResourceBuilder::LoadAnimationFromBinaryFile(pszPath);
		assert(pAniData);
		if (pAniData == NULL)
			return false;

		Init(pAniData);

		return true;
	}

	void Animation::Init(AnimationData* pAniData)
	{
		if (pAniData == NULL)
			return;
	
		//S3ASSERT(nBoneNum+nSocketNum==nTransfTrackNum);
		m_nBoneNum = pAniData->m_nBoneNum;
		m_nSocketNum = pAniData->m_nSocketNum;
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

		for (xmUint nTrackCnt = 0; nTrackCnt < nTransfTrackNum; ++nTrackCnt)
		{
			m_arrTransfTrackName[nTrackCnt] = pAniData->m_arrTransfTrackName[nTrackCnt];

			xmVector3Track* pScaleTrack = new xmVector3Track;
			xmQuaternionTrack* pRotTrack = new xmQuaternionTrack;
			xmVector3Track* pPosTrack = new xmVector3Track;

			Vector3TrackData& scaleTrackData = pAniData->m_arrScaleTrack[nTrackCnt];
			QuaternionTrackData& rotTrackData = pAniData->m_arrRotTrack[nTrackCnt];
			Vector3TrackData& posTrackData = pAniData->m_arrPosTrack[nTrackCnt];

			UINT nKeyFrameNum = scaleTrackData.m_arrFrame.size();
			pScaleTrack->m_arrFrame.resize(nKeyFrameNum);
			pScaleTrack->m_arrValue.resize(nKeyFrameNum);
			for (xmUint nKeyFrameCnt = 0; nKeyFrameCnt < nKeyFrameNum; ++nKeyFrameCnt)
			{
				pScaleTrack->m_arrFrame[nKeyFrameCnt] = scaleTrackData.m_arrFrame[nKeyFrameCnt];
				pScaleTrack->m_arrValue[nKeyFrameCnt] = scaleTrackData.m_arrKey[nKeyFrameCnt];
			}

			nKeyFrameNum = rotTrackData.m_arrFrame.size();
			pRotTrack->m_arrFrame.resize(nKeyFrameNum);
			pRotTrack->m_arrValue.resize(nKeyFrameNum);
			for (xmUint nKeyFrameCnt = 0; nKeyFrameCnt < nKeyFrameNum; ++nKeyFrameCnt)
			{
				pRotTrack->m_arrFrame[nKeyFrameCnt] = rotTrackData.m_arrFrame[nKeyFrameCnt];
				D3DXQuaternionNormalize(&pRotTrack->m_arrValue[nKeyFrameCnt],&rotTrackData.m_arrKey[nKeyFrameCnt]);
				//pRotTrack->m_arrValue[nKeyFrameCnt] = rotTrackData.m_arrKey[nKeyFrameCnt];
			}
			
			nKeyFrameNum = posTrackData.m_arrFrame.size();
			pPosTrack->m_arrFrame.resize(nKeyFrameNum);
			pPosTrack->m_arrValue.resize(nKeyFrameNum);
			for (xmUint nKeyFrameCnt = 0; nKeyFrameCnt < nKeyFrameNum; ++nKeyFrameCnt)
			{
				pPosTrack->m_arrFrame[nKeyFrameCnt] = posTrackData.m_arrFrame[nKeyFrameCnt];
				pPosTrack->m_arrValue[nKeyFrameCnt] = posTrackData.m_arrKey[nKeyFrameCnt];
			}


			//S3AAnimationHelper::BuildTrack(pScaleTrack,&arrScaleTrack[nTrackCnt]);
			//S3AAnimationHelper::BuildTrack(pRotTrack,&arrRotTrack[nTrackCnt]);
			//S3AAnimationHelper::BuildTrack(pPosTrack,&arrPosTrack[nTrackCnt]);

			pTracks->m_scale[nTrackCnt] = pScaleTrack;
			pTracks->m_rot[nTrackCnt] = pRotTrack;
			pTracks->m_pos[nTrackCnt] = pPosTrack;
		}

// 		UINT nFloatTrackNum = pAniData->m_arrFloatLSTrack.size();
// 		pTracks->m_float.resize(nFloatTrackNum);
// 		m_arrFloatTrackName.resize(nFloatTrackNum);
// 		for (xmUint nFloatTrackCnt = 0; nFloatTrackCnt < nFloatTrackNum; ++nFloatTrackCnt)
// 		{
// 			xmFloatTrack* pFloatTrack = new xmFloatTrack;
// 			S3AAnimationHelper::BuildTrack(pFloatTrack,&arrFloatTrack[nFloatTrackCnt]);
// 			pTracks->m_float[nFloatTrackCnt] = pFloatTrack;
// 		}


// 
// 		S3AAnimationHelper::BuildTrack(&pTracks->m_rootMotionScale,pRootMotionScaleTrack);
// 		if (pTracks->m_rootMotionScale.m_arrFrame.size() == 0)
// 		{
// 			pTracks->m_rootMotionScale.m_arrFrame.push_back(0);
// 			pTracks->m_rootMotionScale.m_arrValue.push_back(xmVector3(1.0f,1.0f,1.0f));
// 		}
// 
// 		S3AAnimationHelper::BuildTrack(&pTracks->m_rootMotionRot,pRootMotionRotTrack);
// 		if (pTracks->m_rootMotionRot.m_arrFrame.size() == 0)
// 		{
// 			pTracks->m_rootMotionRot.m_arrFrame.push_back(0);
// 			pTracks->m_rootMotionRot.m_arrValue.push_back(xmQuaternion(0.0f,0.0f,0.0f,1.0f));
// 		}
// 
// 		S3AAnimationHelper::BuildTrack(&pTracks->m_rootMotionPos,pRootMotionPosTrack);
// 		if(pTracks->m_rootMotionPos.m_arrFrame.size() == 0)
// 		{
// 			pTracks->m_rootMotionPos.m_arrFrame.push_back(0);
// 			pTracks->m_rootMotionPos.m_arrValue.push_back(xmVector3(0.0f,0.0f,0.0f));
// 		}
// 
// 		m_strMaxFile = pszMaxFilename;
// 
// 
// 
// 		m_bInit = false;


	}

}
