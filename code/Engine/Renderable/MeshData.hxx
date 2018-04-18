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
		m_nBoneNumber = 0;
		m_nIndexType = INDEX_TYPE_U16;
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
		/*Quaternion qRot = */pDataStream->ReadQuaternion();

		if (ShaeType ==0)
		{
			Vector3 vSize = pDataStream->ReadVector3();
			return AABB(vPos - vSize,vPos + vSize);
		}
		else
		{
			/*Vector2 vRadiusHeight = */pDataStream->ReadVector2();
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

	UINT32 CompressNormal(const Vector3& vNorm)
	{
		Vector3 vNormIn = vNorm.normalisedCopy();

		union UIndex
		{
			uint32 uInde;
			uint8 uByte[4];
		};

		UIndex uTemp;
		uTemp.uByte[0] = (uint8)(vNormIn.x * 127.5f + 128.0f);
		uTemp.uByte[1] = (uint8)(vNormIn.y * 127.5f + 128.0f);
		uTemp.uByte[2] = (uint8)(vNormIn.z * 127.5f + 128.0f);
		uTemp.uByte[3] = (uint8)(0 * 127.5f + 128.0f);

		return uTemp.uInde;
	}

	UINT32 CompressQuaternion(const Vector3& tangent,Vector3 const& normal)
	{
		Vector3 vTangent = ( tangent - normal * ( tangent.dotProduct(normal) ) ).normalisedCopy();  
		Vector3 binormal = normal.crossProduct(vTangent);

		float k = 1;
		if ( binormal.dotProduct( normal.crossProduct(tangent) ) < 0 )
		{
			k = -1;
		}

		Matrix3 tangent_frame;
		tangent_frame.SetColumn(0,vTangent);
		tangent_frame.SetColumn(1,k * binormal);
		tangent_frame.SetColumn(2,normal);

		Quaternion tangent_quat;
		tangent_quat.FromRotationMatrix(tangent_frame);
		if (tangent_quat.w < 0)
		{
			tangent_quat = -tangent_quat;
		}
		//if (bits > 0)
		{
			float const bias = float(1) / ((1UL << (8 - 1)) - 1);
			if (tangent_quat.w < bias)
			{
				float const factor = sqrt(1 - bias * bias);
				tangent_quat.x *= factor;
				tangent_quat.y *= factor;
				tangent_quat.z *= factor;
				tangent_quat.w = bias;
			}
		}
		if (k < 0)
		{
			tangent_quat = -tangent_quat;
		}

		union UIndex
		{
			uint32 uInde;
			uint8 uByte[4];
		};

		UIndex uTemp;
		uTemp.uByte[0] = (uint8)(tangent_quat.x * 127.5f + 128.0f);
		uTemp.uByte[1] = (uint8)(tangent_quat.y * 127.5f + 128.0f);
		uTemp.uByte[2] = (uint8)(tangent_quat.z * 127.5f + 128.0f);
		uTemp.uByte[3] = (uint8)(tangent_quat.w * 127.5f + 128.0f);

		return uTemp.uInde;
	}

	SHORTV4 CompressPos(const Vector3& vPos, const Vector3& vCenter, const Vector3& vExtent)
	{
		SHORTV4 vSkinPos;

		vSkinPos.x = (int16)(((vPos.x -  vCenter.x) / vExtent.x) * 32767.5f);
		vSkinPos.y = (int16)(((vPos.y -  vCenter.y) / vExtent.y) * 32767.5f);
		vSkinPos.z = (int16)(((vPos.z -  vCenter.z) / vExtent.z) * 32767.5f);
		vSkinPos.w = (int16)(((1.0f -  vCenter.z) / vExtent.z) * 32767.5f);

		return vSkinPos;
	}

	Vector3 DecompressPos(SHORTV4 vPos,const Vector3& vCenter, const Vector3& vExtent)
	{
		Vector3 vDePos;

		vDePos.x = vPos.x / 32767.5f * vExtent.x + vCenter.x;
		vDePos.y = vPos.y / 32767.5f * vExtent.y + vCenter.y;
		vDePos.z = vPos.z / 32767.5f * vExtent.z + vCenter.z;

		return vDePos;
	}

	SHORTV2 CompressUV(const Vector2& vUV,const Vector2& vCenter, const Vector2& vExtent)
	{
		SHORTV2 vSkinUV;

		vSkinUV.x = (int16)(((vUV.x -  vCenter.x) / vExtent.x) * 32767.5f);
		vSkinUV.y = (int16)(((vUV.y -  vCenter.y) / vExtent.y) * 32767.5f);

		return vSkinUV;
	}

	bool MeshData::SaveToFile(const char* pszFile)
	{
		RefPtr<FileStream> pSaveStream = CreateFileStream(pszFile); 

		uint32 nIden = 'MAMD';
		uint32 nVersion = 0;

		pSaveStream->WriteUInt(nIden);
		pSaveStream->WriteUInt(nVersion);

		pSaveStream->WriteUInt(m_nBoneNumber);
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
		
		pSaveStream->WriteUInt(m_arrSubMesh.size());
		for (uint32 j = 0; j < m_arrSubMesh.size(); ++j)
		{
			SubMeshData* subMesh = m_arrSubMesh[j].get();

			pSaveStream->WriteUInt(subMesh->m_nIndexStart);
			pSaveStream->WriteUInt(subMesh->m_nIndexCount);
			pSaveStream->WriteUInt(subMesh->m_nVertexStart);
			pSaveStream->WriteUInt(subMesh->m_nVertexCount);
	
			pSaveStream->WriteUInt(subMesh->m_arrBonePalette.size());
			for (uint32 iBone = 0; iBone < subMesh->m_arrBonePalette.size(); ++iBone)
			{
				pSaveStream->WriteUShort(subMesh->m_arrBonePalette[iBone]);
			}

			pSaveStream->WriteUInt(subMesh->m_nMateiralID);	
		}
	
		return true;
	}

	void MeshData::ReadDataV1()
	{
		//uint32 nIden = m_pDataStream->ReadUInt();
		uint32 nVersion = m_pDataStream->ReadUInt();
		ASSERT(nVersion == 0);

		m_nBoneNumber = m_pDataStream->ReadUInt();
		uint32 nIndexNum = m_pDataStream->ReadUInt();
		uint32 nVertexNum = m_pDataStream->ReadUInt();

		m_nIndexType = (INDEX_TYPE)m_pDataStream->ReadUInt();
		ASSERT(m_nIndexType == INDEX_TYPE_U16);

		m_nVertexType = (VertexType)m_pDataStream->ReadUInt();
		
		vector<uint8> vecIBData;

		uint32 nIndexSize = m_pDataStream->ReadUInt();
		vecIBData.resize(nIndexSize);
		m_pDataStream->Read(&vecIBData[0],nIndexSize);

		if (m_nIndexType == INDEX_TYPE_U16)
		{
			UINT32 nIndexCount = nIndexSize / sizeof(uint16);
			ASSERT(nIndexCount == nIndexNum);

			m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer(&vecIBData[0],nIndexSize,sizeof(uint16));
		}
		else
		{
			UINT32 nIndexCount = nIndexSize / sizeof(UINT32);
			ASSERT(nIndexCount == nIndexNum);

			m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer(&vecIBData[0],nIndexSize,sizeof(UINT32));
		}
		
		vector<uint8> vecVBData;

		uint32 nVertexSize = m_pDataStream->ReadUInt();
		vecVBData.resize(nVertexSize);
		m_pDataStream->Read(&vecVBData[0],nVertexSize);
	
		if (m_nVertexType == SKIN_VERTEX_1)
		{
			uint32 nVertexCount = nVertexSize / sizeof(SkinVertexV1);
			ASSERT(nVertexCount == nVertexNum);

			VertexElement element[5];
			element[0] = VertexElement(0,0,DT_SHORT4N,DU_POSITION,0);
			element[1] = VertexElement(0,8,DT_UBYTE4N,DU_NORMAL,0);
			element[2] = VertexElement(0,12,DT_SHORT2N,DU_TEXCOORD,0);
			element[3] = VertexElement(0,16,DT_UBYTE4,DU_BLENDINDICES,0);
			element[4] = VertexElement(0,20,DT_UBYTE4N,DU_BLENDWEIGHT,0);
			m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,5);

			m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer(&vecVBData[0],nVertexSize,sizeof(SkinVertexV1));
		}
		else if (m_nVertexType == STATIC_VERTEX_1)
		{
			uint32 nVertexCount = nVertexSize / sizeof(StaticVertexV1);
			ASSERT(nVertexCount == nVertexNum);

			VertexElement element[3];
			element[0] = VertexElement(0,0,DT_SHORT4N,DU_POSITION,0);
			element[1] = VertexElement(0,8,DT_UBYTE4N,DU_NORMAL,0);
			element[2] = VertexElement(0,12,DT_SHORT2N,DU_TEXCOORD,0);
			m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,3);

			m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer(&vecVBData[0],nVertexSize,sizeof(StaticVertexV1));
		}

		m_meshBound = m_pDataStream->ReadBoundingBox();
		m_tcBound = m_pDataStream->ReadBoundingBox2D();

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

			m_arrSubMesh.push_back(subMesh);
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

		char GID[16];
		m_pDataStream->Read(GID,16);

		m_nBoneNumber = m_pDataStream->ReadUInt();
		uint32 nIndexNum = m_pDataStream->ReadUInt();
		uint32 nVertexNum = m_pDataStream->ReadUInt();

		m_nIndexType = (INDEX_TYPE)m_pDataStream->ReadUInt();
		m_nVertexType = (VertexType)m_pDataStream->ReadUInt();
		ASSERT(m_nIndexType == INDEX_TYPE_U16);
		ASSERT(m_nVertexType == SKIN_VERTEX_0);

		vector<uint8> vecIBData;

		uint32 nIndexSize = m_pDataStream->ReadUInt();
		vecIBData.resize(nIndexSize);
		m_pDataStream->Read(&vecIBData[0],nIndexSize);
	
		UINT32 nIndexCount = nIndexSize / sizeof(uint16);
		ASSERT(nIndexCount == nIndexNum);
		
		vector<uint8> vecVBData;

		uint32 nVertexSize = m_pDataStream->ReadUInt();
		vecVBData.resize(nVertexSize);
		m_pDataStream->Read(&vecVBData[0],nVertexSize);

		uint32 nVertexCount = nVertexSize / sizeof(SkinVertexV0);

		m_meshBound = ReadShape(m_pDataStream.get());

		uint32 nMeshLod = m_pDataStream->ReadUInt();
		for (uint32 iLod = 0; iLod < nMeshLod; ++iLod)
		{
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

				m_arrSubMesh.push_back(subMesh);
			}
		}

		SkinVertexV0* pVertexV0 = (SkinVertexV0*)(&vecVBData[0]);
		uint16* pIndex = (uint16*)(&vecIBData[0]);

		vector<SkinVertexV1> pVertexV1;
		pVertexV1.resize(nVertexNum);
		UpdateMeshData(&pVertexV1[0],pVertexV0,nVertexCount,pIndex);

		VertexElement element[5];
		element[0] = VertexElement(0,0,DT_SHORT4N,DU_POSITION,0);
		element[1] = VertexElement(0,8,DT_UBYTE4N,DU_NORMAL,0);
		element[2] = VertexElement(0,12,DT_SHORT2N,DU_TEXCOORD,0);
		element[3] = VertexElement(0,16,DT_UBYTE4,DU_BLENDINDICES,0);
		element[4] = VertexElement(0,20,DT_UBYTE4N,DU_BLENDWEIGHT,0);
		m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,5);

		m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer(&vecIBData[0],nIndexSize,sizeof(uint16));

		m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer((uint8*)&pVertexV1[0],nVertexNum * sizeof(SkinVertexV1),sizeof(SkinVertexV1));
	}

	void MeshData::UpdateMeshData(StaticVertexV1* pVertexV1,SkinVertexV0* pVertexV0,uint32 nVertexCount,uint16* pIndex)
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

		for (UINT i = 0; i < nVertexCount; ++i)
		{
			SkinVertexV0& vertexV0 = pVertexV0[i];
			StaticVertexV1& vertexV1 = pVertexV1[i];

			vertexV1.pos = CompressPos(vertexV0.pos,pos_center,pos_extent);
			vertexV1.nor = CompressNormal(vertexV0.nor);
			vertexV1.uv = CompressUV(vertexV0.uv,tc_center,tc_extent);
		}
	}

	void MeshData::UpdateMeshData(SkinVertexV1* pVertexV1,SkinVertexV0* pVertexV0,uint32 nVertexCount,uint16* pIndex)
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

		for (UINT i = 0; i < nVertexCount; ++i)
		{
			SkinVertexV0& vertexV0 = pVertexV0[i];
			SkinVertexV1& vertexV1 = pVertexV1[i];

			vertexV1.pos = CompressPos(vertexV0.pos,pos_center,pos_extent);
			vertexV1.nor = CompressNormal(vertexV0.nor);
			vertexV1.uv = CompressUV(vertexV0.uv,tc_center,tc_extent);
			vertexV1.bone_index = vertexV0.bone_index;
			vertexV1.bone_weight = vertexV0.bone_weight;
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

	void MeshData::AddSubMeshData(SubMeshData* pSubMeshData)
	{
		m_arrSubMesh.push_back(pSubMeshData);
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

