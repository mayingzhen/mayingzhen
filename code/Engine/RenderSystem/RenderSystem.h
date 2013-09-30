#ifndef  _RenderSystem__H__
#define  _RenderSystem__H__



namespace ma
{

	class ScreenQuad;
	class Camera;
	class DeferredLight;
	class Shadow;
	class RenderThread;
	class RenderQueue;

	class ENGINE_API  RenderSystem
	{
		friend RenderThread;

	public:
		RenderSystem();	

		void				Init();

		void				ShoutDown();

		void				AddLight(Light* pLight);
		
		void				BeginFrame();

		void				EndFrame();

		void				Render();

		void				OnFlushFrame();

		void				SetCamera(Camera* pCamera); 

		const Matrix4x4&	GetViewMatrix() const {return m_matView;}

		const Matrix4x4&	GetProjMatrix() const {return m_matProj;}

		Matrix4x4			GetViewProjMatrix() const {return GetViewMatrix() * GetProjMatrix();}

		float				GetNearClip() {return m_fNear;}

		float				GetFarClip() {return m_fFar;}

		UINT				GetLightNumber() {return m_arrLight.size();}

		Light*				GetLightByIndex(UINT i) {return m_arrLight[i];}

		const Vector4&		GetAmbientColor() {return m_cAmbientColor;}

		void				SetAmbientColor(const Vector4& cAmbientColor) {m_cAmbientColor = cAmbientColor;}

		RenderMesh*			CreatRenderMesh(const char* pMeshPath,const char* pDiffueTexture);

		void				DeleteRenderMesh(RenderMesh* pRenderMesh);
	
		void				SetDefferLight(bool bDefferLight) {m_bDefferLight = bDefferLight;}

		void				SetShadow(bool bShadow) {m_bShadow = bShadow;}

		void				SetClearClor(Color cClor) {m_cClearClor = cClor;}

		void				DrawRenderable(Renderable* pRenderable);

		void				DrawDyRenderable(Renderable* pRenderable);
	
	protected: // Rendrt Thread
		void				RT_Init();

		void				RT_BeginFrame();

		void				RT_EndFrame();

		void				RT_Render();

	protected:
		void				ShadingPass();

	protected:
		std::vector<Light*>			m_arrLight;

		//Camera*						m_pCamera;
		Matrix4x4					m_matView;
		Matrix4x4					m_matProj;
		float						m_fNear;
		float						m_fFar;

		std::vector<RenderMesh*>	m_arrRenderMesh;
		
		DeferredLight*				m_pDefferLight;
		Shadow*						m_pShadow;
		RenderThread*				m_pRenderThread;
		RenderQueue*				m_pRenderQueue;

		bool						m_bShadow;
		bool						m_bDefferLight;
		bool						m_bThread;

		Vector4						m_cAmbientColor;

		Color						m_cClearClor;

	};

	ENGINE_API RenderSystem*	GetRenderSystem();

	ENGINE_API void				SetRenderSystem(RenderSystem* pRenderSystem);
}

#endif