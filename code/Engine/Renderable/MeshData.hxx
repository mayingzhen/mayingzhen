#include "MeshData.h"


namespace ma
{
	void SubMeshData::AddBonePalette(BoneIndex bonde)
	{
		m_arrBonePalette.push_back(bonde);
	}

	RefPtr<SubMeshData>  CreateSubMeshData()
	{
		return new SubMeshData();
	}

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

	void ReadShape(MemoryStream* pDataStream)
	{
		uint16 ShaeType = pDataStream->ReadUShort();
		Vector3 vPos = pDataStream->ReadVector3();
		Quaternion qRot = pDataStream->ReadQuaternion();

		if (ShaeType ==0)
		{
			Vector3 vAxis = pDataStream->ReadVector3();
		}
		else
		{
			Vector2 vRadiusHeight = pDataStream->ReadVector2();
		}
	}

	uint32 ToBoneIndex(uint8 nID0,uint8 nID1, uint8 nID2, uint8 nID3)
	{
		union UIndex
		{
			uint32 uInde;
			uint8 uByte[4];
		};

		UIndex uBondID;
		uBondID.uByte[0] = nID0;
		uBondID.uByte[1] = nID1;
		uBondID.uByte[2] = nID2;
		uBondID.uByte[3] = nID3;

		return uBondID.uInde;
	}

	void GetBoneIndex(uint32 nIndex, uint8& nID0, uint8& nID1, uint8& nID2, uint8& nID3)
	{
		union UIndex
		{
			uint32 uInde;
			uint8 uByte[4];
		};

		UIndex uBondID;
		uBondID.uInde = nIndex;
		nID0 = uBondID.uByte[0];
		nID1 = uBondID.uByte[1];
		nID2 = uBondID.uByte[2];
		nID3 = uBondID.uByte[3];
	}

	bool MeshData::SaveToFile(const char* pszFile)
	{
		std::fstream out(pszFile, ios::out|ios::binary);
		FileStream SaveStream(&out);

		uint32 nIden = 0;
		uint32 nVersion = 0;

		uint32 nIndexType = 0;
		uint32 nVertexType = 0;

		SaveStream.WriteUInt(nIden);
		SaveStream.WriteUInt(nVersion);

		SaveStream.WriteUInt(m_pIndexBuffer->GetNumber());
		SaveStream.WriteUInt(m_pVertexBuffer->GetNumber());
		
		SaveStream.WriteUInt(nIndexType);
		SaveStream.WriteUInt(nVertexType);

		uint32 nIndexSize = m_pVertexBuffer->GetSize();
		SaveStream.WriteUInt(nIndexSize);
		SaveStream.Write(m_pVertexBuffer->GetData(),nIndexSize);

		uint32 nVertexSize = m_pIndexBuffer->GetSize();
		SaveStream.WriteUInt(nVertexSize);
		SaveStream.Write(m_pIndexBuffer->GetData(),nVertexSize);

		SaveStream.WriteBoundingBox(m_meshBound);
		
		SaveStream.WriteUInt(m_arrLodSubMesh.size());
		for (uint32 i = 0; i < m_arrLodSubMesh.size(); ++i)
		{
			VEC_SUBMESH& vecSubMesh = m_arrLodSubMesh[i];
			SaveStream.WriteUInt(vecSubMesh.size());
			for (uint32 j = 0; j < vecSubMesh.size(); ++j)
			{
				SubMeshData* subMesh = vecSubMesh[j].get();

				SaveStream.WriteUInt(subMesh->m_nIndexStart);
				SaveStream.WriteUInt(subMesh->m_nIndexCount);
				SaveStream.WriteUInt(subMesh->m_nVertexStart);
				SaveStream.WriteUInt(subMesh->m_nVertexCount);
		
				SaveStream.WriteUInt(subMesh->m_arrBonePalette.size());
				for (uint32 iBone = 0; iBone < subMesh->m_arrBonePalette.size(); ++iBone)
				{
					SaveStream.WriteUInt(subMesh->m_arrBonePalette[iBone]);
				}

				SaveStream.WriteUInt(subMesh->m_nMateiralID);	
			}
		}

		return true;
	}

	void MeshData::ReadDataV1()
	{
		uint32 nIden = m_pDataStream->ReadUInt();
		uint32 nVersion = m_pDataStream->ReadUInt();

		uint32 nIndexNum = m_pDataStream->ReadUInt();
		uint32 nVertexNum = m_pDataStream->ReadUInt();

		uint32 nIndexType = m_pDataStream->ReadUInt();
		uint32 nVertexType = m_pDataStream->ReadUInt();

		uint32 nIndexSize = m_pDataStream->ReadUInt();
		uint8* vecIndex = new uint8[nIndexSize];
		m_pDataStream->Read(&vecIndex[0],nIndexSize);

		uint16* pIndex = (uint16*)vecIndex;
		UINT32 nIndexCount = nIndexSize / sizeof(uint16);
		ASSERT(nIndexCount == nIndexNum);

		uint32 nVertexSize = m_pDataStream->ReadUInt();
		uint8* vecVertex = new uint8[nVertexSize];
		m_pDataStream->Read(&vecVertex[0],nVertexSize);

		SkinVertexV1* pVertexV0 = (SkinVertexV1*)vecVertex;
		uint32 nVertexCount = nVertexSize / sizeof(SkinVertexV1);
		ASSERT(nVertexCount == nVertexNum);

		m_meshBound = m_pDataStream->ReadBoundingBox();

		uint32 nMeshLod = m_pDataStream->ReadUInt();
		for (uint32 iLod = 0; iLod < nMeshLod; ++iLod)
		{
			VEC_SUBMESH vecSubMesh;
			uint32 nSubMesh = m_pDataStream->ReadUInt();
			for (uint32 iSub = 0; iSub < nSubMesh; ++iSub)
			{
				SubMeshData* subMesh = new SubMeshData;
				subMesh->m_nIndexStart = m_pDataStream->ReadUInt();
				subMesh->m_nIndexCount = m_pDataStream->ReadUInt();
				subMesh->m_nVertexStart = m_pDataStream->ReadUInt();
				subMesh->m_nVertexCount = m_pDataStream->ReadUInt();

				uint32 nBoneNum = m_pDataStream->ReadUInt();
				subMesh->m_arrBonePalette.resize(nBoneNum);
				for (uint32 iBone = 0; iBone < nBoneNum; ++iBone)
				{
					subMesh->m_arrBonePalette[iBone] = m_pDataStream->ReadUShort();
				}

				subMesh->m_nMateiralID = m_pDataStream->ReadUInt();

				vecSubMesh.push_back(subMesh);
			}
			m_arrLodSubMesh.push_back(vecSubMesh);
		}
	}

	void MeshData::ReadDataV0()
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

		uint32 nIndexSize = m_pDataStream->ReadUInt();
		uint8* vecIndex = new uint8[nIndexSize];
		m_pDataStream->Read(&vecIndex[0],nIndexSize);
	
		uint16* pIndex = (uint16*)vecIndex;
		UINT32 nIndexCount = nIndexSize / sizeof(uint16);

		uint32 nVertexSize = m_pDataStream->ReadUInt();
		uint8* vecVertex = new uint8[nVertexSize];
		m_pDataStream->Read(&vecVertex[0],nVertexSize);

		SkinVertexV0* pVertexV0 = (SkinVertexV0*)vecVertex;
		uint32 nVertexCount = nVertexSize / sizeof(SkinVertexV0);

		ReadShape(m_pDataStream.get());

		uint32 nMeshLod = m_pDataStream->ReadUInt();
		for (uint32 iLod = 0; iLod < nMeshLod; ++iLod)
		{
			VEC_SUBMESH vecSubMesh;
			uint32 nSubMesh = m_pDataStream->ReadUInt();
			for (uint32 iSub = 0; iSub < nSubMesh; ++iSub)
			{
				SubMeshData* subMesh = new SubMeshData;
				subMesh->m_nIndexStart = m_pDataStream->ReadUInt();
				subMesh->m_nIndexCount = m_pDataStream->ReadUInt();
				subMesh->m_nVertexStart = m_pDataStream->ReadUInt();
				subMesh->m_nVertexCount = m_pDataStream->ReadUInt();

				ReadShape(m_pDataStream.get());
				
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

		//////////////////////////////////////////////////////////////////////////
		std::vector<bool> vertexUpdated;
		vertexUpdated.resize(nVertexCount,false);
		for (uint32 iLod = 0; iLod < m_arrLodSubMesh.size(); ++iLod)
		{
			VEC_SUBMESH& vecSubMesh = m_arrLodSubMesh[iLod];
			for (uint32 iSub = 0; iSub < vecSubMesh.size(); ++iSub)
			{
				SubMeshData* subMesh = vecSubMesh[iSub].get();	

				for(uint32 iIndex = subMesh->m_nIndexStart; iIndex < subMesh->m_nIndexStart + subMesh->m_nIndexCount; ++iIndex)
				{
					pIndex[iIndex] += subMesh->m_nVertexStart;
				}

				subMesh->m_nVertexStart = 0;
				subMesh->m_nVertexCount = nVertexCount;

				if (nBoneNum > 75/*MAX_MAT_NUM_BONES*/)
				{
				}
				else
				{
					for(uint32 iIndex = subMesh->m_nIndexStart; iIndex < subMesh->m_nIndexStart + subMesh->m_nIndexCount; ++iIndex)
					{
						uint16 nIndex = pIndex[iIndex];
						if (vertexUpdated[nIndex])
							continue;

						SkinVertexV0& vertexV0 = pVertexV0[nIndex];

						uint8 boneIndex[4];

						GetBoneIndex(vertexV0.bone_index,boneIndex[0],boneIndex[1],boneIndex[2],boneIndex[3]);

						boneIndex[0] = (BYTE)subMesh->m_arrBonePalette[ boneIndex[0] ];
						boneIndex[1] = (BYTE)subMesh->m_arrBonePalette[ boneIndex[1] ];
						boneIndex[2] = (BYTE)subMesh->m_arrBonePalette[ boneIndex[2] ];
						boneIndex[3] = (BYTE)subMesh->m_arrBonePalette[ boneIndex[3] ];

						vertexV0.bone_index = ToBoneIndex(boneIndex[0],boneIndex[1],boneIndex[2],boneIndex[3]);

						vertexUpdated[nIndex] = true;
					}

					subMesh->m_arrBonePalette.clear();
				}
			}
		}

		VertexElement element[8];
		element[0] = VertexElement(0,0,DT_FLOAT3,DU_POSITION,0);
		element[1] = VertexElement(0,12,DT_UBYTE4,DU_BLENDINDICES,0);
		element[2] = VertexElement(0,16,DT_UBYTE4N,DU_BLENDWEIGHT,0);
		element[3] = VertexElement(0,20,DT_FLOAT3,DU_NORMAL,0);
		element[4] = VertexElement(0,32,DT_FLOAT2,DU_TEXCOORD,0);
		element[5] = VertexElement(0,40,DT_FLOAT3,DU_TANGENT,0);
		element[6] = VertexElement(0,52,DT_COLOR,DU_COLOR,0);
		element[7] = VertexElement(0,56,DT_COLOR,DU_COLOR,1);
		m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,8);

		m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer(vecIndex,nIndexSize,sizeof(uint16));

		m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer(vecVertex,nVertexSize,sizeof(SkinVertexV0));
	}

	bool MeshData::InitRes()
	{
		ReadDataV0();

		return true;
	}

	void MeshData::AddSubMeshData(uint32 nLod,SubMeshData* pSubMeshData)
	{
		if (nLod >= m_arrLodSubMesh.size())
		{
			m_arrLodSubMesh.resize(nLod + 1);
		}
		m_arrLodSubMesh[nLod].push_back(pSubMeshData);
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

