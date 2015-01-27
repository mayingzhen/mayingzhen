#ifndef  _RenderSystem__H__
#define  _RenderSystem__H__

#include "RenderThread.h"
#include "Engine/Material/RenderState.h"
#include "Engine/Material/SamplerState.h"
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


	class RENDER_API  RenderSystem 
	{
		friend class RenderThread;

	public:
		RenderSystem();	

		~RenderSystem();

		void				Update();

		// Render Command
		void				Init(HWND wndhandle,bool bThread);
		void				Shoutdown();
 		void				BeginFrame();
 		void				EndFrame();
		void				Render();
		
		void				DrawRenderable(Renderable* pRenderable,Technique* pTechnique);
		void				DrawDyRenderable(Renderable* pRenderable,Technique* pTechnique);
		
		RefPtr<Texture>		SetRenderTarget(RefPtr<Texture>	 pTexture,int index = 0);
		RefPtr<Texture>		GetRenderTarget(int index = 0);
		RefPtr<Texture>		SetDepthStencil(RefPtr<Texture>	 pTexture);	
		Rectangle			SetViewPort(const Rectangle& viewPort);
		Rectangle			GetViewPort() {return m_viewport;}
		
		void				SetShaderProgram(ShaderProgram* pShader);
		
		// RenderState
		void				SetRenderState(const RenderState& state);
		void				SetBlendMode(BLEND_MODE eBlendMode);
		void				SetDepthCheckMode(DEPTH_CHECK_MODE eDepthCheckMode);
		void				SetDepthWirte(bool b);
		void				SetColorWrite(bool b);
		void				SetCullMode(CULL_MODE eCullMode);
		void				SetDepthBias(float fConstantBias);

		void				SetValue(Uniform* uniform, float value);
		void				SetValue(Uniform* uniform, const Vector2& value);
		void				SetValue(Uniform* uniform, const Vector3& value);
		void				SetValue(Uniform* uniform, const Vector4& value);
		void				SetValue(Uniform* uniform, const Matrix4& value);
		void				SetValue(Uniform* uniform, const Matrix4* values, UINT count);
		void				SetValue(Uniform* uniform, const Vector4* values, UINT count);
		void				SetValue(Uniform* uniform, const ColourValue& value);
		void				SetValue(Uniform* uniform, SamplerState* sampler);
		void				SetValue(Uniform* uniform, Texture* sampler);
		
		HWND				GetMainWnd() {return m_hWnd;}
		
		void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s);
		
		// Create
		Texture*			CreateRenderTexture(int nWidth = -1,int nHeight = -1,PixelFormat format = PF_A8R8G8B8,USAGE use = USAGE_RENDERTARGET);
		Texture*			CreateDepthStencil(int nWidth,int nHeight,PixelFormat format = PF_D24S8);
		RefPtr<IndexBuffer>	CreateIndexBuffer(void* pData,UINT nSize,int nStride,USAGE eUsage = USAGE_STATIC);
		RefPtr<VertexBuffer> CreateVertexBuffer(void* pData,UINT nSize,int nStride,USAGE eUsage = USAGE_STATIC);
		RefPtr<VertexDeclaration> CreateVertexDeclaration(VertexElement* arrElememt,uint32 nCount);

		void				TexStreamComplete(Texture* pTexture);		
		void				ShaderStreamComplete(ShaderProgram* pShader);
		void				VertexDeclaComplete(VertexDeclaration* pDec);
		void				HardwareBufferStreamComplete(HardwareBuffer* pHB);

		void				BeginProfile(const char* pszLale);
		void				EndProfile();
		
		void				OnFlushFrame();

		int					CurThreadFill() const {return m_pRenderThread->CurThreadFill();}
		int					CurThreadProcess() const {return m_pRenderThread->CurThreadProcess();}
		int					GetThreadList() {return m_pRenderThread->GetThreadList();}
		void				FlushAndWait() {return m_pRenderThread->FlushAndWait();}

		Scene*				GetScene(int index = 0);

		const char*			GetShaderPath();
		
		bool				AddMacro(const char* pszKey, const char* pszValue);
		const char*			GetMacro(const char* pszKey) const;
		uint32				GetNumMacros() const;
		const char*			GetMacroByIndex(uint32 i, OUT const char*& pszValue) const;
		
		void				ReloadShader();

		ColourValue			GetClearColor() const { return m_cClearClor;}
		void				SetClearColor(const ColourValue& cColor) { m_cClearClor = cColor;}

	protected: 
		void				RT_Init(HWND wndhandle);
		void				RT_ShutDown();
 		void				RT_BeginFrame();
 		void				RT_EndFrame();
		void				RT_Render();
		void				RT_DrawRenderable(Renderable* pRenderable,Technique* pTechnique);
		void				RT_DrawDyRenderable(Renderable* pRenderable,Technique* pTechnique);

		void				InitGlobeMarco();

		void				InitCachState();
		
	protected:
		RenderContext*		m_pRenderContext;

 		RenderThread*		m_pRenderThread;
 		
		enum {MAX_RENDER_TARGET = 4};
		enum {MAX_SAMPSTATE = 16};
		RefPtr<Texture>		m_pDepthStencil;
		RefPtr<Texture>		m_pRenderTarget[MAX_RENDER_TARGET];
		Rectangle			m_viewport;
		ShaderProgram*		m_pCurShader;
		RenderState			m_curState;
		SamplerState		m_arrSampState[MAX_SAMPSTATE];
		VertexDeclaration*	m_pCurVertexDecla;
		VertexBuffer*		m_pCurVB;	
		IndexBuffer*		m_pCurIB;

		HWND				m_hWnd;

		typedef std::vector< RefPtr<Scene> > VEC_SCENE;
		VEC_SCENE			m_arrScene;
				
		typedef map<string, string> MAP_STR_STR;
		MAP_STR_STR			m_mapMacros; // Shader globe Macro
		bool				m_bNeedReloadShader;

		bool				m_bThread;
		ColourValue			m_cClearClor;

	};

	RENDER_API RenderSystem*	GetRenderSystem();

	RENDER_API void				SetRenderSystem(RenderSystem* pRenderSystem);
}

#endif

