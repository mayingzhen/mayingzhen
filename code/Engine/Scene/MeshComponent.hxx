#include "MeshComponent.h"
#include "../RenderSystem/RenderQueue.h"
#include "../RenderSystem/RenderThread.h"

namespace ma
{
	MeshComponent::MeshComponent()
	{
		m_nLod = 0;
		m_bOnLoadOver = false;
	}

	MeshComponent::~MeshComponent()
	{

	}

	void MeshComponent::RegisterAttribute()
	{
		REGISTER_OBJECT(MeshComponent,CreateMeshComponent); 

		ACCESSOR_ATTRIBUTE(MeshComponent, "MatPath", GetMaterialFile, SetMaterialFile, const char*, NULL, AM_DEFAULT);
	}

	bool MeshComponent::Import(rapidxml::xml_node<>* pXmlElem)
	{
		Serializable::Import(pXmlElem);
	
		rapidxml::xml_node<>* pXmlLodMeshData = pXmlElem->first_node("LodMeshData");
		while(pXmlLodMeshData)
		{
			const char* pszFile = pXmlLodMeshData->findAttribute("MeshData");
			
			RefPtr<MeshData> pMeshData = CreateMeshData(pszFile);

			m_vecMesData.push_back(pMeshData);
		
			pXmlLodMeshData = pXmlElem->next_sibling("LodMeshData");
		}

		return true;
	}

	bool MeshComponent::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		Serializable::Export(pXmlElem,doc);

		for (UINT i = 0; i < m_vecMesData.size(); ++i)
		{
			rapidxml::xml_node<>* pXmlLodMeshData = doc.allocate_node(rapidxml::node_element, doc.allocate_string("LodMeshData"));
			pXmlElem->append_node(pXmlLodMeshData);

			pXmlLodMeshData->append_attribute(doc.allocate_attribute(doc.allocate_string("MeshData"),doc.allocate_string(m_vecMesData[i]->GetResPath())));
		}

		return true;
	}

	bool MeshComponent::Load(const char* pszSknPath,const char* pszMatPath)
	{
		SetMaterialFile(pszMatPath);
		
		SetMeshFile(pszSknPath,0);

		return IsReady();
	}

	const char*	MeshComponent::GetMeshFile(uint32 nLod) const
	{
		if (nLod < 0 || nLod >= m_vecMesData.size())
			return NULL;

		return m_vecMesData[nLod] ? m_vecMesData[nLod]->GetResPath() : NULL;
	}

	void MeshComponent::SetMeshFile(const char* pFile,uint32 nLod)
	{
		if (nLod >= m_vecMesData.size())
		{
			m_vecMesData.resize(nLod + 1);
		}

		RefPtr<MeshData> pMeshData = CreateMeshData(pFile);
		m_vecMesData[nLod] = pMeshData;

		m_bOnLoadOver = false;
		IsReady();
	}

	const char*	 MeshComponent::GetMaterialFile() const
	{
		return m_pMaterial ? m_pMaterial->GetResPath() : NULL;
	}

	void MeshComponent::SetMaterialFile(const char* pFile)
	{
		SetMaterial( CreateMaterial(pFile).get() );
	}

	void MeshComponent::SetMaterial(Material* pMaterial)
	{
		m_pMaterial = pMaterial;

		m_bOnLoadOver = false;
		IsReady();
	}

	UINT MeshComponent::GetRenderableCount(uint32 nLod) const
	{
		return m_arrLodRenderable[nLod].size();
	}

	Renderable*	MeshComponent::GetRenderableByIndex(uint32 nLod,UINT index) const
	{
		return m_arrLodRenderable[nLod][index].get();
	}

	UINT32 MeshComponent::GetSubMaterialCount(uint32 nLod)
	{
		return m_arrLodRenderable[nLod].size();
	}

	SubMaterial* MeshComponent::GetSubMaterial(uint32 nLod,UINT index)
	{
		return m_arrLodRenderable[index][index]->GetMaterial();
	}

	void MeshComponent::Update()
	{
		IsReady();

		RenderComponent::Update();
	}

	void MeshComponent::CreateRenderable()
	{
		ASSERT(m_pMaterial && !m_vecMesData.empty());
		if (m_pMaterial == NULL || m_vecMesData.empty())
			return;
		
		m_arrLodRenderable.clear();
		for (uint32 iLod = 0; iLod < m_vecMesData.size(); ++iLod)
		{
			MeshData* pMesData = m_vecMesData[iLod].get();

			VEC_RENDERABLE arrRenderable;
			for (UINT iSub = 0; iSub < pMesData->GetSubMeshNumber(); ++iSub)
			{
				MeshRenderable* pRenderable = new MeshRenderable();

				pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
				pRenderable->m_pDeclaration = pMesData->GetVertexDeclar(); 
				pRenderable->m_pVertexBuffer = pMesData->GetVertexBuffer(); 
				pRenderable->m_pIndexBuffer = pMesData->GetIndexBuffer();
				pRenderable->m_pSubMeshData = pMesData->GetSubMeshByIndex(iSub);
				pRenderable->m_posAABB = pMesData->GetBoundingAABB();
				pRenderable->m_tcAABB = pMesData->GetUVBoundingAABB();

				pRenderable->m_pSubMaterial = m_pMaterial->GetSubMaterialByIndex(iLod,iSub); 

				arrRenderable.push_back(pRenderable);
			}
			m_arrLodRenderable.push_back(arrRenderable);
		}

		SetAABB(m_vecMesData[0]->GetBoundingAABB());
	}

	bool MeshComponent::IsReady()
	{
		if (m_bOnLoadOver)
			return true;

		if (m_vecMesData.empty() || m_vecMesData[m_nLod] == NULL || !m_vecMesData[m_nLod]->IsReady())
			return false;

		if (m_pMaterial == NULL || !m_pMaterial->IsReady())
			return false;

		CreateRenderable();
		
		MarkDirty();

		m_bOnLoadOver = true;
		
		return true;
	}

	void MeshComponent::Show(Camera* pCamera)
	{
		RenderComponent::Show(pCamera);

		if ( m_arrLodRenderable.empty() )
			return;

		uint32 nLod = 0;

		for (UINT i = 0; i < m_arrLodRenderable[nLod].size(); ++i)
		{
			 m_arrLodRenderable[nLod][i]->SetWorldMatrix( m_pSceneNode->GetMatrixWS() );

			m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Mesh, m_arrLodRenderable[nLod][i].get());
		}
	}

	RefPtr<MeshComponent> CreateMeshComponent()
	{
		return new MeshComponent();
	}

}

