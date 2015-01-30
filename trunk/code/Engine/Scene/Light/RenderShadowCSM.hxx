#include "RenderShadowCSM.h"
#include "../Camera.h"
#include "../../Material/Texture.h"
#include "PoissonDiskGen.h"


namespace ma
{
	static const float DEFAULT_CONSTANTBIAS = 0.0001f;
	static const float DEFAULT_SLOPESCALEDBIAS = 0.5f;


	RenderShadowCSM::RenderShadowCSM()
	{
		m_bEnable = false;
		m_nCurSplitCount = 0;
		m_nMaxSplitCount = 1;
		m_nShadowMapSize = 512;
		m_SplitPosParam = Vector4(3000.0f,60.0f,120.0f,270.0f);
		m_curSplitPos[0] = m_SplitPosParam;
		m_curSplitPos[1] = m_SplitPosParam;

		m_fShadowFadeStart = 0.8f;
		m_vShadowDepthFade[0] = Vector4::ZERO;
		m_vShadowDepthFade[1] = Vector4::ZERO;
		
		m_fConstantBias = DEFAULT_CONSTANTBIAS;
		m_fSlopeScaledBias = DEFAULT_SLOPESCALEDBIAS;

		m_ShadowSamplesNumer = MAX_SHADOW_SAMPLES_NUM;
		memset(m_irreg_kernel,0,sizeof(m_irreg_kernel));
		memset(m_viewPosVecLS,0,sizeof(m_viewPosVecLS));

		m_fShadowFarDist = 0;

		m_eCaterType = CasterCull_No;
        m_eShadowBleurLevel = BLUR_NO;
	}

	RenderShadowCSM::~RenderShadowCSM()
	{
		CPoissonDiskGen::Realse();
	}

	void RenderShadowCSM::Init()
	{
		if ( GetDeviceCapabilities()->GetDepthTextureSupported() )
		{
			GetRenderSystem()->AddMacro("USING_HW_PCF","1");
		}
		else if ( GetDeviceCapabilities()->GetFloatTexturesSupported() )
		{
			GetRenderSystem()->AddMacro("USING_32F","1");
		}		

		SetMaxSplitCount(m_nMaxSplitCount);
	}

	void RenderShadowCSM::SetEnabled(bool b)
	{
		if (b == m_bEnable)
			return;

		m_bEnable = b;

		if (m_bEnable)
		{
			GetRenderSystem()->AddMacro("USING_SHADOW", "1");
			GetRenderSystem()->AddMacro("USING_TERRAIN_LIGHTMAP", "0");
			
			Init();	
		}
		else
		{
			GetRenderSystem()->AddMacro("USING_SHADOW", "0");
			GetRenderSystem()->AddMacro("USING_TERRAIN_LIGHTMAP", "1");

			Clear(NULL);
		}
	}

	void RenderShadowCSM::Update(Camera* pCamera)
	{
		if (!m_bEnable)
			return;

		Scene* pScene = pCamera->GetSceneNode()->GetScene();

		m_nCurSplitCount = 0;
		m_curSplitPos[GetRenderSystem()->CurThreadFill()] = Vector4::ZERO;
			
		float fNearSplit = pScene->GetViewMinZ();
		float fFarSplit;

		while (m_nCurSplitCount < m_nMaxSplitCount)
		{
			// If split is completely beyond camera far clip, we are done
			if (fNearSplit > pScene->GetViewMaxZ())
				break;

			fFarSplit = min(pScene->GetViewMaxZ(), m_SplitPosParam[m_nCurSplitCount]);
			if (fFarSplit <= fNearSplit)
				break;

			// Setup the shadow camera for the split
			m_curSplitPos[GetRenderSystem()->CurThreadFill()][m_nCurSplitCount] = fFarSplit;
 
			m_SpitFrustum[m_nCurSplitCount].Update(pCamera,fNearSplit,fFarSplit);

			m_matShadow[GetRenderSystem()->CurThreadFill()][m_nCurSplitCount] =  m_SpitFrustum[m_nCurSplitCount].GetTexAdjustMatrix() * m_SpitFrustum[m_nCurSplitCount].GetShadowMatrix();

			if (m_eShadowBleurLevel == SHADOW_JITTERIN)
			{
				Vector3 vViewPosLS = m_SpitFrustum[m_nCurSplitCount].GetLightViewMatrix() * pCamera->GetSceneNode()->GetPos();
				Vector3 vViewVectLS = m_SpitFrustum[m_nCurSplitCount].GetLightViewMatrix() * pCamera->GetSceneNode()->GetForward();
				m_viewPosVecLS[GetRenderSystem()->CurThreadFill()][m_nCurSplitCount] = Vector4(vViewPosLS.x,vViewPosLS.y,vViewVectLS.x,vViewVectLS.y);
			}

			fNearSplit = fFarSplit;
			++m_nCurSplitCount;
		}

		// Upadte shadowDetphFade
		float shadowRange = fFarSplit;
		float fadeStart = m_fShadowFadeStart * shadowRange;
		float fadeEnd = shadowRange;
		float fadeRange = fadeEnd - fadeStart;

		m_vShadowDepthFade[GetRenderSystem()->CurThreadFill()] = Vector4(0,0,fadeStart,1.0f / fadeRange);
	}



	void RenderShadowCSM::Render(Camera* pCamera)
	{
		if (!m_bEnable)
			return;		

		Rectangle rPreViewport = GetRenderSystem()->SetViewPort(m_viewport);
		RefPtr<Texture> pPreRenderTarget = GetRenderSystem()->SetRenderTarget(m_pShdowDepth,0);
		RefPtr<Texture> pPreDepthStencil = GetRenderSystem()->SetDepthStencil(m_pDepthStencil);
		
		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.f, 0);

		if ( GetDeviceCapabilities()->GetDepthTextureSupported() )
		{
			GetRenderSystem()->SetColorWrite(false);	
		}

		for (int i = 0; i < m_nCurSplitCount; ++i)
		{
			m_SpitFrustum[i].Render(pCamera);
		}
		
		GetRenderSystem()->SetViewPort(rPreViewport);
		GetRenderSystem()->SetRenderTarget(pPreRenderTarget.get(),0);
		GetRenderSystem()->SetDepthStencil(pPreDepthStencil.get());
		
		if ( GetDeviceCapabilities()->GetDepthTextureSupported() )
		{
			GetRenderSystem()->SetDepthBias(0);
			GetRenderSystem()->SetColorWrite(true);
		}
	}

	void RenderShadowCSM::Clear(Camera* pCamera)
	{
		for (int i = 0; i < m_nCurSplitCount; ++i)
		{
			m_SpitFrustum[i].Clear(pCamera);
		}
	}

	void RenderShadowCSM::SetMaxSplitCount(int nMaxSplitCount)
	{
		assert(nMaxSplitCount > 0 && nMaxSplitCount <= MAX_FRUSTUM_SPLIT_NUM);
		if (nMaxSplitCount <= 0 || nMaxSplitCount > MAX_FRUSTUM_SPLIT_NUM)
			return;
		
		m_nMaxSplitCount = nMaxSplitCount;

		CreateShadowMap(m_nShadowMapSize);

		GetRenderSystem()->AddMacro("g_iNumSplits", StringConverter::toString(m_nMaxSplitCount).c_str());
	}

	void RenderShadowCSM::CreateShadowMap(int nSize)
	{
// 		m_pShadowMap = new SamplerState();
// 		m_pShadowMap->SetFilterMode(TFO_POINT);
// 		m_pShadowMap->SetWrapMode(CLAMP);

		if (GetDeviceCapabilities()->GetDepthTextureSupported())
		{
			m_pDepthStencil = GetRenderSystem()->CreateRenderTexture(nSize, nSize, PF_D24S8, USAGE_DEPTHSTENCIL);
			m_pShdowDepth = GetRenderSystem()->CreateRenderTexture(nSize, nSize, PF_A8R8G8B8, USAGE_RENDERTARGET);
		
			//m_pShadowMap->SetTexture(m_pDepthStencil.get());
		}
		else
		{
			PixelFormat format = PF_A8R8G8B8;
			if (GetDeviceCapabilities()->GetFloatTexturesSupported())
			{
				format = PF_FLOAT32_R;
			}
			
			m_pDepthStencil = GetRenderSystem()->CreateDepthStencil(nSize, nSize, PF_D24S8);
			m_pShdowDepth = GetRenderSystem()->CreateRenderTexture(nSize, nSize, format, USAGE_RENDERTARGET);
		
			//m_pShadowMap->SetTexture(m_pShdowDepth.get());
		}

		m_viewport = Rectangle(0, 0, (float)nSize, (float)nSize);

		if (m_nMaxSplitCount == 1)
		{
			m_SpitFrustum[0].CreateViewPort(m_pShdowDepth.get(),m_viewport);
		}
		else if (m_nMaxSplitCount == 2)
		{
			m_SpitFrustum[0].CreateViewPort(m_pShdowDepth.get(),Rectangle(0,0,0.5f * nSize,1));
			m_SpitFrustum[1].CreateViewPort(m_pShdowDepth.get(),Rectangle(0.5f * nSize,0,0.5f * nSize,1));
		}
		else 
		{
			m_SpitFrustum[0].CreateViewPort(m_pShdowDepth.get(),Rectangle(0,0,0.5f * nSize,0.5f * nSize));
			m_SpitFrustum[1].CreateViewPort(m_pShdowDepth.get(),Rectangle(0.5f * nSize,0,0.5f * nSize,0.5f * nSize));
			m_SpitFrustum[2].CreateViewPort(m_pShdowDepth.get(),Rectangle(0,0.5f * nSize,0.5f * nSize,0.5f * nSize));
			m_SpitFrustum[3].CreateViewPort(m_pShdowDepth.get(),Rectangle(0.5f * nSize,0.5f * nSize,0.5f * nSize,0.5f * nSize));
		}

	}

	Texture* RenderShadowCSM::GetShadowMap() const 
	{
		if (GetDeviceCapabilities()->GetDepthTextureSupported())
			return m_pDepthStencil.get();
		else
			return m_pShdowDepth.get();
	} 

	void RenderShadowCSM::SetShadowMapSize(int nSize)
	{
		if (nSize == m_nShadowMapSize)
			return;

		m_nShadowMapSize = nSize;

		CreateShadowMap(m_nShadowMapSize);
	}

	void RenderShadowCSM::GetDepthBiasParams(float& fConstantBias, float& fSlopeScaleBias) const
	{
		fConstantBias = m_fConstantBias;
		fSlopeScaleBias = m_fSlopeScaledBias;
	}

	void RenderShadowCSM::SetDepthBiasParams(float fConstantBias, float fSlopeScaleBias)
	{
		m_fConstantBias = Math::Clamp<float>(fConstantBias,-1.0f,1.0f);
		m_fSlopeScaledBias = Math::Clamp<float>(fSlopeScaleBias,-16.0f,16.0f);
	}

	void RenderShadowCSM::SetShadowBlurLevel(Shadow_Blur eBlur)
	{
		if (m_eShadowBleurLevel == eBlur)
			return;
		
		m_eShadowBleurLevel = eBlur;

		if (BLUR_NO == eBlur)
		{
			GetRenderSystem()->AddMacro("SHADOW_BLUR", "0");
		}
		else if (SHADOW_PCF2x2 == eBlur)
		{
			GetRenderSystem()->AddMacro("SHADOW_BLUR", "1");
		}
		else if (SHADOW_JITTERIN == eBlur)
		{
			GetRenderSystem()->AddMacro("SHADOW_BLUR", "2");
		}

		if (SHADOW_JITTERIN == eBlur)
		{
			m_pRotSampler = CreateSamplerState("_common/rotrandom.dds");
			
			SetShadowSamplesNum(m_ShadowSamplesNumer);
		}
	}


	void RenderShadowCSM::SetShadowSamplesNum(int nNum)
	{
		ASSERT(nNum <= MAX_SHADOW_SAMPLES_NUM);
		if (nNum > MAX_SHADOW_SAMPLES_NUM)
			return;

		m_ShadowSamplesNumer = nNum;

		GetRenderSystem()->AddMacro("SHADOW_SAMPLES_NUM", StringConverter::toString(m_ShadowSamplesNumer).c_str());

		CPoissonDiskGen::SetKernelSize(m_ShadowSamplesNumer);

		for (int i=0, nIdx = 0; i < m_ShadowSamplesNumer; i += 2, nIdx++)
		{
			Vector2 vSample = CPoissonDiskGen::GetSample(i);
			m_irreg_kernel[nIdx].x = vSample.x;
			m_irreg_kernel[nIdx].y = vSample.y;
			vSample = CPoissonDiskGen::GetSample(i + 1);
			m_irreg_kernel[nIdx].z = vSample.x;
			m_irreg_kernel[nIdx].w = vSample.y;
		}
	}
	
	RenderShadowCSM* g_pRenderShadowCSM = NULL;
	RenderShadowCSM* GetRenderShadowCSM()
	{
		return g_pRenderShadowCSM;
	}
}