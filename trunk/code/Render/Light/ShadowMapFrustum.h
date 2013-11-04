#ifndef  _ShadowMapFrustum__H__
#define  _ShadowMapFrustum__H__


namespace ma
{
	class ShadowMapFrustum
	{
	public:
		ShadowMapFrustum(Light* pLight,int nIndex);
		
		~ShadowMapFrustum();
		
		void				SetViewMatrix(const Matrix4x4& matView); 

		void				SetProjMatrix(const Matrix4x4& matPoj); 

		const Matrix4x4&	GetViewMarix();  

		const Matrix4x4&	GetProjMatrix(); 

		const Matrix4x4&	GetViewProjMatrix();

		const Matrix4x4&	GetTexScaleBiasMat() {return m_TexScaleBiasMat;}

		void				RenderShadowMap();

		Texture*			GetDepthTexture();

		void				ClearCasterList(); 

		void				AddCaster(RenderObject* pRenderObj); 

	private:
		Texture*					m_pShdowDepth;

		Texture*					m_pDepthStencil;

		MatViewProj					m_matViewProj[2];

		Matrix4x4					m_TexScaleBiasMat;

		Light*						m_pLight; 
		
		int							m_nIndex; 

		std::vector<RenderObject*>	m_arrCasterList[2];
	};
}

#endif
