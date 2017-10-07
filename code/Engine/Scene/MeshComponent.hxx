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

	UINT MeshComponent::GetRenderableCount(uint32 nLod) const
	{
		return m_arrLodRenderable[nLod].size();
	}

	Renderable*	MeshComponent::GetRenderableByIndex(uint32 nLod,UINT index) const
	{
		return m_arrLodRenderable[nLod][index].get();
	}

	UINT MeshComponent::GetShadowRenderableCount(uint32 nLod) const
	{
		return m_arrLodShadowRenderable[nLod].size();
	}

	Renderable* MeshComponent::GetShadowRenderableByIndex(uint32 nLod, UINT index) const
	{
		return m_arrLodShadowRenderable[nLod][index].get();
	}

	void MeshComponent::SetShadowCaster(bool b)
	{
		RenderComponent::SetShadowCaster(b);
		if (b)
		{
			CreateRenderable(m_arrLodShadowRenderable, RP_ShadowDepth);
		}
	}

	UINT32 MeshComponent::GetSubMaterialCount(uint32 nLod)
	{
		return m_arrLodRenderable[nLod].size();
	}

	SubMaterial* MeshComponent::GetSubMaterial(uint32 nLod,UINT index)
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

	void MeshComponent::CreateRenderable(VEC_LOD_RENDERABLE& arrLodRenderable, RenderPassType ePassType)
	{
		ASSERT(m_pMaterial && !m_vecMesData.empty());
		if (m_pMaterial == NULL || m_vecMesData.empty())
			return;
		
		arrLodRenderable.clear();
		for (uint32 iLod = 0; iLod < m_vecMesData.size(); ++iLod)
		{
			MeshData* pMesData = m_vecMesData[iLod].get();

			VEC_RENDERABLE arrRenderable;
			for (UINT iSub = 0; iSub < pMesData->GetSubMeshNumber(); ++iSub)
			{
				RefPtr<MeshRenderable> pRenderable = CreateMeshRenderable();

				pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
				pRenderable->m_pVertexBuffer = pMesData->GetVertexBuffer(); 
				pRenderable->m_pIndexBuffer = pMesData->GetIndexBuffer();
				pRenderable->m_pSubMeshData = pMesData->GetSubMeshByIndex(iSub);

				SubMaterial* pSubMaterial = m_pMaterial->GetLodSubByIndex(iLod, iSub);

				Technique* pTech = pSubMaterial->GetShadingTechnqiue();
				if (ePassType == RP_ShadowDepth)
				{
					pTech = pSubMaterial->GetShadowDepthTechnqiue();
				}

				pRenderable->m_Technique = pTech;

				Vector3 pos_extent = pMesData->GetBoundingAABB().getHalfSize();
				Vector3 pos_center = pMesData->GetBoundingAABB().getCenter();
				Vector2 tc_extent = pMesData->GetUVBoundingAABB().getHalfSize();
				Vector2	tc_center = pMesData->GetUVBoundingAABB().getCenter();
				Vector4 tc_extent_center = Vector4(tc_extent.x,tc_extent.y,tc_center.x,tc_center.y);

				pTech->SetValue( pTech->GetUniform("pos_extent"), pos_extent );
				pTech->SetValue( pTech->GetUniform("pos_center"), pos_center );
				pTech->SetValue( pTech->GetUniform("tc_extent_center"), tc_extent_center );

				Technique* pInstTech = pTech->GetInstTech();
				if (pInstTech)
				{
					pInstTech->SetValue(pInstTech->GetUniform("pos_extent"), pos_extent);
					pInstTech->SetValue(pInstTech->GetUniform("pos_center"), pos_center);
					pInstTech->SetValue(pInstTech->GetUniform("tc_extent_center"), tc_extent_center);
				}

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

		CreateRenderable(m_arrLodRenderable,RP_Shading);

		SetAABB(m_vecMesData[0]->GetBoundingAABB());

		m_bOnLoadOver = true;
		
		return true;
	}

	void MeshComponent::Show(Camera* pCamera)
	{
		RenderComponent::Show(pCamera);

		if (!m_bOnLoadOver)
			return;

		for (UINT i = 0; i < m_arrLodRenderable[m_nLod].size(); ++i)
		{
			 m_arrLodRenderable[m_nLod][i]->SetWorldMatrix( m_pSceneNode->GetMatrixWS() );

			m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Mesh, m_arrLodRenderable[m_nLod][i].get());
		}

		if (m_nLod < m_arrLodShadowRenderable.size())
		{
			for (UINT i = 0; i < m_arrLodShadowRenderable[m_nLod].size(); ++i)
			{
				m_arrLodShadowRenderable[m_nLod][i]->SetWorldMatrix(m_pSceneNode->GetMatrixWS());
			}
		}
	}

	RefPtr<MeshComponent> CreateMeshComponent()
	{
		return new MeshComponent();
	}

}

