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

// 		Renderable*	GetCurRenderable() {return m_pCurRenderable;}
// 
// 		Light*		GetCurLight()	{return m_pCurLight;}

		UINT		GetSolidEntryNumber() {return m_arrSolidEntry.size();}

		Renderable*	GetSolidEntryByIndex(UINT i) {return m_arrSolidEntry[i];}

		UINT		GetLightNumber() {return m_arrLight.size();}

		Light*		GetLightByIndex(UINT i) {return m_arrLight[i];}

		RenderMesh*	CreatRenderMesh(const char* pMeshPath,const char* pDiffueTexture);

		void		DeleteRenderMesh(RenderMesh* pRenderMesh);

	protected:
		std::vector<Renderable*>	m_arrSolidEntry;
		std::vector<Renderable*>	m_arrTransEntry;
		std::vector<Light*>			m_arrLight;
		//Renderable*					m_pCurRenderable;
		//Light*						m_pCurLight;

		Camera*						m_pCamera;

		std::vector<RenderMesh*>	m_arrRenderMesh;
		
		DeferredLight*				m_pDefferLight;
		Shadow*						m_pShadow;

		bool						m_bShadow;
		bool						m_bDefferLight;

	};

	ENGINE_API RenderSystem*	GetRenderSystem();

	ENGINE_API void				SetRenderSystem(RenderSystem* pRenderSystem);
}

#endif