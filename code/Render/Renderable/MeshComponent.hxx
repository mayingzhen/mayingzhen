#include "MeshComponent.h"
#include "../RenderSystem/RenderQueue.h"
#include "../RenderSystem/RenderThread.h"

namespace ma
{
	IMPL_OBJECT(MeshComponent,RenderComponent)

	MeshComponent::MeshComponent(SceneNode* pGameObj):
		RenderComponent(pGameObj)
	{
		//m_pElMeshLoaded = new ELMeshComponent(this,&MeshComponent::CreateRenderable);
		//m_pElMaterialLoaded = new ELMeshComponent(this,&MeshComponent::CreateMaterial);

		m_bOnLoadOver = false;
	}

	bool MeshComponent::Load(const char* pszSknPath,const char* pszMatPath)
	{
		m_pMesData = LoadResource<MeshData>(pszSknPath);

		m_pMatData = LoadResource<MaterialData>(pszMatPath);

		return true;
	}

	void MeshComponent::Update()
	{
		this->OnLoadOver();
	}


	void MeshComponent::CreateMaterial(void* parm1,void* parm2)
	{
		if (m_pMatData == NULL)
			return;

		for (UINT i = 0; i < m_pMatData->GetSubMatDataNumber(); ++i)
		{
			Material* pMaterial = new Material();

			pMaterial->InitWithSubMatData(m_pMatData->GetSubMatDataByIndex(i));

			m_arrMaterial.push_back(pMaterial);
		}
	}

	void MeshComponent::CreateRenderable(void* parm1,void* parm2)
	{
		if (m_pMesData == NULL)
			return;

		for (UINT i = 0; i < m_pMesData->GetSubMeshNumber(); ++i)
		{
			Renderable* pRenderable = new Renderable();

			pRenderable->InitWithMeshData(*m_pMesData,i);

			m_arrRenderable.push_back(pRenderable);
		}

		m_AABB = m_pMesData->GetBoundingAABB();
		m_worldAABB = m_AABB;
		m_worldAABB.transform( m_pSceneNode->GetWorldMatrix() );

		OnTransformChange();
	}

	bool MeshComponent::OnLoadOver()
	{
		if (m_bOnLoadOver)
			return true;

		if (m_pMesData->GetResState() != ResInited || m_pMatData->GetResState() != ResInited)
			return false;

		CreateRenderable(NULL,NULL);

		CreateMaterial(NULL,NULL);

		m_bOnLoadOver = true;

		return true;
	}


	void MeshComponent::Show(Camera* pCamera)
	{
		RenderComponent::Show(pCamera);

		if ( m_arrRenderable.empty() || m_arrMaterial.empty() )
			return;

		ASSERT(m_arrRenderable.size() == m_arrMaterial.size());
		if (m_arrRenderable.size() != m_arrMaterial.size())
			return;

		int index = GetRenderSystem()->CurThreadFill();

		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			m_arrRenderable[i]->m_matWorld[index] = m_pSceneNode->GetWorldMatrix();
			m_arrRenderable[i]->m_pMaterial = m_arrMaterial[i];

			GetRenderSystem()->GetRenderQueue()->AddRenderObj(RL_Solid,m_arrRenderable[i]);
		}
	}

	void MeshComponent::Serialize(Serializer& sl, const char* pszLable/* = "MeshComponent"*/)
	{
		sl.BeginSection(pszLable);

		std::string strSknPath = m_pMesData ? m_pMesData->GetResPath() : "";
		std::string strMatPath = m_pMatData ? m_pMatData->GetResPath() : "";

		sl.Serialize(strSknPath,"SknPath");
		sl.Serialize(strMatPath,"MatPath");

		if ( sl.IsReading() )
		{
			Load(strSknPath.c_str(),strMatPath.c_str());
		}	

		sl.EndSection();
	}

	void MeshComponent::SetSkinMatrix(const Matrix4* arrMatrixs,Uint nCount)
	{
		profile_code();
		
		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			m_arrRenderable[i]->SetSkinMatrix(arrMatrixs,nCount);
		}
	}

}

