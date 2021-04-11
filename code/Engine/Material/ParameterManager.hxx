#include "ParameterManager.h"
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
		AddFunMethodBinding<Matrix4>("g_matView", [](Renderable*) { 
			return GetSceneContext()->m_matViewProj.GetMatView(); } );

		AddFunMethodBinding<Matrix4>("g_matViewInv", [](Renderable*) {
			return GetSceneContext()->m_matViewProj.GetMatViewInv(); } );

		AddFunMethodBinding<Matrix4>("g_matProj", [](Renderable*) {
			return GetSceneContext()->m_matViewProj.GetMatProj(); } );
		
		AddFunMethodBinding<Matrix4>("g_matProjInv", [](Renderable*) {
			return GetSceneContext()->m_matViewProj.GetMatProj().inverse(); } );

		AddFunMethodBinding<Matrix4>("g_matViewProj", [](Renderable*) {
			return GetSceneContext()->m_matViewProj.GetMatViewProj(); });

		AddFunMethodBinding<Matrix4>("g_matWorld", [](Renderable* pRenderable) {
			return pRenderable ? pRenderable->GetWorldMatrix() : Matrix4::IDENTITY; } );

		AddFunMethodBinding<Matrix4>("g_matWorldView", [](Renderable* pRenderable) {
			return pRenderable ? GetSceneContext()->m_matViewProj.GetMatView() * pRenderable->GetWorldMatrix() : Matrix4::IDENTITY; } );

		AddFunMethodBinding<Matrix4>("g_matWorldViewProj", [](Renderable* pRenderable) {
			return pRenderable ? GetSceneContext()->m_matViewProj.GetMatViewProj() * pRenderable->GetWorldMatrix() : Matrix4::IDENTITY; });

		AddFunMethodBinding<Vector4>("g_vCameraNearFar", [](Renderable*) {
			float fZNear = GetSceneContext()->m_fNear;
			float fZFar = GetSceneContext()->m_fFar;
			Vector4 vCameaNearFar(fZNear, fZFar, 1.0f / fZFar, 0);
			return vCameaNearFar; 
		} );

		AddFunMethodBinding<Vector3>("g_vEyeWorldPos", [](Renderable*) {
			return GetSceneContext()->m_vEyeWordPos; 
		} );

// 		AddFunMethodBinding<Vector3>("g_cSkyLight", [](Renderable*) {
// 			return GetSceneContext()->m_cAmbientColor; 
// 		});
// 
// 		AddFunMethodBinding<Vector3>("g_cDirLight", [](Renderable*) {
// 			return GetSceneContext()->m_cMianLightColor; 
// 		});
// 
// 		AddFunMethodBinding<Vector3>("g_vDirLight", [](Renderable*) {
// 			return GetSceneContext()->GetDirLightDir(); 
// 		});


// 		AddFunMethodBinding<SamplerState*>("g_tShadowMap", [](Renderable*)->SamplerState* {
// 			ASSERT(false);
// 			Scene* pCurScene = nullptr;// GetRenderContext()->GetCurScene();
// 			if (pCurScene == NULL)
// 				return NULL;
// 			return pCurScene->GetMainDirLight()->GetShadowMapFrustum(0).GetShadowMap(); 
// 		});

// 		AddFunMethodBinding<Matrix4>("g_matShadow", [](Renderable*) {
// 			ASSERT(false);
// 			Scene* pCurScene = nullptr;// GetRenderContext()->GetCurScene();
// 			if (pCurScene == NULL)
// 				return Matrix4::IDENTITY;
// 			return pCurScene->GetMainDirLight()->GetShadowMapFrustum(0).GetShadowMatrix();
// 		});

// 		AddFunMethodBinding<Matrix4>("matLightViewProj", [](Renderable*) {
// 			return GetSceneContext()->m_matLightViewProj; 
// 		});

// 		AddFunMethodBinding<Vector4>("g_shadowMapTexelSize", [](Renderable*) {
// 			ASSERT(false);
// 			Scene* pCurScene = nullptr;//GetRenderContext()->GetCurScene();
// 			if (pCurScene == NULL)
// 				return Vector4::ZERO;
// 
// 			SamplerState* pShadowMap = pCurScene->GetMainDirLight()->GetShadowMapFrustum(0).GetShadowMap();
// 			Texture* pTexture = pShadowMap->GetTexture();
// 			return Vector4((float)pTexture->GetWidth(), 1.0f / (float)pTexture->GetWidth(), 0, 0);
// 		});

// 		AddFunMethodBinding<Vector4>("g_ShadowDepthFade", [](Renderable*) {
// 			ASSERT(false);
// 			Scene* pCurScene = nullptr;//GetRenderContext()->GetCurScene();
// 			if (pCurScene == NULL)
// 				return Vector4::ZERO;
// 
// 			return Vector4::ZERO;//pCurScene->GetSunShaow()->GetShadowDepthFade();
// 		});

	}

	ParameterManager::~ParameterManager()
	{
	
	}

	void ParameterManager::AddMethodBinding(const char* pszName, MethodBinding* pMethod)
	{
		auto it = m_autoDefaultBings.find(pszName);
		ASSERT(it == m_autoDefaultBings.end());

		m_autoDefaultBings[pszName] = pMethod;
	}

	MethodBinding* ParameterManager::GetMethodBinding(const char* pszName)
	{
		auto it = m_autoDefaultBings.find(pszName);
		if (it == m_autoDefaultBings.end())
		{
			return nullptr;
		}

		return it->second.get();
	}

}

