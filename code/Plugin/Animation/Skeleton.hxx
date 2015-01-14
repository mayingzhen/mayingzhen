#include "Skeleton.h"
#include "BoneSet.h"
#include "SkeletonPose.h"


#include "SkeletonSerializer.hxx"

namespace ma
{
	IMPL_OBJECT(Skeleton,Resource)

	Skeleton::Skeleton()
	{
		m_eCreateType = BIN_RES;
		m_refPose = new SkeletonPose;
	}

	Skeleton::~Skeleton()
	{
		SAFE_DELETE(m_refPose);
	}

	const char*	Skeleton::GetBoneNameByIndex(UINT uIndex) const
	{
		return m_arrBoneName[uIndex].c_str();
	}

	BoneIndex Skeleton::GetBoneIdByName(const char* pszBoneName) const
	{
		if (pszBoneName == NULL)
			return Math::InvalidID<BoneIndex>();

		for (UINT i = 0; i < m_arrBoneName.size(); ++i)
		{
			if (_stricmp(pszBoneName,m_arrBoneName[i].c_str()) == 0)
			{
				return i;
			}
		}

		return Math::InvalidID<BoneIndex>();
	}

	BoneIndex Skeleton::GetParentIndice(BoneIndex nBoneID) const
	{
		return m_arrParentIndice[nBoneID];
	}

	bool Skeleton::IsAncestorOf(BoneIndex nAncestorBoneID,BoneIndex nChildBoneID) const
	{
		for (BoneIndex nParentID = GetParentIndice(nChildBoneID); Math::IsValidID(nParentID); nParentID = GetParentIndice(nParentID))
		{
			if (nParentID == nAncestorBoneID)
			{
				return true;
			}
		}
		return false;
	}

	BoneSet* Skeleton::GetBoneSetByName(const char* pszBoneSetName) const
	{
		for (UINT i = 0; i < m_arrBoneSet.size(); ++i)
		{
			if ( _stricmp(pszBoneSetName,m_arrBoneSet[i]->GetBoneSetName()) == 0 )
				return m_arrBoneSet[i].get();
		}

		return NULL;
	}

	void Skeleton::AddBone(const char* pName,UINT nParentID,const Transform& tsfOS)
	{
		m_arrBoneName.push_back(pName);
		m_arrParentIndice.push_back(nParentID);
		m_arrTsfOS.push_back(tsfOS);
	}

	void Skeleton::InitUpLowerBoneSet(const char* pszSplitBone, const char* pszUpBody, const char* pszLowerBody,
		const char* pszFullBoyd,const char* pzEmptyBody)
	{
		if (!pszSplitBone || !pszUpBody || !pszLowerBody)
			return;

		BoneIndex nSplitBone = GetBoneIdByName(pszSplitBone);
		if ( Math::InvalidID<BoneIndex>() == nSplitBone )
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
		sl.Serialize(header,"SkeletonHeader");
		if (header.m_nIden != 'MAED')
			return;

		if (header.m_nVersion == EXP_SKEL_VER_CURRENT)
		{
			SerializeDataV0(sl,pszLable);
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
		//m_arrRefPosePS.resize(nBoneNumer);
		//m_arrRefPoseOS.resize(nBoneNumer);
		m_arrRefPoseOSInv.resize(nBoneNumer);

		for (UINT uBoneCunt = 0; uBoneCunt < nBoneNumer; ++uBoneCunt)
		{
			//MatrixFromTransform( &m_arrRefPoseOS[uBoneCunt], &m_refPose->GetTransformOS(uBoneCunt) );
			//MatrixFromTransform( &m_arrRefPosePS[uBoneCunt], &m_refPose->GetTransformPS(uBoneCunt) );
			Matrix4 matRefPoseOS;
			MatrixFromTransform( &matRefPoseOS, &m_refPose->GetTransformOS(uBoneCunt) );
			m_arrRefPoseOSInv[uBoneCunt] = matRefPoseOS.inverse();
		}

		InitUpLowerBoneSet();
	}

}