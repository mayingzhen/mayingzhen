#ifndef  _ShadowMapFrustum__H__
#define  _ShadowMapFrustum__H__


namespace ma
{
	class ENGINE_API ShadowMapFrustum
	{
	public:
		ShadowMapFrustum(Light* pLight,int nIndex);
		
		~ShadowMapFrustum();
		
		void				SetViewMatrix(const Matrix4x4& matView); 

		void				SetProjMatrix(const Matrix4x4& matPoj); 

		const Matrix4x4&	GetViewMarix();  

		const Matrix4x4&	GetProjMatrix(); 

		const Matrix4x4&	GetTexScaleBiasMat() {return m_TexScaleBiasMat;}

		void				ShadowDepthPass();

		Texture*			GetDepthTexture() {return m_pShdowDepth/*->GetTexture()*/;}

		void				ClearCasterList(); 

		void				AddCaster(RenderObject* pRenderObj); 

	private:
		Texture*					m_pShdowDepth;

		Texture*					m_pDepthStencil;

		Matrix4x4					m_matView[2];

		Matrix4x4					m_matPoj[2];

		Matrix4x4					m_TexScaleBiasMat;

		Light*						m_pLight; 
		
		int							m_nIndex; 

		std::vector<RenderObject*>	m_arrCasterList[2];
	};
}

#endif
