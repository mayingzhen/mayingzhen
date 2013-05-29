#include "AnimationDataCover.h"

namespace ma
{
	bool ConverteAnimDataObjectToLocalSpaceAnimation(
		const std::vector<std::string>& arrTrackName,
		std::vector<Vector3TrackData>& arrScaleTrackPS,
		std::vector<QuaternionTrackData>& arrRotTrackPS,
		std::vector<Vector3TrackData>& arrPosTrackPS,
		Skeleton* pSkel)
	{

		if (pSkel == NULL)
			return false;

		const SkeletonPose* pRefPose = pSkel->GetResPose();
		if (pRefPose == NULL)
			return false;

		for (UINT i = 0; i < pSkel->GetBoneNumer(); ++i)
		{	
			NodeTransform tsfBoneOSInv;
			const NodeTransform& tsfBoneOS = pRefPose->GetTransformOS(i);
			TransformInverse(&tsfBoneOSInv,&tsfBoneOS);

			Vector3TrackData& scaleTrack = arrScaleTrackPS[i];
			QuaternionTrackData& rotTrack = arrRotTrackPS[i];
			Vector3TrackData& posTrack = arrPosTrackPS[i];

			UINT nFrameNumber = Max(scaleTrack.m_arrFrame.back(),rotTrack.m_arrFrame.back());
			nFrameNumber = Max(nFrameNumber,posTrack.m_arrFrame.back());
			nFrameNumber = nFrameNumber + 1;

			for (UINT nFrameCnt = 0; nFrameCnt < nFrameNumber; ++ nFrameCnt)
			{
				NodeTransform tsfAnimOS;
				NodeTransform tsfAnimLS;
				tsfAnimOS.m_vPos =  posTrack.m_arrKey[nFrameCnt];
				tsfAnimOS.m_qRot = rotTrack.m_arrKey[nFrameCnt];
				tsfAnimOS.m_fScale = 1.0f;//scaleTrack.m_arrValue[nFrameCnt];
				TransformMul(&tsfAnimLS,&tsfAnimOS,&tsfBoneOSInv);
				scaleTrack.m_arrKey[nFrameCnt] = Vector3(1.0f,1.0f,1.0f);//tsfAnimLS.m_fScale;
				rotTrack.m_arrKey[nFrameCnt] = tsfAnimLS.m_qRot;
				posTrack.m_arrKey[nFrameCnt] = tsfAnimLS.m_vPos;	
			}
		}

		return true;

	}


	bool ConverteAnimDataParentToLocalSpaceAnimation(
		const std::vector<std::string>& arrTrackName,
		std::vector<Vector3TrackData>& arrScaleTrackPS,
		std::vector<QuaternionTrackData>& arrRotTrackPS,
		std::vector<Vector3TrackData>& arrPosTrackPS,
		Skeleton* pSkel)
	{

		if (pSkel == NULL)
			return false;

		const SkeletonPose* pRefPose = pSkel->GetResPose();
		if (pRefPose == NULL)
			return false;

		for (UINT i = 0; i < pSkel->GetBoneNumer(); ++i)
		{	
			NodeTransform tsfBonePSInv;
			const NodeTransform& tsfBonePS = pRefPose->GetTransformPS(i);
			TransformInverse(&tsfBonePSInv,&tsfBonePS);

			Vector3TrackData& scaleTrack = arrScaleTrackPS[i];
			QuaternionTrackData& rotTrack = arrRotTrackPS[i];
			Vector3TrackData& posTrack = arrPosTrackPS[i];

			UINT nFrameNumber = Max(scaleTrack.m_arrFrame.back(),rotTrack.m_arrFrame.back());
			nFrameNumber = Max(nFrameNumber,posTrack.m_arrFrame.back());
			nFrameNumber = nFrameNumber + 1;

			for (UINT nFrameCnt = 0; nFrameCnt < nFrameNumber; ++ nFrameCnt)
			{
				NodeTransform tsfAnimPS;
				NodeTransform tsfAnimLS;
				tsfAnimPS.m_vPos =  posTrack.m_arrKey[nFrameCnt];
				tsfAnimPS.m_qRot = rotTrack.m_arrKey[nFrameCnt];
				tsfAnimPS.m_fScale = scaleTrack.m_arrKey[nFrameCnt].x;
				TransformMul(&tsfAnimLS,&tsfAnimPS,&tsfBonePSInv);
				scaleTrack.m_arrKey[nFrameCnt] = Vector3(tsfAnimLS.m_fScale,tsfAnimLS.m_fScale,tsfAnimLS.m_fScale);
				rotTrack.m_arrKey[nFrameCnt] = tsfAnimLS.m_qRot;
				posTrack.m_arrKey[nFrameCnt] = tsfAnimLS.m_vPos;	
			}
		}

		return true;

	}

	bool ConverteAnimDataParentToLocalSpaceAnimation(AnimationData* pAniData,const SkeletonData* pSkelData)
	{	
		if (pAniData == NULL)
			return false;

		Skeleton* pSkeleton = new Skeleton();
		pSkeleton->InitWithData(pSkelData);

		bool res =  ConverteAnimDataParentToLocalSpaceAnimation(
			pAniData->m_arrTransfTrackName,
			pAniData->m_arrScaleTrack, 
			pAniData->m_arrRotTrack, 
			pAniData->m_arrPosTrack, 
			pSkeleton);

		pAniData->Save();

		SAFE_DELETE(pSkeleton);

		return res;
	}

	bool ConverteAnimDataObjectToLocalSpaceAnimation(AnimationData* pAniData,const SkeletonData* pSkelData)
	{
		if (pAniData == NULL)
			return false;

		Skeleton* pSkeleton = new Skeleton();
		pSkeleton->InitWithData(pSkelData);

		bool res =  ConverteAnimDataObjectToLocalSpaceAnimation(
			pAniData->m_arrTransfTrackName,
			pAniData->m_arrScaleTrack, 
			pAniData->m_arrRotTrack, 
			pAniData->m_arrPosTrack, 
			pSkeleton);

		SAFE_DELETE(pSkeleton);

		return res;
	}

}


