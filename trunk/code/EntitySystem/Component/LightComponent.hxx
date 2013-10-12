#include "LightComponent.h"

namespace ma
{
// 	IMPL_OBJECT(LightComponent,Component)
// 
// 	LightComponent::LightComponent(GameObject* pGameObj)
// 	:Component(pGameObj)
// 	{
// 		m_pRendMesh = NULL;
// 	}
// 
// 	LightComponent::~LightComponent()
// 	{
// 		SAFE_DELETE(m_pRendMesh);
// 	}
// 
// 
// 	void LightComponent::Serialize(Serializer& sl, const char* pszLable)
// 	{
// 		sl.BeginSection(pszLable);
// 
// 
// 
// 		sl.EndSection();
// 	}


	IMPL_OBJECT(PointLightComponent,Component)

	PointLightComponent::PointLightComponent(GameObject* pGameObj)
		:Component(pGameObj)
	{
		m_pLight = new PointLight();
		GetRenderSystem()->AddLight(m_pLight);
	}

	PointLightComponent::~PointLightComponent()
	{
		SAFE_DELETE(m_pLight);
	}

	void PointLightComponent::BuildRenderItem()
	{
		m_pLight->SetPos(m_pGameObject->GetSceneNode()->GetTransform().m_vPos);
	}

	void PointLightComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);



		sl.EndSection();
	}

}

