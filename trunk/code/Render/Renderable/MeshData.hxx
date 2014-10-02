#include "MeshData.h"
#include "Engine/RTTI/Object.h"

#include "MeshDataSerialize.hxx"


namespace ma
{
	IMPL_OBJECT(MeshData,Resource)

	MeshData::MeshData(const char* pszPath)
	:Resource(pszPath)
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

	void MeshData::Serialize(Serializer& sl, const char* pszLable)
	{
		MeshHeader header;
		sl.Serialize(header,"MeshHeader");
		if (header.m_nIdent != 'MAMD')
			return;

		if (header.m_nVersion == MESH_VER_INITIAL)
		{
			SerializeDataV0(sl,*this,pszLable);
		}

		if (sl.IsReading())
		{
			m_pDeclaration->Init(m_nVertexType);
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

