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
		m_cAmbientColor = Vector4(1,1,1,1);
	}

	void LightSystem::Update()
	{
		if (!GetRenderSetting()->m_bShadow)
			return;

		UINT nLight = m_arrLight.size();
		for (UINT i = 0; i < nLight; ++i)
		{
			Light* pLight = m_arrLight[i];
			if ( !pLight->IsCreateShadow() )
				continue;

			pLight->UpdateShadowFrustum( GetRenderSystem()->GetMainCamera() );
		}
	}
}