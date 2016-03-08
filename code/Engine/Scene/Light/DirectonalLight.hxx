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
		m_eLightType = LIGHT_DIRECTIONAL;
		m_bShadowEnable = false;
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

// 		m_ShadowSamplesNumer = MAX_SHADOW_SAMPLES_NUM;
// 		memset(m_irreg_kernel,0,sizeof(m_irreg_kernel));
// 		memset(m_viewPosVecLS,0,sizeof(m_viewPosVecLS));

		m_fShadowFarDist = 0;

		m_eCaterType = CasterCull_No;
        m_eShadowBleurLevel = BLUR_NO;
	}

	DirectonalLight::~DirectonalLight()
	{
		PoissonDiskGen::Realse();
	}

	void DirectonalLight::Init()
	{
		if ( GetDeviceCapabilities()->GetD24S8Supported() )
		{
			GetRenderSystem()->AddShaderGlobaMacro("USING_HW_PCF","1");
		}
		else if ( GetDeviceCapabilities()->GetFloatTexturesSupported() )
		{
			GetRenderSystem()->AddShaderGlobaMacro("USING_FLOATTEXTURE","1");
		}		

		SetMaxSplitCount(m_nMaxSplitCount);
	}

	void DirectonalLight::SetShadowEnabled(bool b)
	{
		if (b == m_bShadowEnable)
			return;

		m_bShadowEnable = b;

		if (m_bShadowEnable)
		{
			GetRenderSystem()->AddShaderGlobaMacro("USING_SHADOW", "1");
			GetRenderSystem()->AddShaderGlobaMacro("USING_TERRAIN_LIGHTMAP", "0");
			
			Init();	
		}
		else
		{
			GetRenderSystem()->AddShaderGlobaMacro("USING_SHADOW", "0");
			GetRenderSystem()->AddShaderGlobaMacro("USING_TERRAIN_LIGHTMAP", "1");

			Clear(NULL);
		}
	}

	void ProjectScreenToWorldExpansionBasis(const Matrix4& mShadowTexGen, Camera& cam, float fViewWidth, float fViewHeight, 
		Vector4& vWBasisX, Vector4& vWBasisY, Vector4& vWBasisZ, Vector4& vBasisMagnitudes, Vector4& vCamPos)
	{

		const Matrix4& camMatrix = cam.GetMatrixWS();

		// projection ratio
		float fProjectionRatio = fViewWidth / fViewHeight ;

		//all values are in camera space
		float fFar = cam.GetFarClip();
		float fNear	= cam.GetNearClip();
		float fWorldHeightDiv2 = fNear * Math::Tan( cam.GetFov() * 0.5f );
		float fWorldWidthDiv2 = fWorldHeightDiv2 * fProjectionRatio; 
		float k = fFar / fNear;

		Vector3 vZ = -camMatrix.GetColumn(2).normalisedCopy() * fNear * k; // size of vZ is the distance from camera pos to near plane
		Vector3 vX = camMatrix.GetColumn(0).normalisedCopy() * fWorldWidthDiv2 * k;
		Vector3 vY = camMatrix.GetColumn(1).normalisedCopy() * fWorldHeightDiv2 * k;

		vZ = vZ - vX;
		vX *= (2.0f / fViewWidth);   // 变换到 -1 ~ 1

		vZ = vZ + vY;
		vY *= -(2.0f / fViewHeight); // 变换到 -1 ~ 1

		// Transform basis to any local space ( shadow space here )
		vWBasisX = mShadowTexGen * Vector4(vX, 0.0f);
		vWBasisY = mShadowTexGen * Vector4(vY, 0.0f);
		vWBasisZ = mShadowTexGen * Vector4(vZ, 0.0f);
		vCamPos =  mShadowTexGen * Vector4(cam.GetPosWS(), 1.0f);

		// 		vBasisMagnitudes.x = vWBasisX.length();
		// 		vBasisMagnitudes.y = vWBasisY.length();
		// 		vBasisMagnitudes.z = vWBasisZ.length();
		// 		vBasisMagnitudes.w = 1.0f;
		// 
		// 		vWBasisX /= vBasisMagnitudes.x;
		// 		vWBasisY /= vBasisMagnitudes.y;
		// 		vWBasisZ /= vBasisMagnitudes.z;
	}

	void DirectonalLight::Update()
	{
		m_pSceneNode->GetScene()->AddRenderLight(this);
	}

	void DirectonalLight::Update(Camera* pCamera)
	{
		if (!m_bShadowEnable)
			return;

		UpdateViewMinMaxZ(pCamera);

		Scene* pScene = pCamera->GetScene();

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



	void DirectonalLight::RenderShadowMap(Camera* pCamera)
	{
		if (!m_bShadowEnable)
			return;	

		Update(pCamera);
		
		for (int i = 0; i < m_nCurSplitCount; ++i)
		{
			m_SpitFrustum[i].Render(pCamera);
		}
		
// 		if (m_nMaxSplitCount > 1)
// 		{
// 			ASSERT(pCamera->GetDeferredShadowEnabled());
// 		}
	}

	void DirectonalLight::Clear(Camera* pCamera)
	{
		for (int i = 0; i < m_nCurSplitCount; ++i)
		{
			m_SpitFrustum[i].Clear(pCamera);
		}
	}

	void DirectonalLight::SetMaxSplitCount(int nMaxSplitCount)
	{
		assert(nMaxSplitCount > 0 && nMaxSplitCount <= MAX_FRUSTUM_SPLIT_NUM);
		if (nMaxSplitCount <= 0 || nMaxSplitCount > MAX_FRUSTUM_SPLIT_NUM)
			return;
		
		m_nMaxSplitCount = nMaxSplitCount;

		for (int i = 0; i < nMaxSplitCount; ++i)
		{
			m_SpitFrustum[i].CreateShadowMap(m_nShadowMapSize);
			m_SpitFrustum[i].m_pParent = this;
		}
	}

	void DirectonalLight::UpdateViewMinMaxZ(Camera* pCamera)
	{
		Scene* pScene = pCamera->GetScene();
		uint32 nNode = pScene->GetVisibleNodeNum();
		if (nNode <= 0)
			return;

		// find the nearest and farthest points of given
		// scene objects in camera's view space
		float fMaxZ = 0;
		float fMinZ = FLT_MAX;

		// for each object
		for(uint32 i = 0; i < nNode; i++)
		{
			RenderComponent* pObject = pScene->GetVisibleNodeByIndex(i);

			AABB aabb = pObject->GetAABBWS();
			aabb.transform(pCamera->GetMatView());

			float aabbMinZ = -aabb.getMaximum().z;
			float aabbMaxZ = -aabb.getMinimum().z;

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

		for (int i = 0; i < m_nMaxSplitCount; ++i)
		{
			m_SpitFrustum[i].CreateShadowMap(m_nShadowMapSize);
			m_SpitFrustum[i].m_pParent = this;
		}
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


//	void RenderShadowCSM::SetShadowSamplesNum(int nNum)
//	{
// 		ASSERT(nNum <= MAX_SHADOW_SAMPLES_NUM);
// 		if (nNum > MAX_SHADOW_SAMPLES_NUM)
// 			return;
// 
// 		m_ShadowSamplesNumer = nNum;
// 
// 		GetRenderSystem()->AddShaderGlobaMacro("SHADOW_SAMPLES_NUM", StringConverter::toString(m_ShadowSamplesNumer).c_str());
// 
// 		PoissonDiskGen::SetKernelSize(m_ShadowSamplesNumer);
// 
// 		for (int i=0, nIdx = 0; i < m_ShadowSamplesNumer; i += 2, nIdx++)
// 		{
// 			Vector2 vSample = PoissonDiskGen::GetSample(i);
// 			m_irreg_kernel[nIdx].x = vSample.x;
// 			m_irreg_kernel[nIdx].y = vSample.y;
// 			vSample = PoissonDiskGen::GetSample(i + 1);
// 			m_irreg_kernel[nIdx].z = vSample.x;
// 			m_irreg_kernel[nIdx].w = vSample.y;
// 		}
//	}


	RefPtr<DirectonalLight> CreateDirectonalLight()
	{
		return new DirectonalLight();
	}
}