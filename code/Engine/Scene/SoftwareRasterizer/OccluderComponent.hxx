#include "OccluderComponent.h"

namespace ma
{
	OccluderComponent::OccluderComponent()
	{
		m_bOnLoadOver = false;
	}

	OccluderComponent::~OccluderComponent()
	{

	}

	void OccluderComponent::RegisterAttribute()
	{
		REGISTER_OBJECT(OccluderComponent, CreateOccluderComponent);

		ACCESSOR_ATTRIBUTE(OccluderComponent, "MtgPath", GetMaterialFile, SetMaterialFile, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(OccluderComponent, "MesPath", GetMeshFile, SetMeshFile, const char*, NULL, AM_DEFAULT);
	}

	bool OccluderComponent::Load(const char* pszSknPath,const char* pszMatPath)
	{
		SetMaterialFile(pszMatPath);
		
		SetMeshFile(pszSknPath);

		return IsReady();
	}

	MeshData* OccluderComponent::GetMeshData()
	{
		return m_pMesData ? m_pMesData.get() : nullptr;
	}

	const char*	OccluderComponent::GetMeshFile() const
	{
		return m_pMesData ? m_pMesData->GetResPath() : nullptr;  
	}

	void OccluderComponent::SetMeshFile(const char* pFile)
	{
		m_pMesData = CreateMeshData(pFile);

		m_bOnLoadOver = false;
		IsReady();
	}

	const char*	 OccluderComponent::GetMaterialFile() const
	{
		return m_pMaterial ? m_pMaterial->GetResPath() : nullptr;
	}

	void OccluderComponent::SetMaterialFile(const char* pFile)
	{
		RefPtr<Material> pMaterilaTemp = CreateMaterial(pFile);
		RefPtr<Material> pMaterila = pMaterilaTemp->Clone();
		SetMaterial(pMaterila.get());
	}

	void OccluderComponent::SetMaterial(Material* pMaterial)
	{
		m_pMaterial = pMaterial;

		m_bOnLoadOver = false;
		IsReady();
	}

	uint32_t OccluderComponent::GetRenderableCount() const
	{
		return m_arrRenderable.size();
	}

	Renderable*	OccluderComponent::GetRenderableByIndex(uint32_t index) const
	{
		return m_arrRenderable[index].get();
	}

	uint32_t OccluderComponent::GetShadowRenderableCount() const
	{
		uint32_t nLod = m_arrRenderable.size() - 1;
		return m_arrRenderable.size();
	}

	Renderable*	OccluderComponent::GetShadowRenderableByIndex(uint32_t index) const
	{
		uint32_t nLod = m_arrRenderable.size() - 1;
		return m_arrRenderable[index].get();
	}

	void OccluderComponent::SetShadowCaster(bool b)
	{
		RenderComponent::SetShadowCaster(b);
	}

	void OccluderComponent::SetSuportInstance(bool b)
	{
		m_bSuportInstance = b;

		for (uint32_t iSub = 0; iSub < m_arrRenderable.size(); ++iSub)
		{
			m_arrRenderable[iSub]->m_bSuportInstace = b;
		}
	}

	uint32_t OccluderComponent::GetSubMaterialCount()
	{
		return m_arrRenderable.size();
	}

	SubMaterial* OccluderComponent::GetSubMaterial(uint32_t index)
	{
		return m_pMaterial->GetSubByIndex(index);
	}

	void OccluderComponent::Update()
	{
		IsReady();

		RenderComponent::Update();
	}

	RefPtr<MeshRenderable> OccluderComponent::CreateMeshRenderable()
	{
		return new MeshRenderable();
	}

	void OccluderComponent::CreateRenderable(VEC_RENDERABLE& arrRenderable)
	{
		ASSERT(m_pMaterial && m_pMesData);
		if (m_pMaterial == nullptr || m_pMesData == nullptr)
			return;
		
		arrRenderable.clear();

		for (uint32_t iSub = 0; iSub < m_pMesData->GetSubMeshNumber(); ++iSub)
		{
			RefPtr<MeshRenderable> pRenderable = CreateMeshRenderable();

			pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
			pRenderable->m_pVertexBuffer = m_pMesData->GetVertexBuffer();
			pRenderable->m_pIndexBuffer = m_pMesData->GetIndexBuffer();
			pRenderable->m_pSubMeshData = m_pMesData->GetSubMeshByIndex(iSub);

			pRenderable->m_bSuportInstace = m_bSuportInstance;

			SubMaterial* pSubMaterial = m_pMaterial->GetSubByIndex(iSub);
			pRenderable->m_pSubMaterial = pSubMaterial;

			Vector3 pos_extent = m_pMesData->GetBoundingAABB().getHalfSize();
			Vector3 pos_center = m_pMesData->GetBoundingAABB().getCenter();
			Vector2 tc_extent = m_pMesData->GetUVBoundingAABB().getHalfSize();
			Vector2	tc_center = m_pMesData->GetUVBoundingAABB().getCenter();
			Vector4 tc_extent_center = Vector4(tc_extent.x, tc_extent.y, tc_center.x, tc_center.y);

			pSubMaterial->SetParameter("pos_extent", Any(pos_extent));
			pSubMaterial->SetParameter("pos_center", Any(pos_center));
			pSubMaterial->SetParameter("tc_extent_center", Any(tc_extent_center));

			arrRenderable.push_back(pRenderable);
		}
	}

	bool OccluderComponent::IsReady()
	{
		if (m_bOnLoadOver)
			return true;

		if (m_pMesData == nullptr || !m_pMesData->IsReady())
			return false;

		if (m_pMaterial == nullptr || !m_pMaterial->IsReady())
			return false;

		CreateRenderable(m_arrRenderable);

		SetAABB(m_pMesData->GetBoundingAABB());

		m_bOnLoadOver = true;
		
		return true;
	}

	void OccluderComponent::Render(RenderQueue* pRenderQueue)
	{
		if (!m_bOnLoadOver)
			return;

		for (uint32_t i = 0; i < m_arrRenderable.size(); ++i)
		{
			m_arrRenderable[i]->SetWorldMatrix( m_pSceneNode->GetMatrixWS() );

			Renderable* pRenderObj = m_arrRenderable[i].get();

			pRenderQueue->AddRenderObj(RL_Mesh, pRenderObj, pRenderObj->GetMaterial()->GetShadingTechnqiue());
		}
	}

	void OccluderComponent::RenderShadow(RenderQueue* pRenderQueue)
	{
		if (!m_bOnLoadOver)
			return;

		for (uint32_t i = 0; i < m_arrRenderable.size(); ++i)
 		{
			Renderable* pRenderObj = m_arrRenderable[i].get();

 			pRenderQueue->AddRenderObj(RL_Mesh, pRenderObj, pRenderObj->GetMaterial()->GetShadowDepthTechnqiue());
 		}
	}

	RefPtr<OccluderComponent> CreateOccluderComponent()
	{
		return new OccluderComponent();
	}

}

