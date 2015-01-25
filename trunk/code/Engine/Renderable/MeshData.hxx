#include "MeshData.h"
#include "MeshDataSerialize.hxx"


namespace ma
{
	IMPL_OBJECT(MeshData,Resource)

	MeshData::MeshData(const char* pszPath)
	:Resource(pszPath)
	{
		m_eCreateType = BIN_RES;
		m_nIndexType = 0;
		m_nVertexType = 0;
		m_pIndexBuffer = GetRenderDevice()->CreateIndexBuffer();
		m_pVertexBuffer = GetRenderDevice()->CreateVertexBuffer();
		m_pDeclaration = GetRenderDevice()->CreateVertexDeclaration();	
	}

	MeshData::~MeshData()
	{
	}

	SubMeshData* MeshData::AddSubMeshData()
	{
		SubMeshData* pSubMeshData = new SubMeshData();
		m_arrSubMesh.push_back(pSubMeshData);
		return pSubMeshData;
	}

	void MeshData::Serialize(Serializer& sl, const char* pszLable)
	{
		MeshHeader header;
		sl.Serialize(header,"MeshHeader");
		if (header.m_nIdent != 'MAMD')
			return;

		if (header.m_nVersion == MESH_VER_INITIAL)
		{
			SerializeDataV0(sl,pszLable);
		}

		if (sl.IsReading())
		{
			m_pDeclaration->Init(m_nVertexType);

			GetRenderSystem()->VertexDeclaComplete(m_pDeclaration.get());
			
			GetRenderSystem()->HardwareBufferStreamComplete(m_pIndexBuffer.get());

			GetRenderSystem()->HardwareBufferStreamComplete(m_pVertexBuffer.get());
		}
	}

	void SubMeshData::AddBonePalette(BoneIndex bonde)
	{
		m_arrBonePalette.push_back(bonde);
	}
}

