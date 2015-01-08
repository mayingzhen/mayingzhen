#ifndef  _Light__H__
#define  _Light__H__

#include "Engine/Scene/Light/ShadowMapFrustum.h"

namespace ma
{
	class ShadowMapFrustum;
	class Camera;
	class Texture;

	#define MAX_FRUSTUM_SPLIT_NUM 4
	#define MAX_SHADOW_SAMPLES_NUM 16

	enum LightType
	{
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,			
		LIGHT_POINT,	
	};

	enum Caster_Cull
	{
		CasterCull_No,			// 直接使用视锥裁剪结果
		LightViewFrustum_Cull,  // 光照空间的view Frustum
		LightFrustum_Cull,		// 完整的Light Frustum
	};

	enum Shadow_Blur
	{
		BLUR_NO,
		SHADOW_PCF2x2,
		SHADOW_JITTERIN,
	};


	class RENDER_API Light : public RenderComponent
	{
		DECL_OBJECT(Light)

	public:
		Light();

		~Light();

		LightType					GetLightType() {return m_eLightType;}

		const ColourValue&			GetLightColor() {return m_cLightColor;}

		void						SetLightColor(const ColourValue& cLightColor) {m_cLightColor = cLightColor;}

		void						SetLightIntensity(float fIntensity) {m_fLightIntensity = fIntensity;}

		float						GetLightIntensity() {return m_fLightIntensity;}

		bool						IsCreateShadow() {return m_bCreateShadow;}

		virtual void				SetCreateShadow(bool bCreateShaow) {m_bCreateShadow = bCreateShaow;}

		virtual void				UpdateShadowFrustum(Camera* pCamera) {}

// 		UINT						GetShadowFrustumNumber() {return m_arrShadowFrustum.size();}
// 
// 		ShadowMapFrustum*			GetShadowFrustumByIndex(UINT index) {return m_arrShadowFrustum[index];}
	
		void						SetShadowFarDistance(float distance) {m_fShadowFarDist = distance;}
		float						GetShadowFarDistance() const{ return m_fShadowFarDist; }

		void						GetDepthBiasParams(float& fConstantBias, float& fSlopeScaleBias) const;
		void						SetDepthBiasParams(float fConstantBias, float fSlopeScaleBias);

		Caster_Cull					GetCasterCullType() const {return m_eCaterType;}
		void						SetCasterCullType(Caster_Cull eCasterCull) {m_eCaterType = eCasterCull;}

	protected:
		LightType					m_eLightType;

		ColourValue					m_cLightColor;

		float						m_fLightIntensity;

		bool						m_bCreateShadow;
		
		float						m_fConstantBias;
		float						m_fSlopeScaledBias;	

		float						m_fShadowFarDist;

		Vector4						m_vShadowDepthFade;
		float						m_fShadowFadeStart;	

		Caster_Cull					m_eCaterType;

		Shadow_Blur					m_eShadowBleurLevel;

		RefPtr<Texture>				m_pShdowDepth;
		RefPtr<Texture>				m_pDepthStencil;
		int							m_nShadowMapSize;
		
	};

	class RENDER_API PointLight : public Light
	{
		DECL_OBJECT(PointLight)

	public:
		PointLight() {m_eLightType = LIGHT_POINT;}
		
		Vector3			GetPos();

		float			GetRadius() {return m_fRadius;}

		void			SetRadius(float fRadius) {m_fRadius = fRadius ? fRadius : 1.0f;}

	private:

		float		m_fRadius;
	};
	
	class RENDER_API DirectonalLight : public Light
	{
		DECL_OBJECT(DirectonalLight)

	public:
		DirectonalLight();

		Vector3			GetDirection();

		virtual void	SetCreateShadow(bool bCreateShaow);

		const Vector4&	GetSplitPos();

		virtual void	Show(Camera* pCamera); 
		

	private:
		Matrix4			CalculateCropMatrix(const AABB& cropAABB);

	private:
		
		// Shadow
		int					m_nCurSplitCount;
		int					m_nMaxSplitCount;
		Vector4				m_SplitPosParam;
		Vector4				m_curSplitPos;

		Rectangle			m_viewport;	

		ShadowMapFrustum	m_SpitFrustum[MAX_FRUSTUM_SPLIT_NUM];
		Matrix4				m_matShadow[MAX_FRUSTUM_SPLIT_NUM];

		// Shadow irreg
		int					m_ShadowSamplesNumer;
		Vector4				m_irreg_kernel[MAX_SHADOW_SAMPLES_NUM / 2];
		Vector4				m_viewPosVecLS[MAX_FRUSTUM_SPLIT_NUM];
		RefPtr<Texture>		m_pRotSampler;

		bool				m_bEnable;

	
	};


	class RENDER_API SpotLight : public Light
	{
		DECL_OBJECT(SpotLight)

	public:
		SpotLight()	{m_eLightType = LIGHT_SPOT;}

		Transform		GetTransform() {return m_tsfWS;}

		void			SetTransform(const Transform& tsfWS) {m_tsfWS = tsfWS;}

		//void			LookAt(const Vector3& vEye,const Vector3& vAt,const Vector3& vUp);	

		Matrix4			GetViewMatrix() {return m_mView;}

		Matrix4			GetProjmatrix()	{return m_mProj;}

	private:
		Transform		m_tsfWS;

		float			m_fAspectRatio;
		float			m_fNearClip;
		float			m_fFarClip;
		float			m_fFOV;

		Vector3			m_vTarget;
		Vector3			m_vUpVector;
		Matrix4			m_mView;
		Matrix4			m_mProj;
	};


	DeclareRefPtr(PointLight);
	DeclareRefPtr(DirectonalLight);

}

#endif


