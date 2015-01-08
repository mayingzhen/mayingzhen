#ifndef  _ShadowMapFrustum__H__
#define  _ShadowMapFrustum__H__


namespace ma
{
	class Light;
	class Texture;

	class ShadowMapFrustum
	{
	public:
		ShadowMapFrustum();
		
		~ShadowMapFrustum();

		void				CreateViewPort(Texture* pShadowMap,Rectangle veiewPort);	

		void				Update(Camera* pCamera,float fSpiltNear,float fSpiltFar);
		
		void				Render(Camera* pCamera,int index);

		void				Clear(Camera* pCamera);

		const Matrix4&		GetShadowMatrix() const {return m_matVPShadow;}
		const Matrix4&		GetLightViewMatrix() const {return m_matLightView;}
		const Matrix4&		GetTexAdjustMatrix() const {return m_matTexAdjust;}

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

		Light*				m_pLight; 

		Matrix4				m_matLightView;
		Matrix4				m_matLightProj;
		Matrix4				m_matCrop;	
		Matrix4				m_matVPShadow;
		Matrix4				m_matTexAdjust;

		AABB				m_casterAABB;
		VEC_CASTER			m_arrCaster;

		AABB				m_sceneAABB;

		Frustum				m_frustum;

		float				m_fConstantBias;
		float				m_fSlopeScaleBias;

		Rectangle			m_viewport;	

		bool				m_bDraw;
	};
}

#endif
