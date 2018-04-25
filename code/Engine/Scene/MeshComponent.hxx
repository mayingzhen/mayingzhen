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
		
		IsReady();

		return true;
	}

	bool MeshComponent::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		Serializable::Export(pXmlElem,doc);

		for (uint32_t i = 0; i < m_vecMesData.size(); ++i)
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

	MeshData* MeshComponent::GetMeshData(uint32_t nLod)
	{
		if (nLod < 0 || nLod >= m_vecMesData.size())
			return NULL;

		return m_vecMesData[nLod].get();
	}

	const char*	MeshComponent::GetMeshFile(uint32_t nLod) const
	{
		if (nLod < 0 || nLod >= m_vecMesData.size())
			return NULL;

		return m_vecMesData[nLod] ? m_vecMesData[nLod]->GetResPath() : NULL;
	}

	void MeshComponent::SetMeshFile(const char* pFile,uint32_t nLod)
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
		RefPtr<Material> pMaterilaTemp = CreateMaterial(pFile);
		RefPtr<Material> pMaterila = pMaterilaTemp->Clone();
		SetMaterial(pMaterila.get());
	}

	void MeshComponent::SetMaterial(Material* pMaterial)
	{
		m_pMaterial = pMaterial;

		m_bOnLoadOver = false;
		IsReady();
	}

	uint32_t MeshComponent::GetRenderableCount(uint32_t nLod) const
	{
		return m_arrLodRenderable[nLod].size();
	}

	Renderable*	MeshComponent::GetRenderableByIndex(uint32_t nLod,uint32_t index) const
	{
		return m_arrLodRenderable[nLod][index].get();
	}

	uint32_t MeshComponent::GetShadowRenderableCount() const
	{
		uint32_t nLod = m_arrLodRenderable.size() - 1;
		return m_arrLodRenderable[nLod].size();
	}

	Renderable*	MeshComponent::GetShadowRenderableByIndex(uint32_t index) const
	{
		uint32_t nLod = m_arrLodRenderable.size() - 1;
		return m_arrLodRenderable[nLod][index].get();
	}

	void MeshComponent::SetShadowCaster(bool b)
	{
		RenderComponent::SetShadowCaster(b);
	}

	void MeshComponent::SetSuportInstance(bool b)
	{
		m_bSuportInstance = b;

		for (uint32_t iLod = 0; iLod < m_arrLodRenderable.size(); ++iLod)
		{
			VEC_RENDERABLE arrRenderable = m_arrLodRenderable[iLod];
			for (uint32_t iSub = 0; iSub < arrRenderable.size(); ++iSub)
			{
				arrRenderable[iSub]->m_bSuportInstace = b;
			}
		}
	}

	uint32_t MeshComponent::GetSubMaterialCount(uint32_t nLod)
	{
		return m_arrLodRenderable[nLod].size();
	}

	SubMaterial* MeshComponent::GetSubMaterial(uint32_t nLod,uint32_t index)
	{
		return m_pMaterial->GetLodSubByIndex(nLod,index);
	}

	void MeshComponent::Update()
	{
		IsReady();

		RenderComponent::Update();
	}

	RefPtr<MeshRenderable> MeshComponent::CreateMeshRenderable()
	{
		return new MeshRenderable();
	}

	void MeshComponent::CreateRenderable(VEC_LOD_RENDERABLE& arrLodRenderable)
	{
		ASSERT(m_pMaterial && !m_vecMesData.empty());
		if (m_pMaterial == NULL || m_vecMesData.empty())
			return;
		
		arrLodRenderable.clear();
		for (uint32_t iLod = 0; iLod < m_vecMesData.size(); ++iLod)
		{
			MeshData* pMesData = m_vecMesData[iLod].get();

			VEC_RENDERABLE arrRenderable;
			for (uint32_t iSub = 0; iSub < pMesData->GetSubMeshNumber(); ++iSub)
			{
				RefPtr<MeshRenderable> pRenderable = CreateMeshRenderable();

				pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
				pRenderable->m_pVertexBuffer = pMesData->GetVertexBuffer(); 
				pRenderable->m_pIndexBuffer = pMesData->GetIndexBuffer();
				pRenderable->m_pSubMeshData = pMesData->GetSubMeshByIndex(iSub);

				pRenderable->m_bSuportInstace = m_bSuportInstance;

				SubMaterial* pSubMaterial = m_pMaterial->GetLodSubByIndex(iLod, iSub);
				pRenderable->m_pSubMaterial = pSubMaterial;

				Vector3 pos_extent = pMesData->GetBoundingAABB().getHalfSize();
				Vector3 pos_center = pMesData->GetBoundingAABB().getCenter();
				Vector2 tc_extent = pMesData->GetUVBoundingAABB().getHalfSize();
				Vector2	tc_center = pMesData->GetUVBoundingAABB().getCenter();
				Vector4 tc_extent_center = Vector4(tc_extent.x,tc_extent.y,tc_center.x,tc_center.y);

				pSubMaterial->SetParameter( "pos_extent", Any(pos_extent) );
				pSubMaterial->SetParameter( "pos_center", Any(pos_center) );
				pSubMaterial->SetParameter( "tc_extent_center", Any(tc_extent_center) );

				arrRenderable.push_back(pRenderable);
			}
			arrLodRenderable.push_back(arrRenderable);
		}
	}

	bool MeshComponent::IsReady()
	{
		if (m_bOnLoadOver)
			return true;

		if (m_vecMesData.empty() || m_vecMesData[m_nLod] == NULL || !m_vecMesData[m_nLod]->IsReady())
			return false;

		if (m_pMaterial == NULL || !m_pMaterial->IsReady())
			return false;

		CreateRenderable(m_arrLodRenderable);

		SetAABB(m_vecMesData[0]->GetBoundingAABB());

		m_bOnLoadOver = true;
		
		return true;
	}

	void MeshComponent::Show(Camera* pCamera)
	{
		RenderComponent::Show(pCamera);

		if (!m_bOnLoadOver)
			return;

		for (uint32_t i = 0; i < m_arrLodRenderable[m_nLod].size(); ++i)
		{
			 m_arrLodRenderable[m_nLod][i]->SetWorldMatrix( m_pSceneNode->GetMatrixWS() );

			m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Mesh, m_arrLodRenderable[m_nLod][i].get());
		}
	}

	RefPtr<MeshComponent> CreateMeshComponent()
	{
		return new MeshComponent();
	}

}

