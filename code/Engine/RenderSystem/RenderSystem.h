#ifndef  _RenderSystem__H__
#define  _RenderSystem__H__



namespace ma
{

	class ScreenQuad;
	class Camera;
	class DeferredLight;
	class Shadow;

	class ENGINE_API  RenderSystem
	{
	public:
		RenderSystem();	

		void		Init();

		void		ShoutDown();

		void		AddRenderable(Renderable* pRenderable,bool bTrans = false);

		void		AddLight(Light* pLight);
		
		void		BeginFrame();

		void		EndFrame();

		void		Flush();

		void		SetCamera(Camera* pCamera) {m_pCamera = pCamera;}

		Camera*		GetCamera() {return m_pCamera;}

		UINT		GetSolidEntryNumber() {return m_arrSolidEntry.size();}

		Renderable*	GetSolidEntryByIndex(UINT i) {return m_arrSolidEntry[i];}

		UINT		GetLightNumber() {return m_arrLight.size();}

		Light*		GetLightByIndex(UINT i) {return m_arrLight[i];}

		Vector4		GetAmbientColor() {return m_cAmbientColor;}

		void		SetAmbientColor(const Vector4& cAmbientColor) {m_cAmbientColor = cAmbientColor;}

		RenderMesh*	CreatRenderMesh(const char* pMeshPath,const char* pDiffueTexture);

		void		DeleteRenderMesh(RenderMesh* pRenderMesh);

		void		SetDefferLight(bool bDefferLight) {m_bDefferLight = bDefferLight;}

		void		SetShadow(bool bShadow) {m_bShadow = bShadow;}

		void		SetClearClor(Color cClor) {m_cClearClor = cClor;}

	protected:
		std::vector<Renderable*>	m_arrSolidEntry;
		std::vector<Renderable*>	m_arrTransEntry;
		std::vector<Light*>			m_arrLight;

		Camera*						m_pCamera;

		std::vector<RenderMesh*>	m_arrRenderMesh;
		
		DeferredLight*				m_pDefferLight;
		Shadow*						m_pShadow;

		bool						m_bShadow;
		bool						m_bDefferLight;

		Vector4						m_cAmbientColor;

		Color						m_cClearClor;

	};

	ENGINE_API RenderSystem*	GetRenderSystem();

	ENGINE_API void				SetRenderSystem(RenderSystem* pRenderSystem);
}

#endif