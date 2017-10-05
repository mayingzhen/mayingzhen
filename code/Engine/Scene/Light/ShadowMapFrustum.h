#ifndef  _ShadowMapFrustum__H__
#define  _ShadowMapFrustum__H__


namespace ma
{
	class Light;
	class Texture;
	class RenderPass;
	class RenderQueue;

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

		void				CreateShadowMap(int nSize);	

		SamplerState*		GetShadowMap() const; 

		void				Update(Camera* pCamera,float fSpiltNear,float fSpiltFar);	
		void				Render(Camera* pCamera);

		void				Clear(Camera* pCamera);

		const Matrix4&		GetLightViewProjMatrix() const {return m_matLightViewProj[GetRenderSystem()->CurThreadProcess()];}
		const Matrix4&		GetLightViewMatrix() const {return m_matLightView;}
		const Matrix4&		GetTexAdjustMatrix() const {return m_matTexAdjust;}
		const Matrix4&		GetShadowMatrix() const {return m_matShadow[GetRenderSystem()->CurThreadProcess()];}

		bool				GetDraw() {return m_bDraw[GetRenderSystem()->CurThreadProcess()];}

		RenderPass*			GetShadowMapFrameBuffer() {return m_pShadowMapFB.get();}

	private:
		void				UpdateDepthBias(Camera* pCamera,float fSpiltNear,float fSpiltFar);
		void				UpdateFrustum(Camera* pCamera,float fSpiltNear,float fSpiltFar);
		void				UpdateBatch(Camera* pCamera);
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
		Matrix4				m_matLightViewProj[2];
		Matrix4				m_matShadow[2];

		AABB				m_casterAABB;
		VEC_CASTER			m_arrCaster;

		RenderQueue*		m_pRenderQueue[2];

		AABB				m_sceneAABB;

		Frustum				m_frustum;

		Frustum				m_lightFrustum;

		float				m_fConstantBias[2];
		float				m_fSlopeScaleBias[2];

		Rectangle			m_viewport;	
		
		RefPtr<RenderPass>	m_pShadowMapFB;
		RefPtr<Texture>		m_pShdowMapDepth;
		RefPtr<SamplerState> m_pShadowMapSampler;

		Shadow_Blur			m_eShadowBleurLevel;

		bool				m_bDraw[2];

	public:
		// Defeferred Shadow
		Vector4				m_vWBasisX;
		Vector4				m_vWBasisY;
		Vector4				m_vWBasisZ;
		Vector4				m_vShadowCamPos;

		// Shadow irreg
		Vector4				m_viewPosVecLS;
		Vector2				m_vkernelRadius;
		
		float				m_fShadowFarDist;

		Light*				m_pParent;


	};
}

#endif
