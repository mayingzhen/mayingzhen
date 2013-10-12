#include "MeshComponent.h"

namespace ma
{
	IMPL_OBJECT(MeshComponent,Component)

	MeshComponent::MeshComponent(GameObject* pGameObj)
	:Component(pGameObj)
	{
		m_pRendMesh = NULL;
	}

	MeshComponent::~MeshComponent()
	{
		SAFE_DELETE(m_pRendMesh);
	}

	void MeshComponent::BuildRenderItem()
	{
		if (m_pRendMesh == NULL || m_pGameObject == NULL)
			return;	

		m_pRendMesh->SetWorldMatrix( m_pGameObject->GetSceneNode()->GetWorldMatrix() );	

		GetRenderQueue()->AddRenderObj(RL_Solid,m_pRendMesh);
	}

	void MeshComponent::Load(const char* pszSknPath,const char* pszMatPath)
	{
		IRenderDevice* pRenderDevice = ma::GetRenderDevice();
		if (pRenderDevice == NULL)
			return;

		SAFE_DELETE(m_pRendMesh);
		m_pRendMesh = new RenderMesh();
		m_pRendMesh->Load(pszSknPath,pszMatPath);
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
		sl.BeginSection(pszLable);

		if ( sl.IsReading() )
		{
			std::string strSknPath;
			std::string strMatPath;

			sl.Serialize(strSknPath);
			sl.Serialize(strMatPath);

			Load(strSknPath.c_str(),strMatPath.c_str());
		}
		else
		{
			ASSERT(m_pRendMesh && m_pRendMesh->GetMeshData() && m_pRendMesh->GetMaterial());
	
			std::string strSknPath = m_pRendMesh->GetMeshData()->GetResPath();
			std::string strMatPath = m_pRendMesh->GetMaterial()->GetResPath();

			sl.Serialize(strSknPath);
			sl.Serialize(strMatPath);
		}

		sl.EndSection();
	}

}

