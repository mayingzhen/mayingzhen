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

		// Camera
		void				SetCamera(Camera* pCamera); 
		const Matrix4x4&	GetViewMatrix() const {return m_matView;}
		const Matrix4x4&	GetProjMatrix() const {return m_matProj;}
		Matrix4x4			GetViewProjMatrix() const {return GetViewMatrix() * GetProjMatrix();}
		float				GetNearClip() {return m_fNear;}
		float				GetFarClip() {return m_fFar;}

		// Light
		void				AddLight(Light* pLight);
		UINT				GetLightNumber() {return m_arrLight.size();}
		Light*				GetLightByIndex(UINT i) {return m_arrLight[i];}
		const Vector4&		GetAmbientColor() {return m_cAmbientColor;}
		void				SetAmbientColor(const Vector4& cAmbientColor) {m_cAmbientColor = cAmbientColor;}
		void				SetDefferLight(bool bDefferLight) {m_bDefferLight = bDefferLight;}
		bool				IsDefferLight() {return m_bDefferLight;}

		// Shadow
		void				SetShadow(bool bShadow) {m_bShadow = bShadow;}
		bool				IsShadow() {return m_bShadow;}
		
		void				SetClearClor(Color cClor) {m_cClearClor = cClor;}

		void				DrawRenderable(Renderable* pRenderable,Technique* pTechnique);
		void				DrawDyRenderable(Renderable* pRenderable,Technique* pTechnique);

		// Render Command
		void				Init();
		void				ShoutDown();
		void				BeginFrame();
		void				EndFrame();
		void				Render();
		Texture*			CreateRenderTarget(int nWidth = -1,int nHeight = -1,FORMAT format = FMT_A8R8G8B8,bool bDepthStencil = false);
		ShaderProgram*		CreateShaderProgram(Technique* pTech,const char* pszName,const char* pszDefine);
		void				PushRenderTarget(Texture* pTexture);
		void				PushDepthStencil(Texture* pTexture);
		void				PushViewPort(Rectangle& viewPort);
		void				PopRenderTargert();
		void				PopDepthStencil();
		void				PopViewPort();
		void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s);
		void				TexStreamComplete(Texture* pTexture,DataStream* pDataStream);		

		void				OnFlushFrame();
	
	protected: // Rendrt Thread
		void				RT_Init();
		void				RT_BeginFrame();
		void				RT_EndFrame();
		void				RT_Render();

	protected:
		void				ShadingPass();

		

	protected:
		std::vector<Light*>			m_arrLight;
		Vector4						m_cAmbientColor;

		// Camera
		Matrix4x4					m_matView;
		Matrix4x4					m_matProj;
		float						m_fNear;
		float						m_fFar;

		DeferredLight*				m_pDefferLight;
		Shadow*						m_pShadow;
		RenderThread*				m_pRenderThread;
		RenderQueue*				m_pRenderQueue;

		bool						m_bShadow;
		bool						m_bDefferLight;
		bool						m_bThread;

		Color						m_cClearClor;

	};

	ENGINE_API RenderSystem*	GetRenderSystem();

	ENGINE_API void				SetRenderSystem(RenderSystem* pRenderSystem);


	struct ENGINE_API RenderProfile
	{
		RenderProfile(const char * str)
		{
			GetRenderDevice()->BeginProfile(str);
		}

		~RenderProfile()
		{
			GetRenderDevice()->EndProfile();
		}
	};


	#define RENDER_PROFILE(name) RenderProfile __re_##name(#name)

}

#endif