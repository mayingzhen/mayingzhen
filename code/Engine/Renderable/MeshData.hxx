#include "MeshData.h"


namespace ma
{
	MeshData::MeshData()
	{
		m_nIndexType = 0;
		m_nVertexType = 0;
		m_pIndexBuffer = GetRenderDevice()->CreateIndexBuffer();
		m_pVertexBuffer = GetRenderDevice()->CreateVertexBuffer();
		m_pDeclaration = GetRenderDevice()->CreateVertexDeclaration();	
	}

	MeshData::~MeshData()
	{
	}

// 	SubMeshData* MeshData::AddSubMeshData()
// 	{
// 		SubMeshData* pSubMeshData = new SubMeshData();
// 		m_arrSubMesh.push_back(pSubMeshData);
// 		return pSubMeshData;
// 	}

// 	void MeshData::Improt(TiXmlElement* pXmlObject)
// 	{
// 
// 	}
// 
// 	void MeshData::Export(TiXmlElement* pXmlObject)
// 	{
// 
// 	}

	void MeshData::ReadS3Data()
	{
		uint32 nIden = m_pDataStream->ReadUInt();
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
		uint32 nIndexNum = m_pDataStream->ReadUInt();
		uint32 nVertexNum = m_pDataStream->ReadUInt();

		uint32 nIndexType = m_pDataStream->ReadUInt();
		uint32 nVertexType = m_pDataStream->ReadUInt();

		uint32 nSize = m_pDataStream->ReadUInt();
		vector<uint8> vecIndex;
		vecIndex.resize(nSize);
		m_pDataStream->Read(&vecIndex[0],nSize);

		nSize = m_pDataStream->ReadUInt();
		vector<uint8> vecVertex;
		vecVertex.resize(nSize);
		m_pDataStream->Read(&vecVertex[0],nSize);

		uint16 ShaeType = m_pDataStream->ReadUShort();
		Vector3 vPos = m_pDataStream->ReadVector3();
		Quaternion qRot = m_pDataStream->ReadQuaternion();

		if (ShaeType ==0)
		{
			Vector3 vAxis = m_pDataStream->ReadVector3();
		}
		else
		{
			Vector2 vRadiusHeight = m_pDataStream->ReadVector2();
		}

		uint32 nMeshLod = m_pDataStream->ReadUInt();
		for (uint32 i = 0; i < nMeshLod; ++i)
		{
			VEC_SUBMESH vecSubMesh;
			uint32 nSubMesh = m_pDataStream->ReadUInt();
			for (uint32 j = 0; j < nSubMesh; ++j)
			{
				SubMeshData* subMesh = new SubMeshData;
				subMesh->m_nIndexStart = m_pDataStream->ReadUInt();
				subMesh->m_nIndexCount = m_pDataStream->ReadUInt();
				subMesh->m_nVertexStart = m_pDataStream->ReadUInt();
				subMesh->m_nVertexCount = m_pDataStream->ReadUInt();

				uint16 ShaeType = m_pDataStream->ReadUShort();
				Vector3 vPos = m_pDataStream->ReadVector3();
				Quaternion qRot = m_pDataStream->ReadQuaternion();

				if (ShaeType ==0)
				{
					Vector3 vAxis = m_pDataStream->ReadVector3();
				}
				else
				{
					Vector2 vRadiusHeight = m_pDataStream->ReadVector2();
				}
				
				uint32 nBoneNum = m_pDataStream->ReadUInt();
				subMesh->m_arrBonePalette.resize(nBoneNum);
				for (uint32 iBone = 0; iBone < nBoneNum; ++iBone)
				{
					subMesh->m_arrBonePalette[iBone] = m_pDataStream->ReadUShort();
				}

				uint32 nStringLen = m_pDataStream->ReadUInt();
				vector<char> vecChar;
				vecChar.resize(nStringLen);
				m_pDataStream->Read(&vecChar[0],nStringLen);	
				string strSubmesName = &vecChar[0];

				nStringLen = m_pDataStream->ReadUInt();
				vecChar.resize(nStringLen);
				m_pDataStream->Read(&vecChar[0],nStringLen);	
				string strSubmeshTag = &vecChar[0];
				
				subMesh->m_nMateiralID = m_pDataStream->ReadUInt();

				vecSubMesh.push_back(subMesh);
			}
			m_arrLodSubMesh.push_back(vecSubMesh);
		}
	
	}

	bool MeshData::InitRes()
	{
		ReadS3Data();
// 		TiXmlDocument doc;
// 		bool bLoadOK = doc.Parse( (const char*)m_pDataStream->GetPtr() ) != NULL;
// 		ASSERT(bLoadOK);
// 		if (!bLoadOK)
// 			return false;
// 
// 		TiXmlElement* pXmlRoot = doc.FirstChildElement();
// 		ASSERT(pXmlRoot);
// 		this->Improt(pXmlRoot);

		return true;
	}

	void SubMeshData::AddBonePalette(BoneIndex bonde)
	{
		m_arrBonePalette.push_back(bonde);
	}

	ResourceSystem<MeshData>* g_pMeshManager = NULL;

	RefPtr<MeshData> CreateMeshData()
	{
		return new MeshData;
	}

	RefPtr<MeshData> CreateMeshData(const char* pszFile)
	{
		return g_pMeshManager->CreateResource(pszFile);
	}
}

