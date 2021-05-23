#pragma once

#include "ShadowMapFrustum.h"

namespace ma
{
	class DeferredShadow;
	class ShadowRenderView;

	#define MAX_FRUSTUM_SPLIT_NUM 4
	#define MAX_SHADOW_SAMPLES_NUM 16


	class DirectonalLight : public Light
	{
	public:
		DirectonalLight();

		~DirectonalLight();

		DECL_OBJECT(DirectonalLight);

		virtual	void		Update();

		void				SetShadowEnabled(bool b);
		bool				GetShadowEnabled() {return m_bShadowEnable;}

		void				Init();

		void				UpdateShadowMap(Camera* pCamera);

		void				RenderShadowMap(Camera* pCamera);

		void				Clear(Camera* pCamera);

		void				SetShadowBlurLevel(Shadow_Blur eBlur);
		Shadow_Blur			GetShadowBlurLevel() const {return m_eShadowBleurLevel;}

		// 
		void				SetMaxSplitCount(int nMaxSplitCount);
		uint32_t			GetMaxSplitCount() const {return m_nMaxSplitCount;}

		void				SetSplitPosParam(Vector4 vSpiltPos) {m_SplitPosParam = vSpiltPos;}
		const Vector4&		GetCurSplitPos() const {return m_curSplitPos;}
		
		int					GetShadowMapSize() const {return m_nShadowMapSize;}
		void				SetShadowMapSize(int nSize);
		Vector4				GetShadowMapTexSize() const {return Vector4(m_nShadowMapSize, 1.0f / m_nShadowMapSize, 0, 0);}

		void				SetShadowFadeStart(float fFadeStart) {m_fShadowFadeStart = fFadeStart;}
		float				GetShadowFadeStart() const {return m_fShadowFadeStart;}
		const Vector4&		GetShadowDepthFade() const {return m_vShadowDepthFade;}
		
		void				GetDepthBiasParams(float& fConstantBias, float& fSlopeScaleBias) const;
		void				SetDepthBiasParams(float fConstantBias, float fSlopeScaleBias);

		void				SetShadowFarDistance(float distance) {m_fShadowFarDist = distance;}
		float				GetShadowFarDistance() const{ return m_fShadowFarDist; }

		RenderPass*			GetShadowMapPass() { return m_pShadowMapPass.get(); }

		SamplerState*		GetShadowMapSampler() { return m_pShadowMapSampler.get(); }

	private:
		void				UpdateViewMinMaxZ(Camera* pCamera);

		void				CreateShadowMap();

		DirLightProxy*		GetDirLightProxy();
	
	private:
		int					m_nMaxSplitCount;
		Vector4				m_SplitPosParam;
		Vector4				m_curSplitPos;

		int					m_nShadowMapSize;

		ShadowRenderStep	m_SpitFrustum[MAX_FRUSTUM_SPLIT_NUM];

		float				m_fConstantBias;
		float				m_fSlopeScaledBias;	

		float				m_fShadowFarDist;

		Vector4				m_vShadowDepthFade;
		float				m_fShadowFadeStart;

		Shadow_Blur			m_eShadowBleurLevel;

		bool				m_bShadowEnable;

		RefPtr<RenderPass>	m_pShadowMapPass;
		RefPtr<Texture>		m_pShdowMapDepth;
		RefPtr<SamplerState> m_pShadowMapSampler;

		RefPtr<ShadowRenderView> m_ShadowMapView;
	};

	RefPtr<DirectonalLight> CreateDirectonalLight();
 

}
