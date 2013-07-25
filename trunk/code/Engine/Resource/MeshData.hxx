#include "MeshData.h"
#include "Common/RTTI/Object.h"


namespace ma
{
	IMPL_OBJECT(MeshData,Resource)

	enum MeshVersion
	{
		MESH_VER_INITIAL = 1,

		MESH_VER_CURRENT = MESH_VER_INITIAL,
	};

	MeshHeader::MeshHeader()
	{
		m_nIdent = 'MAMD';
		m_nVersion = MESH_VER_CURRENT;
	}

	void SerializeData(Serializer& sl, AABB& val,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		
		sl.Serialize(val.m_vMin,"m_vMin");
		sl.Serialize(val.m_vMax,"m_vMax");

		sl.EndSection();
	}


	void SerializeData(Serializer& sl, MeshHeader& mehHeader, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(mehHeader.m_nIdent,"FileIden");
		sl.Serialize(mehHeader.m_nVersion,"Version");	

		sl.EndSection();
	}

	void SerializeData(Serializer& sl, SubMeshData& subMeshData, const char* pszLabel)
	{
		sl.BeginSection(pszLabel);

		sl.Serialize(subMeshData.m_nIndexStart,"IndexStart");
		sl.Serialize(subMeshData.m_nIndexCount,"IndexCount");
		sl.Serialize(subMeshData.m_nVertexStart,"VertexStart");
		sl.Serialize(subMeshData.m_nVertexCount,"VertexCount");
		sl.Serialize(subMeshData.m_arrBonePalette,"BonePalette");
		sl.Serialize(subMeshData.m_nMateiralID,"MaterialID");

		sl.EndSection();
	}

	void SerializeDataV0(Serializer& sl, MeshData& meshData, const char* pszLabel)
	{
		sl.BeginSection(pszLabel);

		sl.Serialize(meshData.m_nIndexType);
		sl.Serialize(meshData.m_nVertexType);
		sl.Serialize(meshData.m_arrIndexBuffer);
		sl.Serialize(meshData.m_arrVertexBuffer);	
		sl.Serialize(meshData.m_meshBound,"MeshBound");
		sl.Serialize(meshData.m_arrSubMesh,"SubMesh");
		sl.Serialize(meshData.m_nSkelGUID,"SkeletonGUID");

		sl.EndSection();
	}

	MeshData::MeshData()
	{
		m_nIndexType = 0;
		m_nVertexType = 0;
	}

	void MeshData::CreateFromMemeory()
	{
		if (m_eResState == ResLoaded)
			return;

		ASSERT(m_pDataStream);
		if (m_pDataStream == NULL)
			return;
		
		BinaryInputArchive inAr;
		inAr.Open(m_pDataStream);
		Serialize(inAr);
		m_pDataStream->rewind();

		InitRendable();
	
		m_eResState = ResLoaded;
	}


	bool MeshData::InitRendable()
	{
		void* pIndexData =  &m_arrIndexBuffer[0];
		int nIndexSize = m_arrIndexBuffer.size();
		INDEX_TYPE eIndexType = m_nIndexType == INDEX_TYPE_U16 ? INDEX_TYPE_U16 : INDEX_TYPE_U32; 
		m_pIndexBuffer = GetRenderDevice()->CreateIndexBuffer(pIndexData,nIndexSize,eIndexType);
		//m_pIndexBuffer->Active();

		void* pVertexData =  &m_arrVertexBuffer[0];
		int nVertexDataSize = m_arrVertexBuffer.size();
		int nVertexStride = GetVertexStride();
		m_pVertexBuffer = GetRenderDevice()->CreateVertexBuffer(pVertexData,nVertexDataSize, nVertexStride);
		//m_pVertexBuffer->Active();

		m_pDeclaration = GetRenderDevice()->CreateVertexDeclaration();	
		m_pDeclaration->Init(m_nVertexType);
		//m_pDeclaration->Active();

		return true;
	}

	void MeshData::SaveToFile(const char* pPath)
	{
		BinaryOutputArchive ar;
		bool bLoadOK = ar.Open(pPath);
		ASSERT(bLoadOK);
		if (!bLoadOK)
			return;

		Serialize(ar,"Mesh");

		ar.Close();
	}

	void MeshData::Serialize(Serializer& sl, const char* pszLable)
	{
		MeshHeader header;
		sl.Serialize(header);
		if (header.m_nIdent != 'MAMD')
			return;

		if (header.m_nVersion == MESH_VER_INITIAL)
		{
			SerializeDataV0(sl,*this,pszLable);
		}
	}

	int MeshData::GetVertexStride()
	{
		int voffset = 0;
		if (m_nVertexType & DUM_POSITION)
		{
			voffset += 12;
		}
		if (m_nVertexType & DUM_TEXCOORD)
		{
			voffset += 8;
		}
		if (m_nVertexType & DUM_NORMAL)
		{
			voffset += 12;
		}
		if (m_nVertexType & DUM_TANGENT)
		{
			voffset += 12;
		}
		if (m_nVertexType & DUM_BLENDINDICES)
		{
			voffset += 4;
		}
		if (m_nVertexType & DUM_BLENDWEIGHT)
		{
			voffset += 4;
		}

		return voffset;
	}

}

