#pragma once

namespace ma
{
	class Light;
	class Texture;
	class RenderPass;
	class ShadowMapRenderView;

	enum Shadow_Blur
	{
		BLUR_NO,
		SHADOW_PCF2x2,
		SHADOW_JITTERIN,
	};

	class ShadowRenderStep : public Referenced
	{
	public:
		ShadowRenderStep();
		
		~ShadowRenderStep();

		void				InitShadowMap(DirLightProxy* pParent, Rectangle viewPort, RenderPass* pSMPass);

		void				PrepareRender(SceneContext* sc, CullTree* cull);

		void				Update(SceneContext* sc, CullTree* cull, float fSpiltNear,float fSpiltFar);

		void				Clear();

		const Matrix4&		GetLightViewProjMatrix() const { return m_matLightViewProj; }
		const Matrix4&		GetLightViewMatrix() const {return m_matLightView;}
		const Matrix4&		GetTexAdjustMatrix() const {return m_matTexAdjust;}
		const Matrix4&		GetShadowMatrix() const {return m_matShadow;}

		void				SetSplitPost(float fNear, float fFar);

		bool				GetDraw() {return m_bDraw;}

	private:
		void				UpdateDepthBias(SceneContext* sc,float fSpiltNear,float fSpiltFar);
		void				UpdateFrustum(SceneContext* sc,float fSpiltNear,float fSpiltFar);
		void				UpdateBatch(SceneContext* sc, CullTree* cull);
		void				UpdateLightMatrix(SceneContext* sc,float fSpiltNear,float fSpiltFar);
		void				UpdateCropMats();
		void				UpdateDefferedShadow(SceneContext* sc);
		void				QuantizeViewSize(float& fWidth,float& fHight);
		Matrix4				CalculateTexAdjustMatrix(Texture* pShadowMap,Rectangle veiewPort);

	private:

		Matrix4				m_matTexAdjust;
		Matrix4				m_matCrop;	
		Matrix4				m_matLightProj;
		Matrix4				m_matLightView;
		Matrix4				m_matLightViewProj;
		Matrix4				m_matShadow;
		Vector4				m_uvClamp = Vector4(0.0,0.0,1.0,1.0);

		AABB				m_casterAABB;

		AABB				m_sceneAABB;

		Frustum				m_frustum;

		Frustum				m_lightFrustum;

		float				m_fNear = 0.0f;
		float				m_fFar = 0.0f;

		float				m_fSplitPos = 0.0f;

		float				m_fConstantBias;
		float				m_fSlopeScaleBias;

		Shadow_Blur			m_eShadowBleurLevel;

		bool				m_bDraw;
		
		float				m_fShadowFarDist;

		DirLightProxy*		m_pParent;

		RenderPass*			m_shadowMapPass = nullptr;

	public:
		RefPtr<RenderQueue> m_pRenderQueue;
		Rectangle			m_veiwPort;
		BatchRenderable		m_batchRender;
	};
}
