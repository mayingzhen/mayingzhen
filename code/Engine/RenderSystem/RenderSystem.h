#pragma once

#include "RenderThread.h"
#include "Engine/RenderScheme/RenderScheme.h"
#include "Engine/RenderSystem/RenderQueue.h"

namespace ma
{
	class Camera;
	class RenderThread;
	class RenderPass;
	class MainRenderStep;
	class Scene;
	class Uniform;
	class Texture;
	class FrameBuffer;
	class BlendState;
	class RasterizerState;
	class DepthStencilState;
	class RenderView;


	class  RenderSystem 
	{
		friend class RenderThread;

	public:
		RenderSystem();	

		~RenderSystem();

		// Render Command
		void				Init(void* wndhandle, int width, int height, bool bThread);
		void				Reset(uint32_t nWidth, uint32_t nHeight);
		void				Shoutdown();

		void				Update();

		void				BegineRender();
		void				EndRender();
		void				Render();

		void				BegineCompute();
		void				EndCompute();
	
		Rectangle			GetViewPort() {return m_curViewport;}

		void				SetDefferedLightRenderPass(RenderPass* pRenderPass);
		RenderPass*			GetDefferedLightRenderPass();

		void				SetBaseRenderPass(RenderPass* pRenderPass);
		RenderPass*			GetBaseRenderPass();

		RenderPass*			GetBackBufferRenderPass();
		
		void*				GetMainWnd() {return m_hWnd;}
		
		// Create	
		RefPtr<Texture>		CreateRenderTarget(int nWidth = -1,int nHeight = -1,uint32_t nMipMap = 1, PixelFormat format = PF_A8R8G8B8,bool bSRGB = true,TEXTURE_TYPE eType = TEXTYPE_2D);
		RefPtr<Texture>		CreateDepthStencil(int nWidth = -1,int nHeight = -1,PixelFormat format = PF_D24S8);
		RefPtr<IndexBuffer>	CreateIndexBuffer(uint8_t* pData,uint32_t nSize,int nStride,HBU_USAGE eUsage = HBU_STATIC);
		RefPtr<VertexBuffer> CreateVertexBuffer(uint8_t* pData,uint32_t nSize,int nStride,HBU_USAGE eUsage = HBU_STATIC);
		RefPtr<VertexDeclaration> CreateVertexDeclaration(VertexElement* arrElememt,uint32_t nCount);

		inline void			RC_AddRenderCommad(const std::function<void()>& fun) {m_pRenderThread->RC_AddRenderCommad(fun);}

		void				TechniqueStreamComplete(Technique* pTech);
		void				TexStreamComplete(Texture* pTexture);		
		void				ShaderStreamComplete(ShaderProgram* pShader);
		void				VertexDeclaComplete(VertexDeclaration* pDec);
		void				HardwareBufferStreamComplete(HardwareBuffer* pHB);
		void				RenderPassStreamComplete(RenderPass* pRenderPass);

		void				UpdteHardwareBuffer(HardwareBuffer* pHB, const void* data, uint32_t nSize);

		void				SetUniformValue(Uniform* pUniform, const void* data, uint32_t nSize);
		void				SetSampler(Uniform* pUniform, SamplerState* pSampler);
		void				SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer);

		void				BeginProfile(const char* pszLale);
		void				EndProfile();
		
		void				OnFlushFrame();

		int					CurThreadFill() const {return m_pRenderThread->CurThreadFill();}
		int					CurThreadProcess() const {return m_pRenderThread->CurThreadProcess();}
		int					GetThreadList() {return m_pRenderThread->GetThreadList();}
		void				FlushAndWait() {return m_pRenderThread->FlushAndWait();}
		bool				IsRenderThread() { return m_pRenderThread->IsRenderThread(); }

		Scene*				GetScene();

		const char*			GetShaderPath();
		
		bool				AddShaderGlobaMacro(const char* pszKey, const char* pszValue);
		const char*			GetShaderGlobaMacro(const char* pszKey) const;
		uint32_t			GetNumShaderGlobaMacro() const;
		const char*			GetShaderGlobaMacroByIndex(uint32_t i, OUT const char*& pszValue) const;
		
		void				ReloadShader();

		ComputeCommand*		GetComputeCommand() { return m_pComputeCommd.get(); }

		void				AddRenderView(RenderView* pRenderView);

		//void				AddRenderStep(RefPtr<RenderStep> renderstep);

	protected: 
		void				RT_Init(void* wndhandle, int width, int height);
		void				RT_Reset(uint32_t nWidth,uint32_t nHeight);
		void				RT_ShutDown();
 		void				RT_BeginRender();
 		void				RT_EndRender();
		void				RT_Render();

		void				InitGlobeMarco();

		void				InitCachState();
		
	protected:
 		RenderThread*		m_pRenderThread;
 		
		Rectangle			m_curViewport;

		RefPtr<RenderPass>	m_pDefferedLightPass;

		RefPtr<RenderPass>	m_pBaseRenderPass;

		RefPtr<RenderPass>	m_pBackBufferRenderPass;

		RefPtr<ComputeCommand> m_pComputeCommd;

		void*				m_hWnd;

		RefPtr<Scene>		m_scene;
		
		bool				m_bDefferedRender = true;

		std::vector< RefPtr<RenderView> > m_renderView;

		//std::vector< RefPtr<RenderStep> > m_renderStepList;
				
		typedef map<string, string> MAP_STR_STR;
		MAP_STR_STR			m_mapMacros; // Shader globe Macro
		bool				m_bNeedReloadShader;

		bool				m_bThread;
	};

	extern RenderSystem* g_pRenderSystem;
	RenderSystem* GetRenderSystem();
}

