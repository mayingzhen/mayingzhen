#include "ParameterManager.h"
#include "../RenderScheme/DeferredShadingPass.h"
#include "../RenderScheme/DeferredShadowPass.h"

namespace ma
{
	ParameterManager* g_pParameterManager = NULL;

	ParameterManager* GetParameterManager()
	{
		return g_pParameterManager;
	}

	ParameterManager::ParameterManager() 
	{
	}

	ParameterManager::~ParameterManager()
	{
	
	}

	void ParameterManager::UseDefaultBing(Uniform* pParam)
	{
		ASSERT(pParam);
		if (pParam == NULL)
			return;

		const char* pszName = pParam->GetName();
		if ( strcmp(pszName, "g_matWorld") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetWorldMatrix);
		}
		else if ( strcmp(pszName,"g_matView") == 0 )
		{
			pParam->BindMethod(GetRenderContext(),&RenderContext::GetViewMatrix);
		}
		else if ( strcmp(pszName,"g_matViewInv") == 0 )
		{
			pParam->BindMethod(GetRenderContext(),&RenderContext::GetViewMatrixInv);
		}
		else if ( strcmp(pszName,"g_matProj") == 0 )
		{
			pParam->BindMethod(GetRenderContext(),&RenderContext::GetViewMatrix);
		}
		else if ( strcmp(pszName,"g_matProjInv") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetInverseProjectionMatrix);
		}
		else if ( strcmp(pszName,"g_matWorldView") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetWorldViewMatrix);
		}
		else if ( strcmp(pszName,"g_matViewProj") == 0 )
		{
			pParam->BindMethod(GetRenderContext(), &RenderContext::GetViewProjMatrix);
		}
		else if ( strcmp(pszName,"g_matWorldViewProj") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetWorldViewProjectionMatrix);
		}
		else if (  strcmp(pszName,"g_vCameraNearFar") == 0 )
		{
			pParam->BindMethod(this,&ParameterManager::autoBindingGetCameraNearFar);
		}
		else if ( strcmp(pszName,"g_vEyeWorldPos") == 0 )
		{
			pParam->BindMethod(GetRenderContext(), &RenderContext::GetEyeWorldPos);
		}
		else if ( strcmp(pszName,"g_cSkyLight") == 0 )
		{
			pParam->BindMethod(GetRenderContext(), &RenderContext::GetAmbientColor);
		}
		else if ( strcmp(pszName,"g_cDirLight") == 0 )
		{
			pParam->BindMethod(GetRenderContext(), &RenderContext::GetDirLightColor);
		}
		else if ( strcmp(pszName,"g_vDirLight") == 0 )
		{
			pParam->BindMethod(GetRenderContext(), &RenderContext::GetDirLightDir);
		}
		else if ( strcmp(pszName,"tDeviceDepthMapSampler") == 0 )
		{
			pParam->BindMethod(this,&ParameterManager::autoBindingDeviceDepthMap);
		}
		else if ( strcmp(pszName,"g_tShadowMap") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingShadowMap);
		}
		else if ( strcmp(pszName,"g_matShadow") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingShadowMatrix);
		}
		else if ( strcmp(pszName,"g_shadowMapTexelSize") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingShadowMapTexSize);
		}
		else if ( strcmp(pszName,"g_ShadowDepthFade") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingShadowDepthFade);
		}
		else if ( strcmp(pszName,"INVERSE_TRANSPOSE_WORLD_MATRIX") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetInverseTransposeWorldMatrix);
		}
		else if ( strcmp(pszName,"INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetInverseTransposeWorldViewMatrix);
		}
		else if ( strcmp(pszName,"CAMERA_WORLD_POSITION") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetCameraWorldPosition);
		}
		else if ( strcmp(pszName,"CAMERA_VIEW_POSITION") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetCameraViewPosition);
		}
		else if ( strcmp(pszName,"SCENE_LIGHT_COLOR") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetLightColor);
		}
		else if ( strcmp(pszName,"SCENE_LIGHT_DIRECTION") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetLightDirection);
		}
		else if ( strcmp(pszName,"DepthNearFarInvfar") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBingingDepthNearFarInvfar);
		}
		else if ( strcmp(pszName,"TextureSceneDiffuse") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBingingSceneDiffuse);
		}
		else if ( strcmp(pszName,"TextureSceneDepth") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBingingSceneDetph);
		}
		else if ( strcmp(pszName,"TextureSceneDeviceDepth") == 0 )
		{
			pParam->BindMethod(this,&ParameterManager::autoBingingSceneDetph);
		}
		else if ( strcmp(pszName,"TextureSceneNormal") == 0 )
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingSceneNormal);
		}
		else if ( strcmp(pszName,"TextureLightShadow") == 0 )
		{
			pParam->BindMethod(this,&ParameterManager::autoBindingTextureLightShadow);
		}
		else
		{
			ASSERT("Unsupported auto binding type ");
		}
	}

	SamplerState* ParameterManager::autoBindingShadowMap() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return NULL;

		return pCurScene->GetDirLight()->GetShadowMapFrustum(0).GetShadowMap();
	}

	SamplerState* ParameterManager::autoBindingDeviceDepthMap() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return NULL;

		return pCurScene->GetRenderScheme()->GetSceneDepth();
	}

	const Matrix4& ParameterManager::autoBindingShadowMatrix() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return Matrix4::IDENTITY;

		return pCurScene->GetDirLight()->GetShadowMapFrustum(0).GetShadowMatrix();
	}

	Vector4	ParameterManager::autoBindingShadowMapTexSize() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return Vector4::ZERO;

		SamplerState* pShadowMap = pCurScene->GetDirLight()->GetShadowMapFrustum(0).GetShadowMap();
		Texture* pTexture = pShadowMap->GetTexture();
		return Vector4((float)pTexture->GetWidth(), 1.0f / (float)pTexture->GetWidth(), 0, 0);
	}

	Vector4	ParameterManager::autoBindingShadowDepthFade() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return Vector4::ZERO;

		return Vector4::ZERO;//pCurScene->GetSunShaow()->GetShadowDepthFade();
	}

	float ParameterManager::autoBindingShadowExt() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return 0;

		return 0;//pCurScene->GetSunShaow()->GetShadowDepthFade();
	}

	const Matrix4& ParameterManager::autoBindingGetWorldMatrix() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return Matrix4::IDENTITY;

		return pRenderable->GetWorldMatrix();
	}

	const Matrix4& ParameterManager::autoBindingGetViewMatrix() const
	{
		return GetRenderContext()->GetViewMatrix();
	}

	const Matrix4& ParameterManager::autoBindingGetProjectionMatrix() const
	{
		return GetRenderContext()->GetProjMatrix();;
	}

	Matrix4 ParameterManager::autoBindingGetWorldViewMatrix() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return Matrix4::IDENTITY;

		return GetRenderContext()->GetViewMatrix() * pRenderable->GetWorldMatrix();
	}

	Matrix4 ParameterManager::autoBindingGetViewProjectionMatrix() const
	{
		return GetRenderContext()->GetViewProjMatrix();
	}

	Matrix4 ParameterManager::autoBindingGetWorldViewProjectionMatrix() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return Matrix4::IDENTITY;

		return GetRenderContext()->GetViewProjMatrix() * pRenderable->GetWorldMatrix();
	}

	const Matrix4& ParameterManager::autoBindingGetInverseTransposeWorldMatrix() const
	{
		return /*_nodeBinding ? _nodeBinding->getInverseTransposeWorldMatrix() :*/ Matrix4::IDENTITY;
	}

	const Matrix4& ParameterManager::autoBindingGetInverseTransposeWorldViewMatrix() const
	{
		return /*_nodeBinding ? _nodeBinding->getInverseTransposeWorldViewMatrix() :*/ Matrix4::IDENTITY;
	}
		
	Matrix4 ParameterManager::autoBindingGetInverseProjectionMatrix() const
	{
		return GetRenderContext()->GetProjMatrix().inverse();
	}

	Vector3 ParameterManager::autoBindingGetCameraWorldPosition() const
	{
		return GetRenderContext()->GetViewMatrix().inverse().getTrans();
	}

	Vector3 ParameterManager::autoBindingGetCameraViewPosition() const
	{
		return /*_nodeBinding ? _nodeBinding->getActiveCameraTranslationView() :*/ Vector3::ZERO;
	}

	Vector4	ParameterManager::autoBindingGetCameraNearFar() const
	{
		float fZNear = GetRenderContext()->GetNearClip();
		float fZFar = GetRenderContext()->GetFarClip();
		Vector4 vCameaNearFar(fZNear,fZFar,1.0f / fZFar,0);
		return vCameaNearFar;
	}

// 	const Vector4* ParameterManager::autoBindingGetMatrixPalette() const
// 	{
// 		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
// 		if (pRenderable == NULL)
// 			return NULL;
// 
// 		return (const Vector4*)pRenderable->GetSkinDQ();
// 	}

// 	UINT ParameterManager::autoBindingGetMatrixPaletteSize() const
// 	{
// 		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
// 		if (pRenderable == NULL)
// 			return 0;
// 
// 		return pRenderable->GetSkinDQCount() * 2;
// 	}

// 	const ColourValue& ParameterManager::autoBindingGetAmbientColor() const
// 	{
// 		return GetLightSystem()->GetAmbientColor();
// 	}

	const ColourValue& ParameterManager::autoBindingGetLightColor() const
	{
		Light* pCurLight = GetRenderContext()->GetCurLight();
		if (pCurLight == NULL)
			return ColourValue::White;

		return pCurLight->GetLightColor();
	}

	Vector3 ParameterManager::autoBindingGetLightDirection() const
	{
		Light* pCurLight = GetRenderContext()->GetCurLight();
		if (pCurLight == NULL || pCurLight->GetLightType() != LIGHT_DIRECTIONAL)
		{
			ASSERT(false);
			return Vector3::UNIT_SCALE;
		}
		
		DirectonalLight* pDirLigt = (DirectonalLight*)pCurLight;
		return -pDirLigt->GetSceneNode()->GetForward();
	}

	Vector4 ParameterManager::autoBingingDepthNearFarInvfar() const
	{
		float fNear = GetRenderContext()->GetNearClip();
		float fFar = GetRenderContext()->GetFarClip();
		
		return Vector4(fNear,fFar,1.0f/fFar,0);
	}

	SamplerState* ParameterManager::autoBingingSceneDiffuse() const
	{
		return GetRenderContext()->GetCurScene()->GetRenderScheme()->GetSceneDiffuse();
	}

	SamplerState* ParameterManager::autoBingingSceneDetph() const
	{
		return GetRenderContext()->GetCurScene()->GetRenderScheme()->GetSceneDepth();
	}

	SamplerState* ParameterManager::autoBindingSceneNormal() const
	{
		return GetRenderContext()->GetCurScene()->GetRenderScheme()->GetSceneNormal();
	}

	SamplerState* ParameterManager::autoBindingTextureLightShadow() const
	{
		if (GetRenderContext()->GetCurScene()->GetRenderScheme()->GetDeferredShadowPass() == NULL)
			return NULL;

		return GetRenderContext()->GetCurScene()->GetRenderScheme()->GetDeferredShadowPass()->GetShadowSampler();

	}

}
