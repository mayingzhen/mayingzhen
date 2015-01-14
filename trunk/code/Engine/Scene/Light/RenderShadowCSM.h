#pragma once

#include "ShadowMapFrustum.h"

namespace ma
{

	#define MAX_FRUSTUM_SPLIT_NUM 4
	#define MAX_SHADOW_SAMPLES_NUM 16

	enum Shadow_Blur
	{
		BLUR_NO,
		SHADOW_PCF2x2,
		SHADOW_JITTERIN,
	};

	enum Caster_Cull
	{
		CasterCull_No,			// 直接使用视锥裁剪结果
		LightViewFrustum_Cull,  // 光照空间的view Frustum
		LightFrustum_Cull,		// 完整的Light Frustum
	};

	class RenderShadowCSM : public DirectonalLight
	{
	public:
		RenderShadowCSM();

		~RenderShadowCSM();

		void				SetEnabled(bool b);
		bool				GetEnabled() {return m_bEnable;}

		void				Init();
		void				Update(Camera* pCamera);
		void				Render(Camera* pCamera);
		void				Clear(Camera* pCamera);

		
		const Matrix4*		GetShadowMatrix() const {return m_matShadow;}
		Texture*			GetShadowMap() const; 

		void				SetShadowBlurLevel(Shadow_Blur eBlur);
		Shadow_Blur			GetShadowBlurLevel() const {return m_eShadowBleurLevel;}

		// Shadow Irreg
		const	Vector4*	GetIrregKernel() const {return m_irreg_kernel;}
		uint32				GetIrregKernelSize() const {return m_ShadowSamplesNumer / 2;}
		Texture*			GetRotSampler() const {return m_pRotSampler.get();}		
		int					GetShadowSamplesNum() const {return m_ShadowSamplesNumer;}
		void				SetShadowSamplesNum(int nNum);
		const Vector4*		GetViewPosVecLS() const {return m_viewPosVecLS;}

		// 
		void				SetMaxSplitCount(int nMaxSplitCount);
		int					GetMaxSplitCount() const {return m_nMaxSplitCount;}

		void				SetSplitPosParam(Vector4 vSpiltPos) {m_SplitPosParam = vSpiltPos;}
		const Vector4&		GetCurSplitPos() const {return m_curSplitPos;}
		
		int					GetShadowMapSize() const {return m_nShadowMapSize;}
		void				SetShadowMapSize(int nSize);

		void				SetShadowFadeStart(float fFadeStart) {m_fShadowFadeStart = fFadeStart;}
		float				GetShadowFadeStart() const {return m_fShadowFadeStart;}
		const Vector4&		GetShadowDepthFade() {return m_vShadowDepthFade;}
		
		void				GetDepthBiasParams(float& fConstantBias, float& fSlopeScaleBias) const;
		void				SetDepthBiasParams(float fConstantBias, float fSlopeScaleBias);

		void				SetShadowFarDistance(float distance) {m_fShadowFarDist = distance;}
		float				GetShadowFarDistance() const{ return m_fShadowFarDist; }

		Caster_Cull			GetCasterCullType() const {return m_eCaterType;}
		void				SetCasterCullType(Caster_Cull eCasterCull) {m_eCaterType = eCasterCull;}

	private:
		void				CreateShadowMap(int nSize);

	private:
		int					m_nCurSplitCount;
		int					m_nMaxSplitCount;
		Vector4				m_SplitPosParam;
		Vector4				m_curSplitPos;

		RefPtr<Texture>		m_pShdowDepth;
		RefPtr<Texture>		m_pDepthStencil;
		int					m_nShadowMapSize;
		Rectangle			m_viewport;

		ShadowMapFrustum	m_SpitFrustum[MAX_FRUSTUM_SPLIT_NUM];
		Matrix4				m_matShadow[MAX_FRUSTUM_SPLIT_NUM];

		// Shadow irreg
		int					m_ShadowSamplesNumer;
		Vector4				m_irreg_kernel[MAX_SHADOW_SAMPLES_NUM / 2];
		Vector4				m_viewPosVecLS[MAX_FRUSTUM_SPLIT_NUM];
		RefPtr<Texture>		m_pRotSampler;

		float				m_fConstantBias;
		float				m_fSlopeScaledBias;	

		float				m_fShadowFarDist;

		Vector4				m_vShadowDepthFade;
		float				m_fShadowFadeStart;

		Shadow_Blur			m_eShadowBleurLevel;

		Caster_Cull			m_eCaterType;

		bool				m_bEnable;
	};

	extern RenderShadowCSM* g_pRenderShadowCSM;
	RenderShadowCSM* GetRenderShadowCSM();

}