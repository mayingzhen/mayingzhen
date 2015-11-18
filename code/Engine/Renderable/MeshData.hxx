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

	SubMeshData* MeshData::AddSubMeshData()
	{
		SubMeshData* pSubMeshData = new SubMeshData();
		m_arrSubMesh.push_back(pSubMeshData);
		return pSubMeshData;
	}

	void MeshData::Improt(TiXmlElement* pXmlObject)
	{

	}

	void MeshData::Export(TiXmlElement* pXmlObject)
	{

	}

	bool MeshData::InitRes()
	{
		TiXmlDocument doc;
		bool bLoadOK = doc.Parse( (const char*)m_pDataStream->GetPtr() ) != NULL;
		ASSERT(bLoadOK);
		if (!bLoadOK)
			return false;

		TiXmlElement* pXmlRoot = doc.FirstChildElement();
		ASSERT(pXmlRoot);
		this->Improt(pXmlRoot);

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

