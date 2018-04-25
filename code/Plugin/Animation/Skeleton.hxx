#include "Skeleton.h"
#include "BoneSet.h"
#include "SkeletonPose.h"

namespace ma
{
	Skeleton::Skeleton()
	{
		m_refPose = new SkeletonPose;
	}

	Skeleton::~Skeleton()
	{
	}

	const char*	Skeleton::GetBoneNameByIndex(uint32_t uIndex) const
	{
		return m_arrBoneName[uIndex].c_str();
	}

	BoneIndex Skeleton::GetBoneIdByName(const char* pszBoneName) const
	{
		if (pszBoneName == NULL)
			return Math::InvalidID<BoneIndex>();

		for (uint32_t i = 0; i < m_arrBoneName.size(); ++i)
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
		for (uint32_t i = 0; i < m_arrBoneSet.size(); ++i)
		{
			if ( _stricmp(pszBoneSetName,m_arrBoneSet[i]->GetBoneSetName()) == 0 )
				return m_arrBoneSet[i].get();
		}

		return NULL;
	}

	void Skeleton::AddBone(const char* pName,uint32_t nParentID,const Transform& tsfOS)
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


	bool Skeleton::InitRes()
	{
		uint32_t nIden = m_pDataStream->ReadUInt();
		if (nIden == 'SKLT')
		{
			ReadDataV0();
		}
		else if (nIden == 'MSKE')
		{
			ReadDataV1();
		}

		InitResPose();

		return true;
	}

	void Skeleton::ReadDataV0()
	{	
		//uint32_t nIden = m_pDataStream->ReadUInt();
		uint32_t nVersion = m_pDataStream->ReadUInt();
		uint32_t nStringLen = m_pDataStream->ReadUInt();
		vector<char> vecChar;
		vecChar.resize(nStringLen);
		m_pDataStream->Read(&vecChar[0],nStringLen);	
		string strMaxFile = &vecChar[0];
		//if (nIden != 'S3MD')
		//	return ;

		char GID[16];
		m_pDataStream->Read(GID,16);

		uint32_t nBoneNum = m_pDataStream->ReadUInt();

		m_arrBoneName.resize(nBoneNum);
		m_arrParentIndice.resize(nBoneNum);
		m_arrTsfOS.resize(nBoneNum);

		uint32_t nBoneNameNum = m_pDataStream->ReadUInt();
		for (uint32_t i = 0; i < nBoneNameNum; ++i)
		{
			uint32_t nStringLen = m_pDataStream->ReadUInt();
			vector<char> vecChar;
			vecChar.resize(nStringLen);
			m_pDataStream->Read(&vecChar[0],nStringLen);
			m_arrBoneName[i] = &vecChar[0];
		}

		uint32_t nParentIDNum = m_pDataStream->ReadUInt();
		for (uint32_t i = 0; i < nParentIDNum; ++i)
		{
			m_arrParentIndice[i] = m_pDataStream->ReadUInt();
		}

		uint32_t nSacleOSNum = m_pDataStream->ReadUInt();
		for (uint32_t i = 0; i < nSacleOSNum; ++i)
		{
			m_arrTsfOS[i].m_vScale = m_pDataStream->ReadVector3();
		}

		uint32_t nRotOSNum = m_pDataStream->ReadUInt();
		for (uint32_t i = 0; i < nRotOSNum; ++i)
		{
			m_arrTsfOS[i].m_qRot = m_pDataStream->ReadQuaternion();
		}

		uint32_t nPosOSNum = m_pDataStream->ReadUInt();
		for (uint32_t i = 0; i < nPosOSNum; ++i)
		{
			m_arrTsfOS[i].m_vPos = m_pDataStream->ReadVector3();
		}

		// 		uint32_t nSocketNum = m_pDataStream->ReadUInt();
		// 		for (uint32_t i = 0; i < nSacleOSNum; ++i)
		// 		{
		// 			m_arrTsfOS[i].m_vPos = m_pDataStream->ReadVector3();
		// 		}

	}

	void Skeleton::ReadDataV1()
	{	
		//uint32_t nIden = m_pDataStream->ReadUInt();
		uint32_t nVersion = m_pDataStream->ReadUInt();

		uint32_t nBoneNum = m_pDataStream->ReadUInt();
		
		m_arrBoneName.resize(nBoneNum);
		m_arrParentIndice.resize(nBoneNum);
		m_arrTsfOS.resize(nBoneNum);

		for (uint32_t i = 0; i < nBoneNum; ++i)
		{
			m_arrBoneName[i] = m_pDataStream->ReadString();
		}

		m_pDataStream->Read(&m_arrParentIndice[0],sizeof(BoneIndex) * nBoneNum);
		m_pDataStream->Read(&m_arrTsfOS[0],sizeof(Transform) * nBoneNum);
	}

	bool Skeleton::SaveToFile(const char* pszFile)
	{
		RefPtr<FileStream> pSaveStream = CreateFileStream(pszFile);
		
		uint32_t nIden = 'MSKE';
		uint32_t nVersion = 0;

		pSaveStream->WriteUInt(nIden);
		pSaveStream->WriteUInt(nVersion);

		uint32_t nBoneNum = m_arrBoneName.size();

		pSaveStream->WriteUInt(nBoneNum);
		
		for (uint32_t i = 0; i < m_arrBoneName.size(); ++i)
		{
			pSaveStream->WriteString(m_arrBoneName[i]);
		}

		pSaveStream->Write(&m_arrParentIndice[0],sizeof(BoneIndex) * nBoneNum);
		pSaveStream->Write(&m_arrTsfOS[0],sizeof(Transform) * nBoneNum);

		return true;
	}

	void Skeleton::InitResPose()
	{
		m_refPose->InitObjectSpace(m_arrTsfOS, m_arrParentIndice);

		uint32_t nBoneNumer = m_arrBoneName.size();
		//m_arrRefPosePS.resize(nBoneNumer);
		//m_arrRefPoseOS.resize(nBoneNumer);
		m_arrRefPoseOSInv.resize(nBoneNumer);

		for (uint32_t uBoneCunt = 0; uBoneCunt < nBoneNumer; ++uBoneCunt)
		{
			//MatrixFromTransform( &m_arrRefPoseOS[uBoneCunt], &m_refPose->GetTransformOS(uBoneCunt) );
			//MatrixFromTransform( &m_arrRefPosePS[uBoneCunt], &m_refPose->GetTransformPS(uBoneCunt) );
			Matrix4 matRefPoseOS;
			MatrixFromTransform( &matRefPoseOS, &m_refPose->GetTransformOS(uBoneCunt) );
			m_arrRefPoseOSInv[uBoneCunt] = matRefPoseOS.inverse();
		}

		InitUpLowerBoneSet();
	}

	ResourceSystem<Skeleton>* g_pSkeletonManager = NULL;

	RefPtr<Skeleton> CreateSkeleton(const char* pszFile)
	{
		return g_pSkeletonManager->CreateResource(pszFile);
	}

	RefPtr<Skeleton> CreateSkeleton()
	{
		return new Skeleton();
	}

}
