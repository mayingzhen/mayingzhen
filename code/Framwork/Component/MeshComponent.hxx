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

	void MeshComponent::Update()
	{
		if (m_pRendMesh == NULL || m_pGameObject == NULL)
			return;	

		m_pRendMesh->SetWorldMatrix( m_pGameObject->GetSceneNode()->GetWorldMatrix() );	

		for (UINT i = 0; i < m_pRendMesh->GetRenderableNumber(); ++i)
		{
			GetRenderSystem()->AddRenderable(m_pRendMesh->GetRenderableByIndex(i));
		}
	}

	void MeshComponent::Load(const char* pszMeshPath,const char* pszTexPath)
	{
		IRenderDevice* pRenderDevice = ma::GetRenderDevice();
		if (pRenderDevice == NULL)
			return;

		m_strMeshPath = pszMeshPath;
		m_strMaterialPath = pszTexPath;

		m_pRendMesh = GetRenderSystem()->CreatRenderMesh(pszMeshPath,pszTexPath);
	}	

	AABB MeshComponent::GetBoundingAABB()
	{
		if (m_pRendMesh)
		{
			return m_pRendMesh->GetBoundingAABB();
		}
		else
		{
			ASSERT(false);
			return AABB();
		}
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

