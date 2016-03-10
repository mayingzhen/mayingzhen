#include "AnimationDataCover.h"
#include "Track.h"

namespace ma
{
	bool Animation::ConverteAnimDataParentToLocalSpace(const Skeleton* pSkel)
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

			Vector3Track& scaleTrack = m_arrScaleTrack[i];
			QuaternionTrack& rotTrack = m_arrRotTrack[i];
			Vector3Track& posTrack = m_arrPosTrack[i];

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

	bool Animation::ConverteAnimDataObjectToLocalSpace(const Skeleton* pSkel)
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

			Vector3Track& scaleTrack = m_arrScaleTrack[i];
			QuaternionTrack& rotTrack = m_arrRotTrack[i];
			Vector3Track& posTrack = m_arrPosTrack[i];

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

	bool Animation::ConverteAnimDataLocalToParentSpace(const Skeleton* pSkeleton)
	{
		BoneMap boneMap;
		boneMap.Build(pSkeleton,this);

		const SkeletonPose* pRefPose = pSkeleton->GetResPose();
		if (pRefPose == NULL)
			return false;

		for (UINT i = 0; i < pSkeleton->GetBoneNumer(); ++i)
		{	
			const Transform& tsfBonePS = pRefPose->GetTransformPS(i);

			BoneIndex nMapID = boneMap.MapNode(i);
			if (Math::IsInvalidID<BoneIndex>(nMapID))
				continue;

			Vector3Track& scaleTrack = m_arrScaleTrack[nMapID];
			QuaternionTrack& rotTrack = m_arrRotTrack[nMapID];
			Vector3Track& posTrack = m_arrPosTrack[nMapID];

			UINT nFrameNumber = Math::Max(scaleTrack.m_arrFrame.back(),rotTrack.m_arrFrame.back());
			nFrameNumber = Math::Max(nFrameNumber,posTrack.m_arrFrame.back());
			nFrameNumber = nFrameNumber + 1;

			for (UINT nFrameCnt = 0; nFrameCnt < nFrameNumber; ++ nFrameCnt)
			{
				Transform tsfAnimPS;
				Transform tsfAnimLS;
				tsfAnimLS.m_vPos =  posTrack.m_arrValue[nFrameCnt];
				tsfAnimLS.m_qRot = rotTrack.m_arrValue[nFrameCnt];
				tsfAnimLS.m_vScale = scaleTrack.m_arrValue[nFrameCnt];
				TransformMul(&tsfAnimPS,&tsfBonePS,&tsfAnimLS);
				rotTrack.m_arrValue[nFrameCnt] = tsfAnimPS.m_qRot;
				posTrack.m_arrValue[nFrameCnt] = tsfAnimPS.m_vPos;	
			}
		}

		return true;
	}

}


