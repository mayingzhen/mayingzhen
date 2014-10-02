#ifndef  _RenderContext__H__
#define  _RenderContext__H__


namespace ma
{
	class RenderContext
	{
	public:
		RenderContext();

		void					SetCamera(Camera* pCamera); 

		void					SetCurRenderObj(Renderable* pRenderObj) {m_pCurObject = pRenderObj;}

		Renderable*				GetCurRenderObj() {return m_pCurObject;}

		void					SetCurLight(Light* pLight) {m_pCurLight = pLight;}

		Light*					GetCurLight() {return m_pCurLight;}
		
		void					SetViewMatrix(const Matrix4& matView) {m_matViewProj.SetMatView(matView);}

		const Matrix4&			GetViewMatrix()  {return m_matViewProj.GetMatView();}

		void					SetProjMatrix(const Matrix4& matProj) {m_matViewProj.SetMatProj(matProj);}

		const Matrix4&			GetProjMatrix()  {return m_matViewProj.GetMatProj();}

		const Matrix4&			GetViewProjMatrix()  {return m_matViewProj.GetMatViewProj();}

		float					GetNearClip() {return m_fNear;}

		float					GetFarClip() {return m_fFar;}

	protected:
		MatViewProj				m_matViewProj;

		float					m_fNear;

		float					m_fFar;

		Renderable*				m_pCurObject;

		Light*					m_pCurLight;
	};

	RenderContext*	GetRenderContext();

	void			SetRenderContext(RenderContext* pRenderContext);
}

#endif
