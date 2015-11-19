
namespace ma
{
	
	enum SkelVersion
	{
		EXP_SKEL_VER_INITIAL = 1,
		EXP_SKEL_VER_CURRENT = EXP_SKEL_VER_INITIAL,
	};

	struct SkeletonHeader
	{
		UINT m_nIden;
		UINT m_nVersion;

		SkeletonHeader()
		{
			m_nIden = 'MAED';
			m_nVersion = EXP_SKEL_VER_CURRENT;
		}
	};

	void Skeleton::ReadS3Data()
	{	
		//uint32 nIden = m_pDataStream->ReadUInt();
		uint32 nVersion = m_pDataStream->ReadUInt();
		uint32 nStringLen = m_pDataStream->ReadUInt();
		vector<char> vecChar;
		vecChar.resize(nStringLen);
		m_pDataStream->Read(&vecChar[0],nStringLen);	
		string strMaxFile = &vecChar[0];
		//if (nIden != 'S3MD')
		//	return ;

		char GID[16];
		m_pDataStream->Read(GID,16);

		uint32 nBoneNum = m_pDataStream->ReadUInt();

		m_arrBoneName.resize(nBoneNum);
		m_arrParentIndice.resize(nBoneNum);
		m_arrTsfOS.resize(nBoneNum);

		uint32 nBoneNameNum = m_pDataStream->ReadUInt();
		for (uint32 i = 0; i < nBoneNameNum; ++i)
		{
			uint32 nStringLen = m_pDataStream->ReadUInt();
			vector<char> vecChar;
			vecChar.resize(nStringLen);
			m_pDataStream->Read(&vecChar[0],nStringLen);
			m_arrBoneName[i] = &vecChar[0];
		}

		uint32 nParentIDNum = m_pDataStream->ReadUInt();
		for (uint32 i = 0; i < nParentIDNum; ++i)
		{
			m_arrParentIndice[i] = m_pDataStream->ReadUInt();
		}

		uint32 nSacleOSNum = m_pDataStream->ReadUInt();
		for (uint32 i = 0; i < nSacleOSNum; ++i)
		{
			m_arrTsfOS[i].m_vScale = m_pDataStream->ReadVector3();
		}
			
		uint32 nRotOSNum = m_pDataStream->ReadUInt();
		for (uint32 i = 0; i < nSacleOSNum; ++i)
		{
			m_arrTsfOS[i].m_qRot = m_pDataStream->ReadQuaternion();
		}

		uint32 nPosOSNum = m_pDataStream->ReadUInt();
		for (uint32 i = 0; i < nSacleOSNum; ++i)
		{
			m_arrTsfOS[i].m_vPos = m_pDataStream->ReadVector3();
		}

// 		uint32 nSocketNum = m_pDataStream->ReadUInt();
// 		for (uint32 i = 0; i < nSacleOSNum; ++i)
// 		{
// 			m_arrTsfOS[i].m_vPos = m_pDataStream->ReadVector3();
// 		}
	
	}

	void Skeleton::ReadData()
	{	
		SkeletonHeader header;
		//header.m_nIden = m_pDataStream->ReadUInt();
		header.m_nVersion = m_pDataStream->ReadUInt();
		//if (header.m_nIden != 'MAED')
		//	return;

		uint32 nBoneNum = m_pDataStream->ReadUInt();
		
		m_arrBoneName.resize(nBoneNum);
		for (uint32 i = 0; i < nBoneNum; ++i)
		{
			m_arrBoneName[i] = m_pDataStream->ReadString();
		}

		m_arrParentIndice.resize(nBoneNum);
		m_arrTsfOS.resize(nBoneNum);

		m_pDataStream->Read(&m_arrParentIndice[0],sizeof(uint32) * nBoneNum);
		m_pDataStream->Read(&m_arrTsfOS[0],sizeof(Transform)* nBoneNum);
	}
}

