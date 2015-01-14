#include "LightSystem.h"

namespace ma
{
	LightSystem* gpLightSystem = NULL;

	LightSystem* GetLightSystem() 
	{
		return gpLightSystem;
	}

	void SetLightSystem(LightSystem* pLightSystem)
	{
		gpLightSystem = pLightSystem;
	}

	LightSystem::LightSystem()
	{
		m_cAmbientColor = ColourValue::White;
	}

	void LightSystem::AddLight(Light* pLight)
	{
		m_arrLight.push_back(pLight);
	}

	void LightSystem::RemoveLight(Light* pLight)
	{
		std::vector<Light*>::iterator it = std::find(m_arrLight.begin(),m_arrLight.end(),pLight);
		if ( it != m_arrLight.end() )
		{
			m_arrLight.erase(it);
		}
	}

// 	void LightSystem::Update()
// 	{
// 		if (!GetRenderSetting()->m_bShadow)
// 			return;
// 
// 		UINT nLight = m_arrLight.size();
// 		for (UINT i = 0; i < nLight; ++i)
// 		{
// 			Light* pLight = m_arrLight[i];
// 			if ( !pLight->IsCreateShadow() )
// 				continue;
// 
// 			//pLight->UpdateShadowFrustum( GetRenderSystem()->GetMainCamera().get() );
// 		}
// 	}
}