#include "Animation/Skeleton.h"
#include "Animation/BoneSet.h"

#include "SkeletonSerializer.hxx"

namespace ma
{
	IMPL_OBJECT(Skeleton,Resource)

	Skeleton::Skeleton()
	{
		m_refPose = new SkeletonPose;
	}

	Skeleton::~Skeleton()
	{
		SAFE_DELETE(m_refPose);
	}


// 	bool Skeleton::InitWithData(const SkeletonData* pSkelData)
// 	{
// 		if (pSkelData == NULL)
// 			return false;
// 
// 		//m_pSkeleonData = pSkelData;
// 
// 		UINT nBoneNum = pSkelData->m_nBoneNum;
// 
// 		std::vector<NodeTransform> arrNodeOS;
// 		arrNodeOS.resize(nBoneNum);
// 		m_arrBoneName.resize(nBoneNum);
// 		m_arrParentInd.resize(nBoneNum);
// 		for (UINT nBoneCnt = 0; nBoneCnt < nBoneNum; ++nBoneCnt)
// 		{
// 			TransformSetIdentity(&arrNodeOS[nBoneCnt]);
// 
// 			arrNodeOS[nBoneCnt].m_vPos = pSkelData->m_arrPosOS[nBoneCnt];
// 			//arrNodeOS[nBoneCnt].m_qRot = pSkelData->m_arrRotOS[nBoneCnt];
// 			QuaternionNormalize(&arrNodeOS[nBoneCnt].m_qRot,&pSkelData->m_arrRotOS[nBoneCnt]);
// 			arrNodeOS[nBoneCnt].m_fScale = 1.0f;//pSkelData->m_arrScaleOS[nBoneCnt].x;
// 			m_arrBoneName[nBoneCnt] = pSkelData->m_arrBoneName[nBoneCnt];
// 			m_arrParentInd[nBoneCnt] = pSkelData->m_arrParentIndice[nBoneCnt];
// 		}
// 
// 		m_refPose->InitObjectSpace(arrNodeOS, m_arrParentInd);
// 
// 		m_arrRefPosePS.resize(nBoneNum);
// 		m_arrRefPoseOS.resize(nBoneNum);
// 		m_arrRefPoseOSInv.resize(nBoneNum);
// 
// 		for (UINT uBoneCunt = 0; uBoneCunt < nBoneNum; ++uBoneCunt)
// 		{
// 			MatrixFromTransform( &m_arrRefPoseOS[uBoneCunt], &m_refPose->GetTransformOS(uBoneCunt) );
// 			MatrixFromTransform( &m_arrRefPosePS[uBoneCunt], &m_refPose->GetTransformPS(uBoneCunt) );
// 			MatrixInverse( &m_arrRefPoseOSInv[uBoneCunt], NULL, &m_arrRefPoseOS[uBoneCunt] );
// 		}
// 
// 		return true;
// 	}


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
		return m_arrParentIndice[nBoneID];
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
			if ( _stricmp(pszBoneSetName,m_arrBoneSet[i]->GetBoneSetName()) == 0 )
				return m_arrBoneSet[i];
		}

		return NULL;
	}

	void Skeleton::InitUpLowerBoneSet(const char* pszSplitBone, const char* pszUpBody, const char* pszLowerBody,
		const char* pszFullBoyd,const char* pzEmptyBody)
	{
		if (!pszSplitBone || !pszUpBody || !pszLowerBody)
			return;

		BoneIndex nSplitBone = GetBoneIdByName(pszSplitBone);
		if ( InvalidID<BoneIndex>() == nSplitBone )
			return;

		BoneSet* pUperBody = new BoneSet(pszUpBody);
		BoneSet* pLowerBody = new BoneSet(pszLowerBody);
		BoneSet* pEmptyBody = new BoneSet(pzEmptyBody);
		BoneSet* pFullBody = new BoneSet(pszFullBoyd);
		m_arrBoneSet.push_back(pUperBody);
		m_arrBoneSet.push_back(pLowerBody);
		m_arrBoneSet.push_back(pEmptyBody);
		m_arrBoneSet.push_back(pFullBody);

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

			pFullBody->AddBoneInd(nBoneCnt);
		}
	}

	void Skeleton::Serialize(Serializer& sl, const char* pszLable)
	{
		SkeletonHeader header;
		sl.Serialize(header);
		if (header.m_nIden != 'MAED')
			return;

		if (header.m_nVersion == EXP_SKEL_VER_CURRENT)
		{
			SerializeDataV0(sl,*this,pszLable);
		}

		if (sl.IsReading())
		{
			InitResPose();
		}
	}

	void Skeleton::InitResPose()
	{
		m_refPose->InitObjectSpace(m_arrTsfOS, m_arrParentIndice);

		UINT nBoneNumer = m_arrBoneName.size();
		m_arrRefPosePS.resize(nBoneNumer);
		m_arrRefPoseOS.resize(nBoneNumer);
		m_arrRefPoseOSInv.resize(nBoneNumer);

		for (UINT uBoneCunt = 0; uBoneCunt < nBoneNumer; ++uBoneCunt)
		{
			MatrixFromTransform( &m_arrRefPoseOS[uBoneCunt], &m_refPose->GetTransformOS(uBoneCunt) );
			MatrixFromTransform( &m_arrRefPosePS[uBoneCunt], &m_refPose->GetTransformPS(uBoneCunt) );
			MatrixInverse( &m_arrRefPoseOSInv[uBoneCunt], NULL, &m_arrRefPoseOS[uBoneCunt] );
		}

		InitUpLowerBoneSet();
	}

}