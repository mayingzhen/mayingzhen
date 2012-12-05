#include "Animation/Animation.h"
#include "Animation/Track.h"

namespace ma
{

	AnimationTracks::AnimationTracks()
	{

	}

	AnimationTracks::~AnimationTracks()
	{
		Clear();
	}

	void AnimationTracks::Clear()
	{
// 		xmUint nTrackNum = m_pos.size();
// 		for (xmUint trackCnt = 0; trackCnt <nTrackNum; ++ trackCnt)
// 		{
// 			delete m_scale[trackCnt];
// 			delete m_rot[trackCnt];
// 			delete m_pos[trackCnt];
// 		}
// 		m_scale.clear();
// 		m_rot.clear();
// 		m_pos.clear();
// 
// 
// 		for (xmUint nFloatTrackCnt = 0; nFloatTrackCnt < m_float.size(); ++ nFloatTrackCnt)
// 		{
// 			delete m_float[nFloatTrackCnt];
// 		}
// 		m_float.clear();
// 
// 
// 		m_rootMotionScale.Clear();
// 		m_rootMotionRot.Clear();
// 		m_rootMotionPos.Clear();

	}

	void AnimationTracks::SetPositionTrack(xmUint nTrackInd,const xmVector3Track* pTrack)
	{
		//m_pos[nTrackInd]->Clone(pTrack);
	}


	void AnimationTracks::SetRotationTrack(xmUint nTrackInd,const xmQuaternionTrack* pTrack)
	{
		//m_rot[nTrackInd]->Clone(pTrack);
	}

	void AnimationTracks::SetScaleTrack(xmUint nTrackInd,const xmVector3Track* pTrack)
	{
		//m_scale[nTrackInd]->Clone(pTrack);
	}

	void AnimationTracks::Clone(AnimationTracks* pTracks) const
	{

// 		S3AArrayDeepCopy<S3AArray<xmVector3Track*>,xmVector3Track>(pTracks->m_scale,m_scale);
// 		S3AArrayDeepCopy<S3AArray<xmQuaternionTrack*>,xmQuaternionTrack>(pTracks->m_rot,m_rot);
// 		S3AArrayDeepCopy<S3AArray<xmVector3Track*>,xmVector3Track>(pTracks->m_pos,m_pos);
// 		S3AArrayDeepCopy<S3AArray<xmFloatTrack*>,xmFloatTrack>(pTracks->m_float,m_float);
// 
// 		pTracks->m_rootMotionPos = m_rootMotionPos;
// 		pTracks->m_rootMotionRot = m_rootMotionRot;
// 		pTracks->m_rootMotionScale = m_rootMotionScale;
	}

	xmUint AnimationTracks::ComputeDataSize() const
	{
// 		xmUint nScaleKeyFrameNum = GetKeyFrameNumber(m_scale);
// 		nScaleKeyFrameNum += (xmUint)m_rootMotionScale.m_arrValue.size();
// 
// 		xmUint nRotKeyFrameNum = GetKeyFrameNumber(m_rot);
// 		nRotKeyFrameNum += (xmUint)m_rootMotionRot.m_arrValue.size();
// 
// 		xmUint nPosKeyFrameNum = GetKeyFrameNumber(m_pos);
// 		nPosKeyFrameNum += (xmUint)m_rootMotionPos.m_arrValue.size();
// 
// 		xmUint nDataSize = nScaleKeyFrameNum*(sizeof(xmVector3)+sizeof(xmUint))
// 			+nPosKeyFrameNum*(sizeof(xmVector3)+sizeof(xmUint))
// 			+nRotKeyFrameNum*(sizeof(xmQuaternion)+sizeof(xmUint));
// 		return nDataSize;
		return 0;
	}



	Animation::Animation()
	{
		m_fLocalFrame = 0;	
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

	void Animation::AdvanceTime(float fTimeElapsed)
	{
		m_fLocalFrame += fTimeElapsed * 30.0f/*GetFrameRate()*/ * 1.0f/*m_fPlaySpeed*/;
		WrapLocalFrame();
	}

	void Animation::WrapLocalFrame()
	{
		if (m_fLocalFrame > m_nFrameNumber)
		{
			//if (m_playbackMode == S3L_PLAYBACK_LOOP)
			//{
				m_fLocalFrame = 0.0f;
			//}
			//else
			//{
			//	m_playerStatus = S3L_PLAYER_STOP;
			//}
		}
	}

	void Animation::EvaluateAnimation(std::vector<maNodeTransform>&m_arrTSFLS)
	{
		//m_arrTSFLS.resize(m_nBoneNum);
		for (xmUint uBoneId = 0; uBoneId < m_nBoneNum; ++uBoneId)
		{
			//maTransformSetIdentity(&m_arrTSFLS[uBoneId]);
			SampleAndAddSingleTrackByFrame(&m_arrTSFLS[uBoneId],uBoneId,1.0f,m_fLocalFrame);
		}
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
				pRotTrack->m_arrValue[nKeyFrameCnt] = rotTrackData.m_arrKey[nKeyFrameCnt];
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
