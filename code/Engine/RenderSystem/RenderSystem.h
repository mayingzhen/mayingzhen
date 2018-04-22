#ifndef  _RenderSystem__H__
#define  _RenderSystem__H__

#include "RenderThread.h"
#include "Engine/RenderScheme/RenderScheme.h"

namespace ma
{
	class Camera;
	class RenderThread;
	class RenderQueue;
	class RenderPass;
	class RenderContext;
	class RenderScheme;
	class Scene;
	class Uniform;
	class Texture;
	class FrameBuffer;
	class BlendState;
	class RasterizerState;
	class DepthStencilState;


	class  RenderSystem 
	{
		friend class RenderThread;

	public:
		RenderSystem();	

		~RenderSystem();

		void				Update();

		// Render Command
		void				Init(HWND wndhandle,bool bThread);
		void				Reset(uint32 nWidth, uint32 nHeight);
		void				Shoutdown();
		void				BegineRender();
		void				EndRender();
		void				Render();
		
		RefPtr<Texture>		GetRenderTarget(int index = 0);
		Rectangle			GetViewPort() {return m_curViewport;}
		RenderPass*			GetDefaultRenderPass();
		
		HWND				GetMainWnd() {return m_hWnd;}
		
		// Create	
		RefPtr<Texture>		CreateRenderTarget(int nWidth = -1,int nHeight = -1,UINT32 nMipMap = 1, PixelFormat format = PF_A8R8G8B8,bool bSRGB = true,TEXTURE_TYPE eType = TEXTYPE_2D);
		RefPtr<Texture>		CreateDepthStencil(int nWidth = -1,int nHeight = -1,PixelFormat format = PF_D24S8,bool bTypeLess = false);
		RefPtr<IndexBuffer>	CreateIndexBuffer(uint8* pData,UINT nSize,int nStride,HBU_USAGE eUsage = HBU_STATIC,bool bShadowData = false);
		RefPtr<VertexBuffer> CreateVertexBuffer(uint8* pData,UINT nSize,int nStride,HBU_USAGE eUsage = HBU_STATIC,bool bShadowData = false);
		RefPtr<VertexDeclaration> CreateVertexDeclaration(VertexElement* arrElememt,uint32 nCount);

		void				TechniqueStreamComplete(Technique* pTech);
		void				TexStreamComplete(Texture* pTexture);		
		void				ShaderStreamComplete(ShaderProgram* pShader);
		void				VertexDeclaComplete(VertexDeclaration* pDec);
		void				HardwareBufferStreamComplete(HardwareBuffer* pHB);
		void				RenderPassStreamComplete(RenderPass* pRenderPass);

		void				SetUniformValue(Uniform* pUniform, const void* data, UINT nSize);
		void				SetSampler(Uniform* pUniform, SamplerState* pSampler);

		void				BeginProfile(const char* pszLale);
		void				EndProfile();
		
		void				OnFlushFrame();

		int					CurThreadFill() const {return m_pRenderThread->CurThreadFill();}
		int					CurThreadProcess() const {return m_pRenderThread->CurThreadProcess();}
		int					GetThreadList() {return m_pRenderThread->GetThreadList();}
		void				FlushAndWait() {return m_pRenderThread->FlushAndWait();}

		Scene*				GetScene(int index = 0);

		const char*			GetShaderPath();
		
		bool				AddShaderGlobaMacro(const char* pszKey, const char* pszValue);
		const char*			GetShaderGlobaMacro(const char* pszKey) const;
		uint32				GetNumShaderGlobaMacro() const;
		const char*			GetShaderGlobaMacroByIndex(uint32 i, OUT const char*& pszValue) const;

		
		void				ReloadShader();

		ColourValue			GetClearColor() const { return m_cClearClor;}
		void				SetClearColor(const ColourValue& cColor) { m_cClearClor = cColor;}
		
		enum { nNumParticleBuffer = 3 };
		void				LockParticleVideoMemory(uint32 nId);
		void				UnLockParticleVideoMemory(uint32 nId);
		ParallHardWareBuffer*GetParticleBuffer();
		ParallHardWareBuffer*GetRTParticleBuffer();

		ParallHardWareBuffer*GetInstanceBuffer();
		ParallHardWareBuffer*GetRTInstaneBuffer();

		
		void				UpdatePoolId();
		int					GetPoolId();
		int					GetPooIdRT();	

	protected: 
		void				RT_Init(HWND wndhandle);
		void				RT_Reset(uint32 nWidth,uint32 nHeight);
		void				RT_ShutDown();
 		void				RT_BeginRender();
 		void				RT_EndRender();
		void				RT_Render();
		void				RT_SetPoolId(uint32 poolId);

		void				InitGlobeMarco();

		void				InitCachState();

		void				InitParticleVideoMemory();
		
	protected:
		RenderContext*		m_pRenderContext;

 		RenderThread*		m_pRenderThread;
 		
		enum {MAX_RENDER_TARGET = 4};
		Texture*			m_pDepthStencil;
		Texture*			m_pRenderTarget[MAX_RENDER_TARGET];
		Rectangle			m_curViewport;
		ShaderProgram*		m_pCurShader;
		SamplerState*		m_arrSampState[MAX_TEXTURE_UNITS];
		VertexDeclaration*	m_pCurVertexDecla;
		VertexBuffer*		m_pCurVB;	
		IndexBuffer*		m_pCurIB;

		HWND				m_hWnd;

		typedef std::vector< RefPtr<Scene> > VEC_SCENE;
		VEC_SCENE			m_arrScene;
				
		typedef map<string, string> MAP_STR_STR;
		MAP_STR_STR			m_mapMacros; // Shader globe Macro
		bool				m_bNeedReloadShader;

		RefPtr<ParallHardWareBuffer> m_pParticleBuffer[nNumParticleBuffer];

		RefPtr<ParallHardWareBuffer> m_pInstanceBuffer[nNumParticleBuffer];

		// these ids can be used for tripple (or more) buffered structures
		// they are incremented in RenderWorld on the mainthread
		// use m_nPoolIndex from the mainthread (or jobs which are synced before Renderworld)
		// and m_nPoolIndexRT from the renderthread
		// right now the particle are using this id
		uint32				m_nPoolIndex;
		uint32				m_nPoolIndexRT;

		bool				m_bThread;
		ColourValue			m_cClearClor;

	};

	extern RenderSystem* g_pRenderSystem;
	RenderSystem* GetRenderSystem();
}

#endif

