#include "Animation/Skeleton.h"
#include "Animation/BoneSet.h"

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

// 	bool Skeleton::Load(const char* pszPath)
// 	{
// 		SkeletonData* pSkelData = ResourceBuilder::LoadSkeletonFromBinaryFile(pszPath);
// 		if (pSkelData == NULL)
// 			return false;
// 		
// 		Init(*pSkelData);
// 
// 		return true;
// 	}

	bool Skeleton::InitWithData(const SkeletonData& skelData)
	{
		xmUint nBoneNum = skelData.m_nBoneNum;

		std::vector<maNodeTransform> arrNodeOS;
		arrNodeOS.resize(nBoneNum);
		m_arrBoneName.resize(nBoneNum);
		m_arrParentInd.resize(nBoneNum);
		for (xmUint nBoneCnt = 0; nBoneCnt < nBoneNum; ++nBoneCnt)
		{
			arrNodeOS[nBoneCnt].m_vPos = skelData.m_arrPosOS[nBoneCnt];
			//arrNodeOS[nBoneCnt].m_qRot = ;
			D3DXQuaternionNormalize(&arrNodeOS[nBoneCnt].m_qRot,&skelData.m_arrRotOS[nBoneCnt]);
			arrNodeOS[nBoneCnt].m_fScale = skelData.m_arrScaleOS[nBoneCnt].x;
			m_arrBoneName[nBoneCnt] = skelData.m_arrBoneName[nBoneCnt];
			m_arrParentInd[nBoneCnt] = skelData.m_arrParentIndice[nBoneCnt];
		}

		m_refPose->InitObjectSpace(arrNodeOS, m_arrParentInd);
		//m_refPose->SyncParentSpace();
		//m_pose->SyncObjectSpace();

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
// 			const ExpSocketData* pSocketData = &pSkelData->m_arrSocket[nSocketCnt];
// 			pSkel->AddSocket(pSocketData->m_strBoneName.c_str(),pSocketData->m_nBoneID,pSocketData->m_matTransformBS);
// 		}
// 
// 		pSkel->NotifySocketUpdate();

		return true;

	}

	bool Skeleton::InitWithPSData(const SkeletonData& skelData)
	{
		xmUint nBoneNum = skelData.m_nBoneNum;

		std::vector<maNodeTransform> arrNodePS;
		arrNodePS.resize(nBoneNum);
		m_arrBoneName.resize(nBoneNum);
		m_arrParentInd.resize(nBoneNum);
		for (xmUint nBoneCnt = 0; nBoneCnt < nBoneNum; ++nBoneCnt)
		{
			arrNodePS[nBoneCnt].m_vPos = skelData.m_arrPosOS[nBoneCnt];
			//arrNodeOS[nBoneCnt].m_qRot = ;
			D3DXQuaternionNormalize(&arrNodePS[nBoneCnt].m_qRot,&skelData.m_arrRotOS[nBoneCnt]);
			arrNodePS[nBoneCnt].m_fScale = skelData.m_arrScaleOS[nBoneCnt].x;
			m_arrBoneName[nBoneCnt] = skelData.m_arrBoneName[nBoneCnt];
			m_arrParentInd[nBoneCnt] = skelData.m_arrParentIndice[nBoneCnt];
		}

		m_refPose->InitParentSpace(arrNodePS,m_arrParentInd);
		//m_refPose->InitObjectSpace(arrNodeOS, m_arrParentInd);
		//m_refPose->SyncParentSpace();
		//m_pose->SyncObjectSpace();

		m_arrRefPosePS.resize(nBoneNum);
		m_arrRefPoseOS.resize(nBoneNum);
		m_arrRefPoseOSInv.resize(nBoneNum);

		for (UINT uBoneCunt = 0; uBoneCunt < nBoneNum; ++uBoneCunt)
		{
			maMatrixFromTransform( &m_arrRefPoseOS[uBoneCunt], &m_refPose->GetTransformOS(uBoneCunt) );
			maMatrixFromTransform( &m_arrRefPosePS[uBoneCunt], &m_refPose->GetTransformPS(uBoneCunt) );
			D3DXMatrixInverse( &m_arrRefPoseOSInv[uBoneCunt], NULL, &m_arrRefPoseOS[uBoneCunt] );
		}

		return true;
	}

	BoneIndex Skeleton::GetBoneIdByName(const char* pszBoneName)
	{
		if (pszBoneName == NULL)
			return InvalidID<BoneIndex>();

		for (UINT i = 0; i < m_arrBoneName.size(); ++i)
		{
			if (_stricmp(pszBoneName,m_arrBoneName[i].c_str()) == 0)
			{
				return i;
			}
		}

		return InvalidID<BoneIndex>();
	}

	BoneIndex Skeleton::GetParentIndice(BoneIndex nBoneID)
	{
		return m_arrParentInd[nBoneID];
	}

	bool Skeleton::IsAncestorOf(BoneIndex nAncestorBoneID,BoneIndex nChildBoneID)
	{
		for (BoneIndex nParentID = GetParentIndice(nChildBoneID); IsValidID(nParentID); nParentID = GetParentIndice(nParentID))
		{
			if (nParentID == nAncestorBoneID)
			{
				return true;
			}
		}
		return false;
	}

	BoneSet* Skeleton::GetBoneSetByName(const char* pszBoneSetName)
	{
		for (UINT i = 0; i < m_arrBoneSet.size(); ++i)
		{
			if ( _strcmpi(pszBoneSetName,m_arrBoneSet[i]->GetBoneSetName()) == 0 )
				return m_arrBoneSet[i];
		}

		return NULL;
	}

	void Skeleton::InitUpLowerBoneSet(const char* pszSplitBone, const char* pszUpBody, const char* pszLowerBody)
	{
		if (!pszSplitBone || !pszUpBody || !pszLowerBody)
			return;

		BoneIndex nSplitBone = GetBoneIdByName(pszSplitBone);
		if ( InvalidID<BoneIndex>() == nSplitBone )
			return;

		BoneSet* pUperBody = new BoneSet(pszUpBody);
		BoneSet* pLowerBody = new BoneSet(pszLowerBody);
		m_arrBoneSet.push_back(pUperBody);
		m_arrBoneSet.push_back(pLowerBody);

		for (BoneIndex nBoneCnt = 0; nBoneCnt < m_arrBoneName.size(); ++nBoneCnt)
		{
			if (nSplitBone == nBoneCnt)
			{
				pUperBody->AddBoneInd(nBoneCnt);
			}
			else if ( IsAncestorOf(nSplitBone,nBoneCnt) )
			{
				pUperBody->AddBoneInd(nBoneCnt);
			}
			else
			{
				pLowerBody->AddBoneInd(nBoneCnt);
			}
		}
	}

}