#include "ParameterManager.h"
#include "../RenderScheme/GBufferPass.h"
#include "../RenderScheme/DeferredLightPass.h"
#include "../RenderScheme/DeferredShadowPass.h"
#include "../Scene/Light/LightSystem.h"

namespace ma
{
	ParameterManager* g_pParameterManager = NULL;

	ParameterManager* GetParameterManager()
	{
		return g_pParameterManager;
	}

	ParameterManager::ParameterManager() 
	{
		m_autoDefaultBings["g_matView"] = g_matView;
		m_autoDefaultBings["g_matViewInv"] = g_matViewInv;
		m_autoDefaultBings["g_matProj"] = g_matProj;
		m_autoDefaultBings["g_matProjInv"] = g_matProjInv;
		m_autoDefaultBings["g_matWorld"] = g_matWorld;
		m_autoDefaultBings["g_matViewProj"] = g_matViewProj;
		m_autoDefaultBings["g_matWorldViewProj"] = g_matWorldViewProj;
		m_autoDefaultBings["g_matWorldView"] = g_matWorldView;
		m_autoDefaultBings["g_tShadowMap"] = g_tShadowMap;
		m_autoDefaultBings["g_fSplitPlane"] = g_fSplitPlane;
		m_autoDefaultBings["g_matShadow"] = g_matShadow;
		m_autoDefaultBings["g_shadowMapTexelSize"] = g_shadowMapTexelSize;
		m_autoDefaultBings["g_ShadowDepthFade"] = g_ShadowDepthFade;
		m_autoDefaultBings["g_matShadow"] = g_matShadow;
		//m_autoDefaultBings["boneDQ"] = MATRIX_PALETTE;
		m_autoDefaultBings["g_vCameraNearFar"] = DepthNearFarInvfar;
		m_autoDefaultBings["u_textureSceneDiffuse"] =  TextureSceneDiffuse;
		m_autoDefaultBings["u_textureSceneDepth"] =  TextureSceneDepth;
		m_autoDefaultBings["u_textureSceneNormal"] =  TextureSceneNormal;
		m_autoDefaultBings["u_TextureSceneShadow"] =  TextureLightShadow;
		//m_autoDefaultBings["u_textureLightDiffuse"] =  TextureLightDiffuse;
		//m_autoDefaultBings["u_textureLightSpecular"] =  TextureLightSpecular;
	}

	ParameterManager::~ParameterManager()
	{
	
	}

	void ParameterManager::UseDefaultBing(Uniform* pParam)
	{
		ASSERT(pParam);
		if (pParam == NULL)
			return;

		std::map<std::string, AutoBinding>::iterator itr =  m_autoDefaultBings.find(pParam->GetName());
		if (itr != m_autoDefaultBings.end())
		{
			SetParameterAutoBinding(pParam,itr->second);
		}
	}

	void ParameterManager::SetParameterAutoBinding(Uniform* pParam,AutoBinding autoBinding)
	{
		ASSERT(pParam);
		if (pParam == NULL)
			return;

		if (autoBinding == g_matWorld)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetWorldMatrix);
		}
		else if (autoBinding == g_matView)
		{
			pParam->BindMethod(GetRenderContext(),&RenderContext::GetViewMatrix);
		}
		else if (autoBinding == g_matViewInv)
		{
			pParam->BindMethod(GetRenderContext(),&RenderContext::GetViewMatrixInv);
		}
		else if (autoBinding == g_matProj)
		{
			pParam->BindMethod(GetRenderContext(),&RenderContext::GetViewMatrix);
		}
		else if (autoBinding == g_matProjInv)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetInverseProjectionMatrix);
		}
		else if (autoBinding == g_matWorldView)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetWorldViewMatrix);
		}
		else if (autoBinding == g_matViewProj)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetViewProjectionMatrix);
		}
		else if (autoBinding == g_matWorldViewProj)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetWorldViewProjectionMatrix);
		}
		else if (autoBinding == g_tShadowMap)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingShadowMap);
		}
		else if (autoBinding == g_fSplitPlane)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingSpitPos);
		}
		else if (autoBinding == g_matShadow)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingShadowMatrix,&ParameterManager::autoBindingSplitCount);
		}
		else if (autoBinding == g_shadowMapTexelSize)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingShadowMapTexSize);
		}
		else if (autoBinding == g_ShadowDepthFade)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingShadowDepthFade);
		}
		else if (autoBinding == INVERSE_TRANSPOSE_WORLD_MATRIX)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetInverseTransposeWorldMatrix);
		}
		else if (autoBinding == INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetInverseTransposeWorldViewMatrix);
		}
		else if (autoBinding == CAMERA_WORLD_POSITION)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetCameraWorldPosition);
		}
		else if (autoBinding == CAMERA_VIEW_POSITION)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetCameraViewPosition);
		}
// 		else if (autoBinding == MATRIX_PALETTE)
// 		{
// 			pParam->BindMethod(this, &ParameterManager::autoBindingGetMatrixPalette, &ParameterManager::autoBindingGetMatrixPaletteSize);
// 		}
		else if (autoBinding == SCENE_AMBIENT_COLOR)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetAmbientColor);
		}
		else if (autoBinding == SCENE_LIGHT_COLOR)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetLightColor);
		}
		else if (autoBinding == SCENE_LIGHT_DIRECTION)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetLightDirection);
		}
		else if (autoBinding == DepthNearFarInvfar)
		{
			pParam->BindMethod(this, &ParameterManager::autoBingingDepthNearFarInvfar);
		}
		else if (autoBinding == TextureSceneDiffuse)
		{
			pParam->BindMethod(this, &ParameterManager::autoBingingSceneDiffuse);
		}
		else if (autoBinding == TextureSceneDepth)
		{
			pParam->BindMethod(this, &ParameterManager::autoBingingSceneDetph);
		}
		else if (autoBinding == TextureSceneNormal)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingSceneNormal);
		}
// 		else if (autoBinding == TextureLightDiffuse)
// 		{
// 			pParam->BindMethod(this, &ParameterManager::autoBindingTextureLightDiffuse);
// 		}
// 		else if (autoBinding == TextureLightSpecular)
// 		{
// 			pParam->BindMethod(this, &ParameterManager::autoBindingTextureightSpecular);
// 		}
		else if (autoBinding == TextureLightShadow)
		{
			pParam->BindMethod(this,&ParameterManager::autoBindingTextureLightShadow);
		}
		else
		{
			ASSERT("Unsupported auto binding type " && autoBinding);
		}
	}

	Texture* ParameterManager::autoBindingShadowMap() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return NULL;

		return pCurScene->GetSunShaow()->GetShadowMapFrustum(0).GetShadowMap();
	}

	Vector4	ParameterManager::autoBindingSpitPos() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return Vector4::ZERO;

		return pCurScene->GetSunShaow()->GetCurSplitPos();
	}

	UINT ParameterManager::autoBindingSplitCount() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return 0;

		return pCurScene->GetSunShaow()->GetMaxSplitCount();
	}

	const Matrix4* ParameterManager::autoBindingShadowMatrix() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return NULL;

		return &pCurScene->GetSunShaow()->GetShadowMapFrustum(0).GetShadowMatrix();
	}

	Vector4	ParameterManager::autoBindingShadowMapTexSize() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return Vector4::ZERO;

		return pCurScene->GetSunShaow()->GetShadowMapTexSize();
	}

	Vector4	ParameterManager::autoBindingShadowDepthFade() const
	{
		Scene* pCurScene = GetRenderContext()->GetCurScene();
		if (pCurScene == NULL)
			return Vector4::ZERO;

		return pCurScene->GetSunShaow()->GetShadowDepthFade();
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

	const ColourValue& ParameterManager::autoBindingGetAmbientColor() const
	{
		return GetLightSystem()->GetAmbientColor();
	}

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
		return pDirLigt->GetForward();
	}

	Vector4 ParameterManager::autoBingingDepthNearFarInvfar() const
	{
		float fNear = GetRenderContext()->GetNearClip();
		float fFar = GetRenderContext()->GetFarClip();
		
		return Vector4(fNear,fFar,1.0f/fFar,0);
	}

	Texture* ParameterManager::autoBingingSceneDiffuse() const
	{
		if (GetRenderContext()->GetCurScene()->GetRenderScheme()->GetGBufferPass() == NULL)
			return NULL;

		return GetRenderContext()->GetCurScene()->GetRenderScheme()->GetGBufferPass()->GetSceneDiffuse();
	}

	Texture* ParameterManager::autoBingingSceneDetph() const
	{
		if (GetRenderContext()->GetCurScene()->GetRenderScheme()->GetGBufferPass() == NULL)
			return NULL;

		return GetRenderContext()->GetCurScene()->GetRenderScheme()->GetGBufferPass()->GetSceneDepth();
	}

	Texture* ParameterManager::autoBindingSceneNormal() const
	{
		if (GetRenderContext()->GetCurScene()->GetRenderScheme()->GetGBufferPass() == NULL)
			return NULL;

		return GetRenderContext()->GetCurScene()->GetRenderScheme()->GetGBufferPass()->GetSceneNormal();
	}

// 	Texture* ParameterManager::autoBindingTextureLightDiffuse() const
// 	{
// 		if (GetRenderContext()->GetCurScene()->GetRenderScheme()->GetDeferredLightPass() == NULL)
// 			return NULL;
// 
// 		return GetRenderContext()->GetCurScene()->GetRenderScheme()->GetDeferredLightPass()->GetTextureLightDiffuse();
// 	}
// 
// 	Texture* ParameterManager::autoBindingTextureightSpecular() const
// 	{
// 		if (GetRenderContext()->GetCurScene()->GetRenderScheme()->GetDeferredLightPass() == NULL)
// 			return NULL;
// 
// 		return GetRenderContext()->GetCurScene()->GetRenderScheme()->GetDeferredLightPass()->GetTextureightSpecular();
// 	}	

	Texture* ParameterManager::autoBindingTextureLightShadow() const
	{
		if (GetRenderContext()->GetCurScene()->GetRenderScheme()->GetDeferredShadowPass() == NULL)
			return NULL;

		return GetRenderContext()->GetCurScene()->GetRenderScheme()->GetDeferredShadowPass()->GetShadowTexture();

	}

}
