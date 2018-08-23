#pragma once

#include "ShadowMapFrustum.h"

namespace ma
{

	#define MAX_FRUSTUM_SPLIT_NUM 4
	#define MAX_SHADOW_SAMPLES_NUM 16


	class DirectonalLight : public Light
	{
	public:
		DirectonalLight();

		~DirectonalLight();

		DECL_OBJECT(DirectonalLight);

		virtual	void		Update();

		virtual void		Render(RenderQueue* pRenderQueue);

		void				SetShadowEnabled(bool b);
		bool				GetShadowEnabled() {return m_bShadowEnable;}

		void				Init();

		void				UpdateShadowMap(Camera* pCamera);

		void				RenderShadowMap(Camera* pCamera);

		void				Clear(Camera* pCamera);

		void				SetShadowBlurLevel(Shadow_Blur eBlur);
		Shadow_Blur			GetShadowBlurLevel() const {return m_eShadowBleurLevel;}

		// Shadow Irreg
		const	Vector4*	GetIrregKernel() const {return m_irreg_kernel;}
		uint32_t			GetIrregKernelSize() const {return m_ShadowSamplesNumer / 2;}
		SamplerState*		GetRotSampler() const {return m_pRotSampler.get();}		
		int					GetShadowSamplesNum() const {return m_ShadowSamplesNumer;}
		void				SetShadowSamplesNum(int nNum);
		void				SetIrregKernelRadius(const Vector2& vKernelRadiusm,uint32_t nSplit);

		// 
		void				SetMaxSplitCount(int nMaxSplitCount);
		uint32_t			GetMaxSplitCount() const {return m_nMaxSplitCount;}
		int					GetCurSplitCount() const {return m_nCurSplitCount;}

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

		ShadowMapFrustum&	GetShadowMapFrustum(int index) {return m_SpitFrustum[index];}

	private:
		void				UpdateViewMinMaxZ(Camera* pCamera);

	private:
		int					m_nCurSplitCount;
		int					m_nMaxSplitCount;
		Vector4				m_SplitPosParam;
		Vector4				m_curSplitPos[2];

		int					m_nShadowMapSize;

		ShadowMapFrustum	m_SpitFrustum[MAX_FRUSTUM_SPLIT_NUM];

		// Shadow irreg
		int					m_ShadowSamplesNumer;
		Vector4				m_irreg_kernel[MAX_SHADOW_SAMPLES_NUM / 2];
		RefPtr<SamplerState> m_pRotSampler;

		float				m_fConstantBias;
		float				m_fSlopeScaledBias;	

		float				m_fShadowFarDist;

		Vector4				m_vShadowDepthFade[2];
		float				m_fShadowFadeStart;

		Shadow_Blur			m_eShadowBleurLevel;

		bool				m_bShadowEnable;
	};

	RefPtr<DirectonalLight> CreateDirectonalLight();

}
