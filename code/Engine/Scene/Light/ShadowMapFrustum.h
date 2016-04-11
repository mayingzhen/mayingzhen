#ifndef  _ShadowMapFrustum__H__
#define  _ShadowMapFrustum__H__


namespace ma
{
	class Light;
	class Texture;

	enum Caster_Cull
	{
		CasterCull_No,			// 直接使用视锥裁剪结果
		LightViewFrustum_Cull,  // 光照空间的view Frustum
		LightFrustum_Cull,		// 完整的Light Frustum
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

		VEC_RENDERABLE		m_arrRenderable[2];

		AABB				m_sceneAABB;

		Frustum				m_frustum;

		float				m_fConstantBias[2];
		float				m_fSlopeScaleBias[2];

		Rectangle			m_viewport;	

		RefPtr<Texture>		m_pShdowMapDepth;
		RefPtr<Texture>		m_pShadowMapColor;
		RefPtr<SamplerState> m_pShadowMapSampler;

		bool				m_bDraw[2];

	public:
		// Defeferred Shadow
		Vector4				m_vWBasisX;
		Vector4				m_vWBasisY;
		Vector4				m_vWBasisZ;
		Vector4				m_vShadowCamPos;

		// Shadow irreg
// 		int					m_ShadowSamplesNumer;
// 		Vector4				m_irreg_kernel[MAX_SHADOW_SAMPLES_NUM / 2];
// 		Vector4				m_viewPosVecLS[2][MAX_FRUSTUM_SPLIT_NUM];
// 		RefPtr<Texture>		m_pRotSampler;

		Caster_Cull			m_eCaterType;
		
		float				m_fShadowFarDist;

		Light*				m_pParent;


	};
}

#endif
