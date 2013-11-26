
#ifndef  _RenderSystem__H__
#define  _RenderSystem__H__


#include "Render/RenderSystem/RenderThread.h"

namespace ma
{
	class Camera;
	class RenderThread;
	class RenderQueue;
	class RenderPass;
	class RenderContext;


	class RENDER_API  RenderSystem 
	{
		friend class RenderThread;

	public:
		RenderSystem();	

		~RenderSystem();

		// Camra
		Camera*				GetMainCamera();

		void				SetMainCamera(Camera* pCamera);
		
		void				DrawRenderable(Renderable* pRenderable,Technique* pTechnique);

		void				DrawDyRenderable(Renderable* pRenderable,Technique* pTechnique);

		// Render Command
		void				Init();

		void				Update();

		void				ShoutDown();

		void				BeginFrame();

		void				EndFrame();

		void				Render();

		Texture*			CreateRenderTarget(int nWidth = -1,int nHeight = -1,FORMAT format = FMT_A8R8G8B8,bool bDepthStencil = false);
		
		ShaderProgram*		CreateShaderProgram(Technique* pTech,const char* pVSFile, const char* pPSFile,const char* pszDefine);
		
		Texture*			SetRenderTarget(Texture* pTexture,int index = 0);

		Texture*			GetRenderTarget(int index = 0);

		Texture*			SetDepthStencil(Texture* pTexture);
		
		Rectangle			SetViewPort(const Rectangle& viewPort);
		
		void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s);
		
		void				TexStreamComplete(Texture* pTexture,DataStream* pDataStream);		

		void				OnFlushFrame();

		RenderQueue*		GetRenderQueue();

		int					CurThreadFill() const {return m_pRenderThread->CurThreadFill();}

		int					CurThreadProcess() const {return m_pRenderThread->CurThreadProcess();}

		int					GetThreadList() {return m_pRenderThread->GetThreadList();}

		void				FlushAndWait() {return m_pRenderThread->FlushAndWait();}

	protected: 
		void				RT_Init();

		void				RT_BeginFrame();

		void				RT_EndFrame();

		void				RT_Render();

		
	protected:
		RenderContext*				m_pRenderContext;

 		RenderThread*				m_pRenderThread;
 		
		RenderQueue*				m_pRenderQueue[2];

		Camera*						m_pMainCamera;

		Texture*					m_pDepthStencil;

		enum {MAX_RENDER_TARGET = 4};
		Texture*					m_pRenderTarget[MAX_RENDER_TARGET];
		
		Rectangle					m_viewport;
	};

	RENDER_API RenderSystem*		GetRenderSystem();

	RENDER_API void					SetRenderSystem(RenderSystem* pRenderSystem);
}

#endif
