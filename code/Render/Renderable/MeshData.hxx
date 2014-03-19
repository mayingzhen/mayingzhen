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

	bool MeshData::InitRendable()
	{
// 		void* pIndexData =  &m_arrIndexBuffer[0];
// 		int nIndexSize = m_arrIndexBuffer.size();
// 		INDEX_TYPE eIndexType = m_nIndexType == INDEX_TYPE_U16 ? INDEX_TYPE_U16 : INDEX_TYPE_U32; 
// 		m_pIndexBuffer = GetRenderDevice()->CreateIndexBuffer(pIndexData,nIndexSize,eIndexType);
// 
// 		void* pVertexData =  &m_arrVertexBuffer[0];
// 		int nVertexDataSize = m_arrVertexBuffer.size();
// 		int nVertexStride = GetVertexStride();
// 		m_pVertexBuffer = GetRenderDevice()->CreateVertexBuffer(pVertexData,nVertexDataSize, nVertexStride);
// 
 		
 		m_pDeclaration->Init(m_nVertexType);

		return true;
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

		if (sl.IsReading())
		{
			InitRendable();
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

