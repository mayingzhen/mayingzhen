#include "MeshData.h"
#include "Common/RTTI/Object.h"


namespace ma
{

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

	void SerializeData(SerializeListener& sl, AABB& val,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		
		sl.Serialize(val.m_vMin,"m_vMin");
		sl.Serialize(val.m_vMax,"m_vMax");

		sl.EndSection();
	}


	void SerializeData(SerializeListener& sl, MeshHeader& mehHeader, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(mehHeader.m_nIdent,"FileIden");
		sl.Serialize(mehHeader.m_nVersion,"Version");	

		sl.EndSection();
	}

	void SerializeData(SerializeListener& sl, SubMeshData& subMeshData, const char* pszLabel)
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

// 	void SerializeData(SerializeListener& sl, VertexGENERAL& vertexGeneral, const char* pszLabel)
// 	{
// 		sl.BeginSection(pszLabel);
// 
// 		sl.Serialize(vertexGeneral.m_position,"m_position");
// 		sl.Serialize(vertexGeneral.m_normal,"m_normal");
// 		sl.Serialize(vertexGeneral.m_uv,"m_uv");
// 		
// 		sl.EndSection();
// 	}
// 
// 	void SerializeData(SerializeListener& sl, VertexHWSKIN& vertexSkin, const char* pszLabel)
// 	{
// 		sl.BeginSection(pszLabel);
// 
// 		sl.Serialize(vertexSkin.m_boneID,"m_boneID");
// 		sl.Serialize(vertexSkin.m_weight,"m_weight");
// 
// 		sl.EndSection();
// 	}


	void SerializeDataV0(SerializeListener& sl, MeshData& meshData, const char* pszLabel)
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


	void MeshData::Serialize(SerializeListener& sl, const char* pszLable)
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

