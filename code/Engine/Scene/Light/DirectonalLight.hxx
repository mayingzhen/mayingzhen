#include "DirectonalLight.h"
#include "../Camera.h"
#include "../../Material/Texture.h"
#include "PoissonDiskGen.h"


namespace ma
{
	static const float DEFAULT_CONSTANTBIAS = 0.0001f;
	static const float DEFAULT_SLOPESCALEDBIAS = 0.5f;


	DirectonalLight::DirectonalLight()
	{
		if (GetRenderSystem()->GetDefferedLightRenderPass())
		{
			m_pTech = CreateTechnique("shader/dirlight.tech", "", GetRenderSystem()->GetDefferedLightRenderPass());
		}

		m_eLightType = LIGHT_DIRECTIONAL;
		m_bShadowEnable = false;
		m_nMaxSplitCount = 4;
		m_nShadowMapSize = 512;
		m_SplitPosParam = Vector4(30.0f,60.0f,120.0f,270.0f);
		m_curSplitPos = m_SplitPosParam;

		m_fShadowFadeStart = 0.8f;
		m_vShadowDepthFade = Vector4::ZERO;
		
		m_fConstantBias = DEFAULT_CONSTANTBIAS;
		m_fSlopeScaledBias = DEFAULT_SLOPESCALEDBIAS;

		m_fShadowFarDist = 0;

        m_eShadowBleurLevel = BLUR_NO;

		AABB aabb;
		aabb.setInfinite();
		SetAABB(aabb);

		m_shadowMapRender = new ShadowMapRenderStep();
	}

	DirectonalLight::~DirectonalLight()
	{
		PoissonDiskGen::Realse();
	}

	void DirectonalLight::Init()
	{
		SetMaxSplitCount(m_nMaxSplitCount);
	}

	void DirectonalLight::Update()
	{
		Light::Update();
	}

	void DirectonalLight::Render(RenderQueue* pRenderQueue)
	{
		Light::Render(pRenderQueue);
	}

	void DirectonalLight::SetShadowEnabled(bool b)
	{
		if (b == m_bShadowEnable)
			return;

		m_bShadowEnable = b;

		if (m_bShadowEnable)
		{
			Init();	
		}
		else
		{
			Clear(NULL);
		}
	}

	void DirectonalLight::UpdateShadowMap(Camera* pCamera)
	{
		if (!m_bShadowEnable)
			return;

		MICROPROFILE_SCOPEI("", "UpdateShadowMap", 0);

	
		m_curSplitPos = m_SplitPosParam;

		//Simple Shadow
		if (m_nMaxSplitCount == 1)
		{
 			m_curSplitPos.x = pCamera->GetFarClip(); // For Depth bias
 			m_SpitFrustum[0].Update(pCamera,pCamera->GetNearClip(),pCamera->GetFarClip());
 			return;
 		}

		UpdateViewMinMaxZ(pCamera);

		//float fViewMinZ = pCamera->GetScene()->GetViewMinZ();
		float fViewMaxZ = pCamera->GetScene()->GetViewMaxZ();

		float fNearSplit = pCamera->GetNearClip();
		float fFarSplit = m_SplitPosParam[0];

		int nCurSplitCount = 0;
		while (nCurSplitCount < m_nMaxSplitCount)
		{
			if (fNearSplit > fViewMaxZ)
				break;

			fFarSplit = min(fViewMaxZ, m_SplitPosParam[nCurSplitCount]);
			if (fFarSplit <= fNearSplit)
				break;

			// Setup the shadow camera for the split
			m_curSplitPos[nCurSplitCount] = fFarSplit;

			m_SpitFrustum[nCurSplitCount].Update(pCamera, fNearSplit, fFarSplit);

			fNearSplit = fFarSplit;
			++nCurSplitCount;
		}

		// Upadte shadowDetphFade
		float shadowRange = fFarSplit;
		float fadeStart = m_fShadowFadeStart * shadowRange;
		float fadeEnd = shadowRange;
		float fadeRange = fadeEnd - fadeStart;

		m_vShadowDepthFade = Vector4(0, 0, fadeStart, 1.0f / fadeRange);
	}

	void DirectonalLight::RenderShadowMap(Camera* pCamera)
	{
		if (!m_bShadowEnable)
			return;	

		MICROPROFILE_SCOPEI("", "UpdateShadowMap", 0);

		GetRenderSystem()->AddRenderStep(m_shadowMapRender);

		for (int i = 0; i < m_nMaxSplitCount; ++i)
		{
			m_SpitFrustum[i].Render(pCamera, m_shadowMapRender.get());
		}
 	}

	void DirectonalLight::Clear(Camera* pCamera)
	{
		for (int i = 0; i < m_nMaxSplitCount; ++i)
		{
			m_SpitFrustum[i].Clear(pCamera);
		}
	}

	void DirectonalLight::CreateShadowMap()
	{
		uint32_t width = m_nMaxSplitCount * m_nShadowMapSize;
		uint32_t height = m_nShadowMapSize;
		PixelFormat shadowMapDepthFormat = GetDeviceCapabilities()->GetShadowMapDepthFormat();

		m_pShdowMapDepth = GetRenderSystem()->CreateDepthStencil(width, height, PF_D24S8/*shadowMapDepthFormat*/);

		m_pShadowMapPass = GetRenderDevice()->CreateRenderPass();
		m_pShadowMapPass->AttachDepthStencil( RenderSurface(m_pShdowMapDepth) );
		GetRenderSystem()->RenderPassStreamComplete(m_pShadowMapPass.get());

		m_pShadowMapSampler = CreateSamplerState(m_pShdowMapDepth.get(), CLAMP, TFO_SHADOWCOMPARE, false);

		m_shadowMapRender->m_pRenderPass = m_pShadowMapPass.get();

		for (int i = 0; i < m_nMaxSplitCount; ++i)
		{
			Rectangle viewPort = Rectangle(1.0f + i * m_nShadowMapSize, 1.0f,
				(i + 1) * m_nShadowMapSize - 2.0f, m_nShadowMapSize - 2.0f);
			m_SpitFrustum[i].InitShadowMap(this, viewPort, m_pShadowMapPass.get());
		}
	}

	void DirectonalLight::SetMaxSplitCount(int nMaxSplitCount)
	{
		assert(nMaxSplitCount > 0 && nMaxSplitCount <= MAX_FRUSTUM_SPLIT_NUM);
		if (nMaxSplitCount <= 0 || nMaxSplitCount > MAX_FRUSTUM_SPLIT_NUM)
			return;
		
		m_nMaxSplitCount = nMaxSplitCount;

		CreateShadowMap();
	}

	void DirectonalLight::UpdateViewMinMaxZ(Camera* pCamera)
	{
		Scene* pScene = pCamera->GetScene();
		uint32_t nNode = pScene->GetVisibleNodeNum();
		if (nNode <= 0)
			return;

		// find the nearest and farthest points of given
		// scene objects in camera's view space
		float fMaxZ = 0;
		float fMinZ = (std::numeric_limits<float>::max)();

		// for each object
		for(uint32_t i = 0; i < nNode; i++)
		{
			RenderComponent* pObject = pScene->GetVisibleNodeByIndex(i);

			AABB aabb = pObject->GetAABBWS();
			aabb.transform(pCamera->GetMatView());

			float aabbMinZ = aabb.getMinimum().z;
			float aabbMaxZ = aabb.getMaximum().z;

			pObject->SetViewMinMaxZ(aabbMinZ,aabbMaxZ);

			fMaxZ = max(aabbMaxZ,fMaxZ);
			fMinZ = min(aabbMinZ,fMinZ);
		}

		// use smallest distance as new near plane
		// and make sure it is not too small
		float fSceneViwMinZ = max(fMinZ, pCamera->GetNearClip());

		// use largest distance as new far plane
		// and make sure it is larger than nearPlane
		float fSceneViwMaxZ = max(fMaxZ, fSceneViwMinZ + 1.0f);

		pScene->SetViewMinMaxZ(fSceneViwMinZ,fSceneViwMaxZ);
	}


	void DirectonalLight::SetShadowMapSize(int nSize)
	{
		ASSERT(Bitwise::isPO2(nSize));

		if (nSize == m_nShadowMapSize)
			return;

		m_nShadowMapSize = nSize;

		CreateShadowMap();
	}

	void DirectonalLight::GetDepthBiasParams(float& fConstantBias, float& fSlopeScaleBias) const
	{
		fConstantBias = m_fConstantBias;
		fSlopeScaleBias = m_fSlopeScaledBias;
	}

	void DirectonalLight::SetDepthBiasParams(float fConstantBias, float fSlopeScaleBias)
	{
		m_fConstantBias = Math::Clamp<float>(fConstantBias,-1.0f,1.0f);
		m_fSlopeScaledBias = Math::Clamp<float>(fSlopeScaleBias,-16.0f,16.0f);
	}

	void DirectonalLight::SetShadowBlurLevel(Shadow_Blur eBlur)
	{
		if (m_eShadowBleurLevel == eBlur)
			return;
		
		m_eShadowBleurLevel = eBlur;

		if (BLUR_NO == eBlur)
		{
			GetRenderSystem()->AddShaderGlobaMacro("SHADOW_BLUR", "0");
		}
		else if (SHADOW_PCF2x2 == eBlur)
		{
			GetRenderSystem()->AddShaderGlobaMacro("SHADOW_BLUR", "1");
		}
		else if (SHADOW_JITTERIN == eBlur)
		{
			GetRenderSystem()->AddShaderGlobaMacro("SHADOW_BLUR", "2");
		}

		if (SHADOW_JITTERIN == eBlur)
		{
			//m_pRotSampler = CreateTexture("_common/rotrandom.dds");
			
			//SetShadowSamplesNum(m_ShadowSamplesNumer);
		}
	}

	RefPtr<DirectonalLight> CreateDirectonalLight()
	{
		return new DirectonalLight();
	}
}
