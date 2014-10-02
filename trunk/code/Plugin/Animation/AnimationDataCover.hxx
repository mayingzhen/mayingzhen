#include "AnimationDataCover.h"
#include "Track.h"

namespace ma
{
	bool ConverteAnimDataObjectToLocalSpaceAnimation(
		std::vector<Vector3Track>& arrScaleTrackPS,
		std::vector<QuaternionTrack>& arrRotTrackPS,
		std::vector<Vector3Track>& arrPosTrackPS,
		const Skeleton* pSkel)
	{

		if (pSkel == NULL)
			return false;

		const SkeletonPose* pRefPose = pSkel->GetResPose();
		if (pRefPose == NULL)
			return false;

		for (UINT i = 0; i < pSkel->GetBoneNumer(); ++i)
		{	
			Transform tsfBoneOSInv;
			const Transform& tsfBoneOS = pRefPose->GetTransformOS(i);
			TransformInverse(&tsfBoneOSInv,&tsfBoneOS);

			Vector3Track& scaleTrack = arrScaleTrackPS[i];
			QuaternionTrack& rotTrack = arrRotTrackPS[i];
			Vector3Track& posTrack = arrPosTrackPS[i];

			UINT nFrameNumber = Math::Max(scaleTrack.m_arrFrame.back(),rotTrack.m_arrFrame.back());
			nFrameNumber = Math::Max(nFrameNumber,posTrack.m_arrFrame.back());
			nFrameNumber = nFrameNumber + 1;

			for (UINT nFrameCnt = 0; nFrameCnt < nFrameNumber; ++ nFrameCnt)
			{
				Transform tsfAnimOS;
				Transform tsfAnimLS;
				tsfAnimOS.m_vPos =  posTrack.m_arrValue[nFrameCnt];
				tsfAnimOS.m_qRot = rotTrack.m_arrValue[nFrameCnt];
				tsfAnimOS.m_vScale = scaleTrack.m_arrValue[nFrameCnt];
				TransformMul(&tsfAnimLS,&tsfBoneOSInv,&tsfAnimOS);
				rotTrack.m_arrValue[nFrameCnt] = tsfAnimLS.m_qRot;
				posTrack.m_arrValue[nFrameCnt] = tsfAnimLS.m_vPos;	
			}
		}

		return true;

	}


	bool ConverteAnimDataParentToLocalSpaceAnimation(
		std::vector<Vector3Track>& arrScaleTrackPS,
		std::vector<QuaternionTrack>& arrRotTrackPS,
		std::vector<Vector3Track>& arrPosTrackPS,
		const Skeleton* pSkel)
	{

		if (pSkel == NULL)
			return false;

		const SkeletonPose* pRefPose = pSkel->GetResPose();
		if (pRefPose == NULL)
			return false;

		for (UINT i = 0; i < pSkel->GetBoneNumer(); ++i)
		{	
			Transform tsfBonePSInv;
			const Transform& tsfBonePS = pRefPose->GetTransformPS(i);
			TransformInverse(&tsfBonePSInv,&tsfBonePS);

			Vector3Track& scaleTrack = arrScaleTrackPS[i];
			QuaternionTrack& rotTrack = arrRotTrackPS[i];
			Vector3Track& posTrack = arrPosTrackPS[i];

			UINT nFrameNumber = Math::Max(scaleTrack.m_arrFrame.back(),rotTrack.m_arrFrame.back());
			nFrameNumber = Math::Max(nFrameNumber,posTrack.m_arrFrame.back());
			nFrameNumber = nFrameNumber + 1;

			for (UINT nFrameCnt = 0; nFrameCnt < nFrameNumber; ++ nFrameCnt)
			{
				Transform tsfAnimPS;
				Transform tsfAnimLS;
				tsfAnimPS.m_vPos =  posTrack.m_arrValue[nFrameCnt];
				tsfAnimPS.m_qRot = rotTrack.m_arrValue[nFrameCnt];
				tsfAnimPS.m_vScale = scaleTrack.m_arrValue[nFrameCnt];
				TransformMul(&tsfAnimLS,&tsfBonePSInv,&tsfAnimPS);
				scaleTrack.m_arrValue[nFrameCnt] = tsfAnimLS.m_vScale;
				rotTrack.m_arrValue[nFrameCnt] = tsfAnimLS.m_qRot;
				posTrack.m_arrValue[nFrameCnt] = tsfAnimLS.m_vPos;	
			}
		}

		return true;

	}

	bool ConverteAnimDataParentToLocalSpaceAnimation(Animation* pAniData,const Skeleton* pSkeleton)
	{	
		if (pAniData == NULL)
			return false;

		bool res =  ConverteAnimDataParentToLocalSpaceAnimation(
			pAniData->m_arrScaleTrack, 
			pAniData->m_arrRotTrack, 
			pAniData->m_arrPosTrack, 
			pSkeleton);

		return res;
	}

	bool ConverteAnimDataObjectToLocalSpaceAnimation(Animation* pAniData,const Skeleton* pSkeleton)
	{
		if (pAniData == NULL)
			return false;

		bool res =  ConverteAnimDataObjectToLocalSpaceAnimation(
			pAniData->m_arrScaleTrack, 
			pAniData->m_arrRotTrack, 
			pAniData->m_arrPosTrack, 
			pSkeleton);

		return res;
	}

}


