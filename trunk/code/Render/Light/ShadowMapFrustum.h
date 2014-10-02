#ifndef  _ShadowMapFrustum__H__
#define  _ShadowMapFrustum__H__


namespace ma
{
	class ShadowMapFrustum
	{
	public:
		ShadowMapFrustum(Light* pLight,int nIndex);
		
		~ShadowMapFrustum();
		
		void				SetViewMatrix(const Matrix4& matView); 

		void				SetProjMatrix(const Matrix4& matPoj); 

		const Matrix4&	GetViewMarix();  

		const Matrix4&	GetProjMatrix(); 

		const Matrix4&	GetViewProjMatrix();

		const Matrix4&	GetTexScaleBiasMat() {return m_TexScaleBiasMat;}

		void				RenderShadowMap();

		Texture*			GetDepthTexture();

		void				ClearCasterList(); 

		void				AddCaster(RenderablePtr pRenderObj); 

	private:
		Texture*					m_pShdowDepth;

		Texture*					m_pDepthStencil;

		MatViewProj					m_matViewProj[2];

		Matrix4					m_TexScaleBiasMat;

		Light*						m_pLight; 
		
		int							m_nIndex; 

		std::vector<RenderablePtr>	m_arrCasterList[2];
	};
}

#endif
