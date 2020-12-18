#include "MeshComponent.h"
#include "../RenderSystem/RenderQueue.h"
#include "../RenderSystem/RenderThread.h"

namespace ma
{
	MeshComponent::MeshComponent()
	{
		m_pRenderproxy = new MeshRenderProxy();
	}

	MeshComponent::~MeshComponent()
	{

	}

	void MeshComponent::RegisterAttribute()
	{
		REGISTER_OBJECT(MeshComponent,CreateMeshComponent); 

		ACCESSOR_ATTRIBUTE(MeshComponent, "MtgPath", GetMaterialFile, SetMaterialFile, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(MeshComponent, "MesPath", GetMeshFile, SetMeshFile, const char*, NULL, AM_DEFAULT);
	}

	bool MeshComponent::Load(const char* pszSknPath,const char* pszMatPath)
	{
		SetMaterialFile(pszMatPath);
		
		SetMeshFile(pszSknPath);

		return true;
	}

	MeshData* MeshComponent::GetMeshData()
	{
		return m_pMesData ? m_pMesData.get() : nullptr;
	}

	const char*	MeshComponent::GetMeshFile() const
	{
		return m_pMesData ? m_pMesData->GetResPath() : nullptr;  
	}

	void MeshComponent::SetMeshFile(const char* pFile)
	{
		CreateMeshData(pFile, [this](Resource* res) {
				m_pMesData = (MeshData*)res;
				this->IsReady(); 
			}
		);
	}

	const char*	 MeshComponent::GetMaterialFile() const
	{
		return m_pMaterial ? m_pMaterial->GetResPath() : nullptr;
	}

	void MeshComponent::SetMaterialFile(const char* pFile)
	{
		CreateMaterial(pFile, [this](Resource* res) {
				RefPtr<Material> pMaterila = (Material*)res;
				m_pMaterial = pMaterila->Clone();
				this->IsReady();
			}
		);
	}

	void MeshComponent::SetMaterial(Material* pMaterial)
	{
		m_pMaterial = pMaterial;
	}



	void MeshComponent::SetShadowCaster(bool b)
	{
		RenderComponent::SetShadowCaster(b);
	}

	void MeshComponent::SetSuportInstance(bool b)
	{
		m_bSuportInstance = b;

		for (uint32_t iSub = 0; iSub < m_arrRenderable.size(); ++iSub)
		{
			m_arrRenderable[iSub]->m_bSuportInstace = b;
		}
	}

	uint32_t MeshComponent::GetSubMaterialCount()
	{
		return m_arrRenderable.size();
	}

	SubMaterial* MeshComponent::GetSubMaterial(uint32_t index)
	{
		return m_pMaterial->GetSubByIndex(index);
	}

	void MeshComponent::Update()
	{
		if (m_pMesData == nullptr || m_pMaterial == nullptr)
			return;

		RenderComponent::Update();

		for (auto& pRenderObj : m_arrRenderable)
		{
			pRenderObj->SetWorldMatrix(m_pSceneNode->GetMatrixWS());
		}
	}

	RefPtr<MeshRenderable> MeshComponent::CreateMeshRenderable()
	{
		return new MeshRenderable();
	}

	void MeshComponent::CreateRenderable()
	{
		ASSERT(m_pMaterial && m_pMesData);
		if (m_pMaterial == nullptr || m_pMesData == nullptr)
			return;
		
		MeshRenderProxy* pMeshRenderProxy = dynamic_cast<MeshRenderProxy*>(m_pRenderproxy.get());
		ASSERT(pMeshRenderProxy);

		pMeshRenderProxy->Clear();

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

			pMeshRenderProxy->AddRenderable(pRenderable.get());
		}
	}

	bool MeshComponent::IsReady()
	{
		if (m_pMaterial == nullptr || m_pMesData == nullptr)
			return false;

		CreateRenderable();

		SetAABB(m_pMesData->GetBoundingAABB());

		return true;
	}

	void MeshComponent::Render(RenderQueue* pRenderQueue,RenderPass* pRenderPass)
	{
		for (auto& pRenderObj : m_arrRenderable)
		{
			Technique* Tech = pRenderObj->GetMaterial()->GetTechnqiue(pRenderPass);
			if (Tech == nullptr)
			{
				continue;
			}

			pRenderQueue->AddRenderObj(RL_Mesh, pRenderObj.get(), Tech);
		}
	}

	RefPtr<MeshComponent> CreateMeshComponent()
	{
		return new MeshComponent();
	}


	uint32_t MeshRenderProxy::GetRenderableCount() const
	{
		return m_arrRenderable.size();
	}

	Renderable* MeshRenderProxy::GetRenderableByIndex(uint32_t index) const
	{
		return m_arrRenderable[index].get();
	}

	void MeshRenderProxy::AddRenderable(MeshRenderable* renderable)
	{
		m_arrRenderable.push_back(renderable);
	}

	void MeshRenderProxy::Clear()
	{
		m_arrRenderable.clear();
	}

}

