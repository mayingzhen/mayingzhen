#include "MeshComponent.h"

namespace ma
{
	IMPL_OBJECT(MeshComponent,Component)

	MeshComponent::MeshComponent(GameObject* pGameObj)
	:Component(pGameObj)
	{
		m_pRendMesh = NULL;
		m_pMaterial = NULL;
	}

	MeshComponent::~MeshComponent()
	{

	}

	void MeshComponent::Render()
	{
		if (m_pRendMesh == NULL)
			return;

		m_pRendMesh->Draw();
	}

	void MeshComponent::Update()
	{
		if (m_pRendMesh == NULL || m_pGameObject == NULL)
			return;	

		m_pRendMesh->SetWorldMatrix( m_pGameObject->GetSceneNode()->GetWorldMatrix() );		
	}

	void MeshComponent::Start()
	{
	}

	void MeshComponent::Stop()
	{

	}

	void MeshComponent::Load(const char* pszMeshPath,const char* pszTexPath)
	{
		IRenderDevice* pRenderDevice = ma::GetRenderDevice();
		if (pRenderDevice == NULL)
			return;

		m_strMeshPath = pszMeshPath;
		m_strMaterialPath = pszTexPath;

		m_pRendMesh = new RenderMesh();
		m_pRendMesh->Load(pszMeshPath,pszTexPath);
	}	

	void MeshComponent::GetBoundingAABB(Vector3& vMin,Vector3& vMax)
	{
// 		if (m_pRendMesh)
// 		{
// 			m_pRendMesh->GetBoundingAABB(vMin,vMax);
// 		}
	}

	void MeshComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		IRenderDevice* pRenderDevice = ma::GetRenderDevice();
		if (pRenderDevice == NULL)
			return;

		sl.BeginSection(pszLable);

		sl.Serialize(m_strMeshPath);
		sl.Serialize(m_strMaterialPath);

		if ( sl.IsReading() )
		{
			Load(m_strMeshPath.c_str(),m_strMaterialPath.c_str());
		}

		sl.EndSection();
	}

}

