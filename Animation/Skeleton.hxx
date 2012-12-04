#include "Animation/Skeleton.h"

namespace ma
{
	Skeleton::Skeleton()
	{
		m_refPose = new NodePose;
	}

	Skeleton::~Skeleton()
	{
		SAFE_DELETE(m_refPose);
	}

	bool Skeleton::Load(const char* pszPath)
	{
		SkeletonData* pSkelData = ResourceBuilder::LoadSkeletonFromBinaryFile(pszPath);
		if (pSkelData == NULL)
			return false;
		
		Init(*pSkelData);

		return true;
	}

	void Skeleton::Init(const SkeletonData& skelData)
	{
		xmUint nBoneNum = skelData.m_nBoneNum;

		std::vector<xmNodeTransform> arrNodeOS;
		arrNodeOS.resize(nBoneNum);
		m_arrBoneName.resize(nBoneNum);
		m_arrParentInd.resize(nBoneNum);
		for (xmUint nBoneCnt = 0; nBoneCnt < nBoneNum; ++nBoneCnt)
		{
			arrNodeOS[nBoneCnt].m_vPos = skelData.m_arrPosOS[nBoneCnt];
			arrNodeOS[nBoneCnt].m_qRot = skelData.m_arrRotOS[nBoneCnt];
			m_arrBoneName[nBoneCnt] = skelData.m_arrBoneName[nBoneCnt];
			m_arrParentInd[nBoneCnt] = skelData.m_arrParentIndice[nBoneCnt];
		}

		m_refPose->InitObjectSpace(arrNodeOS, m_arrParentInd);

		m_arrRefPosePS.resize(nBoneNum);
		m_arrRefPoseOS.resize(nBoneNum);
		m_arrRefPoseOSInv.resize(nBoneNum);

		for (UINT uBoneCunt = 0; uBoneCunt < nBoneNum; ++uBoneCunt)
		{
			maMatrixFromTransform( &m_arrRefPoseOS[uBoneCunt], &m_refPose->GetTransformOS(uBoneCunt) );
			maMatrixFromTransform( &m_arrRefPosePS[uBoneCunt], &m_refPose->GetTransformPS(uBoneCunt) );
			D3DXMatrixInverse( &m_arrRefPoseOSInv[uBoneCunt], NULL, &m_arrRefPoseOS[uBoneCunt] );
		}

// 		for (xmUint nSocketCnt = 0; nSocketCnt < pSkelData->m_arrSocket.size(); ++nSocketCnt)
// 		{
// 			const S3AExpSocketData* pSocketData = &pSkelData->m_arrSocket[nSocketCnt];
// 			pSkel->AddSocket(pSocketData->m_strBoneName.c_str(),pSocketData->m_nBoneID,pSocketData->m_matTransformBS);
// 		}
// 
// 		pSkel->NotifySocketUpdate();

	}

}