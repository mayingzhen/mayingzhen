#ifndef  _RenderContext__H__
#define  _RenderContext__H__


namespace ma
{
	class RenderContext
	{
	public:
		RenderContext();

		void					SetCurRenderObj(Renderable* pRenderObj) {m_pCurObject = pRenderObj;}
		Renderable*				GetCurRenderObj() {return m_pCurObject;}

		void					SetCurLight(Light* pLight) {m_pCurLight = pLight;}
		Light*					GetCurLight() {return m_pCurLight;}

		void					SetCurScene(Scene* pScene) { m_pCurScene = pScene; }
		Scene*					GetCurScene() { return m_pCurScene; }
		
		void					SetCamera(Camera* pCamera); 

		void					SetViewMatrix(const Matrix4& matView);

		const Matrix4&			GetViewMatrix();

		void					SetProjMatrix(const Matrix4& matProj);

		const Matrix4&			GetProjMatrix();

		const Matrix4&			GetViewProjMatrix();

		float					GetNearClip();

		float					GetFarClip();

	protected:
		MatViewProj				m_matViewProj[2];

		float					m_fNear[2];

		float					m_fFar[2];

		Renderable*				m_pCurObject;

		Scene*					m_pCurScene;

		Light*					m_pCurLight;
	};

	RenderContext*	GetRenderContext();

	void			SetRenderContext(RenderContext* pRenderContext);
}

#endif
