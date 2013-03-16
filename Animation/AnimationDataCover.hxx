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
			maNodeTransform tsfBoneOSInv;
			const maNodeTransform& tsfBoneOS = pRefPose->GetTransformOS(i);
			maTransformInverse(&tsfBoneOSInv,&tsfBoneOS);

			Vector3TrackData& scaleTrack = arrScaleTrackPS[i];
			QuaternionTrackData& rotTrack = arrRotTrackPS[i];
			Vector3TrackData& posTrack = arrPosTrackPS[i];

			UINT nFrameNumber = maMax(scaleTrack.m_arrFrame.back(),rotTrack.m_arrFrame.back());
			nFrameNumber = maMax(nFrameNumber,posTrack.m_arrFrame.back());
			nFrameNumber = nFrameNumber + 1;

			for (UINT nFrameCnt = 0; nFrameCnt < nFrameNumber; ++ nFrameCnt)
			{
				maNodeTransform tsfAnimOS;
				maNodeTransform tsfAnimLS;
				tsfAnimOS.m_vPos =  posTrack.m_arrKey[nFrameCnt];
				tsfAnimOS.m_qRot = rotTrack.m_arrKey[nFrameCnt];
				tsfAnimOS.m_fScale = 1.0f;//scaleTrack.m_arrValue[nFrameCnt];
				maTransformMul(&tsfAnimLS,&tsfAnimOS,&tsfBoneOSInv);
				scaleTrack.m_arrKey[nFrameCnt] = D3DXVECTOR3(1.0f,1.0f,1.0f);//tsfAnimLS.m_fScale;
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
			maNodeTransform tsfBonePSInv;
			const maNodeTransform& tsfBonePS = pRefPose->GetTransformPS(i);
			maTransformInverse(&tsfBonePSInv,&tsfBonePS);

			Vector3TrackData& scaleTrack = arrScaleTrackPS[i];
			QuaternionTrackData& rotTrack = arrRotTrackPS[i];
			Vector3TrackData& posTrack = arrPosTrackPS[i];

			UINT nFrameNumber = maMax(scaleTrack.m_arrFrame.back(),rotTrack.m_arrFrame.back());
			nFrameNumber = maMax(nFrameNumber,posTrack.m_arrFrame.back());
			nFrameNumber = nFrameNumber + 1;

			for (UINT nFrameCnt = 0; nFrameCnt < nFrameNumber; ++ nFrameCnt)
			{
				maNodeTransform tsfAnimPS;
				maNodeTransform tsfAnimLS;
				tsfAnimPS.m_vPos =  posTrack.m_arrKey[nFrameCnt];
				tsfAnimPS.m_qRot = rotTrack.m_arrKey[nFrameCnt];
				tsfAnimPS.m_fScale = scaleTrack.m_arrKey[nFrameCnt].x;
				maTransformMul(&tsfAnimLS,&tsfAnimPS,&tsfBonePSInv);
				scaleTrack.m_arrKey[nFrameCnt] = D3DXVECTOR3(tsfAnimLS.m_fScale,tsfAnimLS.m_fScale,tsfAnimLS.m_fScale);
				rotTrack.m_arrKey[nFrameCnt] = tsfAnimLS.m_qRot;
				posTrack.m_arrKey[nFrameCnt] = tsfAnimLS.m_vPos;	
			}
		}

		return true;

	}

	bool ConverteAnimDataParentToLocalSpaceAnimation(AnimationData* pAniData,Skeleton* pSkel)
	{
		if (pSkel == NULL || pAniData == NULL)
			return false;

		bool res =  ConverteAnimDataParentToLocalSpaceAnimation(
			pAniData->m_arrTransfTrackName,
			pAniData->m_arrScaleTrack, 
			pAniData->m_arrRotTrack, 
			pAniData->m_arrPosTrack, 
			pSkel);

		pAniData->Save();

		return res;
	}

	bool ConverteAnimDataObjectToLocalSpaceAnimation(AnimationData* pAniData,Skeleton* pSkel)
	{
		if (pSkel == NULL || pAniData == NULL)
			return false;

		return ConverteAnimDataObjectToLocalSpaceAnimation(
			pAniData->m_arrTransfTrackName,
			pAniData->m_arrScaleTrack, 
			pAniData->m_arrRotTrack, 
			pAniData->m_arrPosTrack, 
			pSkel);
	}

}


