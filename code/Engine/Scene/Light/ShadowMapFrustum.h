#pragma once

namespace ma
{
	class Light;
	class Texture;
	class RenderPass;

	enum Shadow_Blur
	{
		BLUR_NO,
		SHADOW_PCF2x2,
		SHADOW_JITTERIN,
	};

	class ShadowMapFrustum
	{
	public:
		ShadowMapFrustum();
		
		~ShadowMapFrustum();

		void				InitShadowMap(DirectonalLight* pParent, Rectangle viewPort, RenderPass* pSMPass);

		void				Update(Camera* pCamera, RenderStep* shadowStep,float fSpiltNear,float fSpiltFar);
		void				Render(Camera* pCamera, RenderStep* shadowStep);

		void				Clear(Camera* pCamera);

		const Matrix4&		GetLightViewProjMatrix() const { return m_matLightViewProj; }
		const Matrix4&		GetLightViewMatrix() const {return m_matLightView;}
		const Matrix4&		GetTexAdjustMatrix() const {return m_matTexAdjust;}
		const Matrix4&		GetShadowMatrix() const {return m_matShadow;}

		bool				GetDraw() {return m_bDraw;}

	private:
		void				UpdateDepthBias(Camera* pCamera,float fSpiltNear,float fSpiltFar);
		void				UpdateFrustum(Camera* pCamera,float fSpiltNear,float fSpiltFar);
		void				UpdateBatch(Camera* pCamera, RenderStep* shadowSte);
		void				UpdateLightMatrix(Camera* pCamera,float fSpiltNear,float fSpiltFar);
		void				UpdateCropMats();
		void				QuantizeViewSize(float& fWidth,float& fHight);
		Matrix4				CalculateTexAdjustMatrix(Texture* pShadowMap,Rectangle veiewPort);

	private:
		typedef vector<RenderComponent*> VEC_CASTER;
		typedef vector<Renderable*>	VEC_RENDERABLE;

		Matrix4				m_matTexAdjust;
		Matrix4				m_matCrop;	
		Matrix4				m_matLightProj;
		Matrix4				m_matLightView;
		Matrix4				m_matLightViewProj;
		Matrix4				m_matShadow;
		Vector4				m_uvClamp = Vector4(0.0,0.0,1.0,1.0);

		AABB				m_casterAABB;
		VEC_CASTER			m_arrCaster;

		//RefPtr<RenderStep> m_shadowMapRender;
		RefPtr<RenderStep> m_subRenderStep;

		AABB				m_sceneAABB;

		Frustum				m_frustum;

		Frustum				m_lightFrustum;

		float				m_fNear = 0.0f;
		float				m_fFar = 0.0f;

		float				m_fConstantBias;
		float				m_fSlopeScaleBias;

		Shadow_Blur			m_eShadowBleurLevel;

		bool				m_bDraw;
		
		float				m_fShadowFarDist;

		DirectonalLight*	m_pParent;
	};
}
