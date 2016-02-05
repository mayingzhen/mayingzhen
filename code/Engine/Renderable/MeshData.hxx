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
		m_nIndexType = INDEX_16;
		m_nVertexType = SKIN_VERTEX_1;
		m_pIndexBuffer = GetRenderDevice()->CreateIndexBuffer();
		m_pVertexBuffer = GetRenderDevice()->CreateVertexBuffer();
		m_pDeclaration = GetRenderDevice()->CreateVertexDeclaration();	
	}

	MeshData::~MeshData()
	{
	}

	AABB ReadShape(MemoryStream* pDataStream)
	{
		uint16 ShaeType = pDataStream->ReadUShort();
		Vector3 vPos = pDataStream->ReadVector3();
		Quaternion qRot = pDataStream->ReadQuaternion();

		if (ShaeType ==0)
		{
			Vector3 vSize = pDataStream->ReadVector3();
			return AABB(vPos - vSize,vPos + vSize);
		}
		else
		{
			Vector2 vRadiusHeight = pDataStream->ReadVector2();
			return AABB();
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

	UINT32 ToSkinNormal(const Vector3& vNorm)
	{
		Vector3 vNormIn = vNorm.normalisedCopy();

		union UIndex
		{
			UINT32 uInde;
			UINT8 uByte[4];
		};

		UIndex uTemp;
		uTemp.uByte[0] = (UINT8)(vNormIn.x * 127.5f + 128.0f);
		uTemp.uByte[1] = (UINT8)(vNormIn.y * 127.5f + 128.0f);
		uTemp.uByte[2] = (UINT8)(vNormIn.z * 127.5f + 128.0f);
		uTemp.uByte[3] = (UINT8)(0 * 127.5f + 128.0f);

		return uTemp.uInde;
	}

	SHORTV4 ToSkinPos(const Vector3& vPos, const Vector3& vCenter, const Vector3& vExtent)
	{
		SHORTV4 vSkinPos;

		vSkinPos.x = (INT16)(((vPos.x -  vCenter.x) / vExtent.x) * 32767.5f);
		vSkinPos.y = (INT16)(((vPos.y -  vCenter.y) / vExtent.y) * 32767.5f);
		vSkinPos.z = (INT16)(((vPos.z -  vCenter.z) / vExtent.z) * 32767.5f);
		vSkinPos.w = (INT16)(((1.0f -  vCenter.z) / vExtent.z) * 32767.5f);

		return vSkinPos;
	}

	SHORTV2 ToSkinUV(const Vector2& vUV,const Vector2& vCenter, const Vector2& vExtent)
	{
		SHORTV2 vSkinUV;

		vSkinUV.x = (INT16)(((vUV.x -  vCenter.x) / vExtent.x) * 32767.5f);
		vSkinUV.y = (INT16)(((vUV.y -  vCenter.y) / vExtent.y) * 32767.5f);

		return vSkinUV;
	}

	bool MeshData::SaveToFile(const char* pszFile)
	{
		RefPtr<FileStream> pSaveStream = CreateFileStream(pszFile); 

		uint32 nIden = 'MAMD';
		uint32 nVersion = 0;

		pSaveStream->WriteUInt(nIden);
		pSaveStream->WriteUInt(nVersion);

		pSaveStream->WriteUInt(m_pIndexBuffer->GetNumber());
		pSaveStream->WriteUInt(m_pVertexBuffer->GetNumber());
		
		pSaveStream->WriteUInt(m_nIndexType);
		pSaveStream->WriteUInt(m_nVertexType);

		uint32 nIndexSize = m_pIndexBuffer->GetSize();
		pSaveStream->WriteUInt(nIndexSize);
		pSaveStream->Write(m_pIndexBuffer->GetData(),nIndexSize);

		uint32 nVertexSize = m_pVertexBuffer->GetSize();
		pSaveStream->WriteUInt(nVertexSize);
		pSaveStream->Write(m_pVertexBuffer->GetData(),nVertexSize);

		pSaveStream->WriteBoundingBox(m_meshBound);
		pSaveStream->WriteBoundingBox2D(m_tcBound);
		
		pSaveStream->WriteUInt(m_arrLodSubMesh.size());
		for (uint32 i = 0; i < m_arrLodSubMesh.size(); ++i)
		{
			VEC_SUBMESH& vecSubMesh = m_arrLodSubMesh[i];
			pSaveStream->WriteUInt(vecSubMesh.size());
			for (uint32 j = 0; j < vecSubMesh.size(); ++j)
			{
				SubMeshData* subMesh = vecSubMesh[j].get();

				pSaveStream->WriteUInt(subMesh->m_nIndexStart);
				pSaveStream->WriteUInt(subMesh->m_nIndexCount);
				pSaveStream->WriteUInt(subMesh->m_nVertexStart);
				pSaveStream->WriteUInt(subMesh->m_nVertexCount);
		
				pSaveStream->WriteUInt(subMesh->m_arrBonePalette.size());
				for (uint32 iBone = 0; iBone < subMesh->m_arrBonePalette.size(); ++iBone)
				{
					pSaveStream->WriteUInt(subMesh->m_arrBonePalette[iBone]);
				}

				pSaveStream->WriteUInt(subMesh->m_nMateiralID);	
			}
		}

		return true;
	}

	void MeshData::ReadDataV1()
	{
		//uint32 nIden = m_pDataStream->ReadUInt();
		uint32 nVersion = m_pDataStream->ReadUInt();
		ASSERT(nVersion == 0);

		uint32 nIndexNum = m_pDataStream->ReadUInt();
		uint32 nVertexNum = m_pDataStream->ReadUInt();

		uint32 nIndexType = m_pDataStream->ReadUInt();
		ASSERT(nIndexType == INDEX_16);

		uint32 nVertexType = m_pDataStream->ReadUInt();

		uint32 nIndexSize = m_pDataStream->ReadUInt();
		uint8* vecIndex = new uint8[nIndexSize];
		m_pDataStream->Read(&vecIndex[0],nIndexSize);

		uint16* pIndex = (uint16*)vecIndex;
		UINT32 nIndexCount = nIndexSize / sizeof(uint16);
		ASSERT(nIndexCount == nIndexNum);

		m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer(vecIndex,nIndexSize,sizeof(uint16));

		uint32 nVertexSize = m_pDataStream->ReadUInt();
		uint8* vecVertex = new uint8[nVertexSize];
		m_pDataStream->Read(&vecVertex[0],nVertexSize);
			
		if (nVertexType == SKIN_VERTEX_1)
		{
			SkinVertexV1* pVertexV1 = (SkinVertexV1*)vecVertex;
			uint32 nVertexCount = nVertexSize / sizeof(SkinVertexV1);
			ASSERT(nVertexCount == nVertexNum);

			VertexElement element[5];
			element[0] = VertexElement(0,0,DT_SHORT4,DU_POSITION,0);
			element[1] = VertexElement(0,8,DT_UBYTE4N,DU_NORMAL,0);
			element[2] = VertexElement(0,12,DT_SHORT2,DU_TEXCOORD,0);
			element[3] = VertexElement(0,16,DT_UBYTE4,DU_BLENDINDICES,0);
			element[4] = VertexElement(0,20,DT_UBYTE4N,DU_BLENDWEIGHT,0);
			m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,5);

			m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer(vecVertex,nVertexSize,sizeof(SkinVertexV1));
		}
		else if (nVertexType == STATIC_VERTEX_1)
		{
			StaticVertexV1* pVertexV1 = (StaticVertexV1*)vecVertex;
			uint32 nVertexCount = nVertexSize / sizeof(StaticVertexV1);
			ASSERT(nVertexCount == nVertexNum);

			VertexElement element[3];
			element[0] = VertexElement(0,0,DT_SHORT4,DU_POSITION,0);
			element[1] = VertexElement(0,8,DT_UBYTE4N,DU_NORMAL,0);
			element[2] = VertexElement(0,12,DT_SHORT2,DU_TEXCOORD,0);
			m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,3);

			m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer(vecVertex,nVertexSize,sizeof(StaticVertexV1));
		}

		m_meshBound = m_pDataStream->ReadBoundingBox();
		m_tcBound = m_pDataStream->ReadBoundingBox2D();

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
		//uint32 nIden = m_pDataStream->ReadUInt();
		//ASSERT(nIden == 'S3MD');
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

		m_meshBound = ReadShape(m_pDataStream.get());

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

		vector<SkinVertexV1> pVertexV1;
		pVertexV1.resize(nVertexNum);
		UpdateMeshData(&pVertexV1[0],pVertexV0,nVertexCount,pIndex);

		VertexElement element[5];
		element[0] = VertexElement(0,0,DT_SHORT4,DU_POSITION,0);
		element[1] = VertexElement(0,8,DT_UBYTE4N,DU_NORMAL,0);
		element[2] = VertexElement(0,12,DT_SHORT2,DU_TEXCOORD,0);
		element[3] = VertexElement(0,16,DT_UBYTE4,DU_BLENDINDICES,0);
		element[4] = VertexElement(0,20,DT_UBYTE4N,DU_BLENDWEIGHT,0);
		m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,5);

		m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer(vecIndex,nIndexSize,sizeof(uint16));

		m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer((uint8*)&pVertexV1[0],nVertexNum * sizeof(SkinVertexV1),sizeof(SkinVertexV1));
	}

	void MeshData::UpdateMeshData(StaticVertexV1* pVertexV1,SkinVertexV0* pVertexV0,uint32 nVertexCount,UINT16* pIndex)
	{
		for (UINT32 i = 0; i < nVertexCount; ++i)
		{
			m_tcBound.merge(pVertexV0[i].uv);
			m_meshBound.merge(pVertexV0[i].pos);
		}

		Vector2 tc_center = m_tcBound.getCenter();
		Vector2 tc_extent = m_tcBound.getHalfSize();

		Vector3 pos_center = m_meshBound.getCenter();
		Vector3 pos_extent = m_meshBound.getHalfSize();

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

				for(uint32 iIndex = subMesh->m_nIndexStart; iIndex < subMesh->m_nIndexStart + subMesh->m_nIndexCount; ++iIndex)
				{
					uint16 nIndex = pIndex[iIndex];
					if (vertexUpdated[nIndex])
						continue;

					SkinVertexV0& vertexV0 = pVertexV0[nIndex];
					StaticVertexV1& vertexV1 = pVertexV1[nIndex];

					vertexV1.pos = ToSkinPos(vertexV0.pos,pos_center,pos_extent);
					vertexV1.nor = ToSkinNormal(vertexV0.nor);
					vertexV1.uv = ToSkinUV(vertexV0.uv,tc_center,tc_extent);
			
					vertexUpdated[nIndex] = true;
				}
			}
		}
	}

	void MeshData::UpdateMeshData(SkinVertexV1* pVertexV1,SkinVertexV0* pVertexV0,uint32 nVertexCount,UINT16* pIndex)
	{
		for (UINT32 i = 0; i < nVertexCount; ++i)
		{
			m_tcBound.merge(pVertexV0[i].uv);
			m_meshBound.merge(pVertexV0[i].pos);
		}

		Vector2 tc_center = m_tcBound.getCenter();
		Vector2 tc_extent = m_tcBound.getHalfSize();

		Vector3 pos_center = m_meshBound.getCenter();
		Vector3 pos_extent = m_meshBound.getHalfSize();

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

				//if (nBoneNum > 75/*MAX_MAT_NUM_BONES*/)
				{
				}
				//else
				{
					for(uint32 iIndex = subMesh->m_nIndexStart; iIndex < subMesh->m_nIndexStart + subMesh->m_nIndexCount; ++iIndex)
					{
						uint16 nIndex = pIndex[iIndex];
						if (vertexUpdated[nIndex])
							continue;

						SkinVertexV0& vertexV0 = pVertexV0[nIndex];
						SkinVertexV1& vertexV1 = pVertexV1[nIndex];

						vertexV1.pos = ToSkinPos(vertexV0.pos,pos_center,pos_extent);
						vertexV1.nor = ToSkinNormal(vertexV0.nor);
						vertexV1.uv = ToSkinUV(vertexV0.uv,tc_center,tc_extent);
						vertexV1.bone_index = vertexV0.bone_index;
						vertexV1.bone_weight = vertexV0.bone_weight;

						uint8 boneIndex[4];

						GetBoneIndex(vertexV0.bone_index,boneIndex[0],boneIndex[1],boneIndex[2],boneIndex[3]);

						boneIndex[0] = (BYTE)subMesh->m_arrBonePalette[ boneIndex[0] ];
						boneIndex[1] = (BYTE)subMesh->m_arrBonePalette[ boneIndex[1] ];
						boneIndex[2] = (BYTE)subMesh->m_arrBonePalette[ boneIndex[2] ];
						boneIndex[3] = (BYTE)subMesh->m_arrBonePalette[ boneIndex[3] ];

						vertexV1.bone_index = ToBoneIndex(boneIndex[0],boneIndex[1],boneIndex[2],boneIndex[3]);

						vertexUpdated[nIndex] = true;
					}

					subMesh->m_arrBonePalette.clear();
				}
			}
		}
	}

	bool MeshData::InitRes()
	{
		uint32 nIden = m_pDataStream->ReadUInt();
		if (nIden == 'S3MD')
		{
			ReadDataV0();
		}
		else if (nIden == 'MAMD')
		{
			ReadDataV1();
		}

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

