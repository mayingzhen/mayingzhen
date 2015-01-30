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
		CasterCull_No,			// ֱ��ʹ����׶�ü����
		LightViewFrustum_Cull,  // ���տռ��view Frustum
		LightFrustum_Cull,		// ������Light Frustum
	};

	class SamplerState;

	class RenderShadowCSM 
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

		
		const Matrix4*		GetShadowMatrix() const {return m_matShadow[GetRenderSystem()->CurThreadProcess()];}
		Texture*			GetShadowMap() const;

		void				SetShadowBlurLevel(Shadow_Blur eBlur);
		Shadow_Blur			GetShadowBlurLevel() const {return m_eShadowBleurLevel;}

		// Shadow Irreg
		const	Vector4*	GetIrregKernel() const {return m_irreg_kernel;}
		uint32				GetIrregKernelSize() const {return m_ShadowSamplesNumer / 2;}
		Texture*			GetRotSampler() const {return m_pRotSampler.get();}		
		int					GetShadowSamplesNum() const {return m_ShadowSamplesNumer;}
		void				SetShadowSamplesNum(int nNum);
		const Vector4*		GetViewPosVecLS() const {return m_viewPosVecLS[GetRenderSystem()->CurThreadProcess()];}

		// 
		void				SetMaxSplitCount(int nMaxSplitCount);
		UINT				GetMaxSplitCount() const {return m_nMaxSplitCount;}

		void				SetSplitPosParam(Vector4 vSpiltPos) {m_SplitPosParam = vSpiltPos;}
		const Vector4&		GetCurSplitPos() const {return m_curSplitPos[GetRenderSystem()->CurThreadProcess()];}
		
		int					GetShadowMapSize() const {return m_nShadowMapSize;}
		void				SetShadowMapSize(int nSize);
		Vector4				GetShadowMapTexSize() const {return Vector4(m_nShadowMapSize, 1.0f / m_nShadowMapSize, 0, 0);}

		void				SetShadowFadeStart(float fFadeStart) {m_fShadowFadeStart = fFadeStart;}
		float				GetShadowFadeStart() const {return m_fShadowFadeStart;}
		const Vector4&		GetShadowDepthFade() const {return m_vShadowDepthFade[GetRenderSystem()->CurThreadProcess()];}
		
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
		Vector4				m_curSplitPos[2];

		RefPtr<Texture>		m_pShdowDepth;
		RefPtr<Texture>		m_pDepthStencil;
		int					m_nShadowMapSize;
		Rectangle			m_viewport;
		//RefPtr<SamplerState> m_pShadowMap;

		ShadowMapFrustum	m_SpitFrustum[MAX_FRUSTUM_SPLIT_NUM];
		Matrix4				m_matShadow[2][MAX_FRUSTUM_SPLIT_NUM];

		// Shadow irreg
		int					m_ShadowSamplesNumer;
		Vector4				m_irreg_kernel[MAX_SHADOW_SAMPLES_NUM / 2];
		Vector4				m_viewPosVecLS[2][MAX_FRUSTUM_SPLIT_NUM];
		RefPtr<Texture>		m_pRotSampler;

		float				m_fConstantBias;
		float				m_fSlopeScaledBias;	

		float				m_fShadowFarDist;

		Vector4				m_vShadowDepthFade[2];
		float				m_fShadowFadeStart;

		Shadow_Blur			m_eShadowBleurLevel;

		Caster_Cull			m_eCaterType;

		bool				m_bEnable;
	};

	extern RenderShadowCSM* g_pRenderShadowCSM;
	RenderShadowCSM* GetRenderShadowCSM();

}