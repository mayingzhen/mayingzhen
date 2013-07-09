#ifndef  _ShadowMapFrustum__H__
#define  _ShadowMapFrustum__H__


namespace ma
{
	class ShadowMapFrustum
	{
	public:
		ShadowMapFrustum();
		
		~ShadowMapFrustum();

		void				Init();

		void				Update();

		const Matrix4x4&	GetViewProjMarix()  {return m_viewProjCropMat;}

		const Matrix4x4&	GetTexScaleBiasMatrix() {return m_TexScaleBiasMat;}

		void				ShadowDepthPass();
	

	private:
		Matrix4x4			CalculateCropMatrix();

	private:
		RenderTarget*				m_pShdowDepth;

		Matrix4x4					m_TexScaleBiasMat;
		Matrix4x4					m_viewProjCropMat;

		Light*						m_pLight; 
		Camera*						m_pCamera;
		
		int							m_nIndex; 
		std::vector<Renderable*>	m_arrCasterList;
	};
}

#endif
