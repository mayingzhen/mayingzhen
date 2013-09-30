#ifndef __RenderThread_H__
#define __RenderThread_H__

#include "TArray.h"

namespace ma
{
	class ParticleEmitter;

	enum ERenderCommand
	{
		eRC_Unknown = 0,

		eRC_Init,
		eRC_TexStreamComplete,
		eRC_BeginFrame,
		eRC_EndFrame,
		eRC_Render,
		eRC_CreateShader,
		eRC_CreateRenderTarget,
 		eRC_ClearBuffer,
 		eRC_PushRenderTarget,
 		eRC_PushViewPort,
 		eRC_PopRenderTarget,
 		eRC_PopViewPort,
	};

	class ENGINE_API RenderThread : public Thread
	{
	public:
		bool			m_bQuit;
		bool			m_bSuccessful;

		int				m_nCurThreadProcess;
		int				m_nCurThreadFill;

		volatile int	m_nFlush;

		UINT			m_nRenderThread;
		UINT			m_nMainThread;
		HRESULT			m_hResult;
		TArray<byte>	m_Commands[2]; // m_nCurThreadFill shows which commands are filled by main thread

		RenderThread();

		~RenderThread();

		// flushCond
		void	SignalFlushFinishedCond();
		void	SignalFlushCond();
		void	WaitFlushCond();
		void	WaitFlushFinishedCond();
		void	InitFlushCond();
		bool	CheckFlushCond();
		void	SignalQuitCond();

		bool	IsFailed();

		// Command
		void	AddCommand(ERenderCommand eRC);
		void	AddDWORD(UINT nVal);
		void	AddDWORD64(Int64 nVal);
		void	AddInt(int nVal);
		void	AddBool(bool bVal);
		void	AddFloat(const float fVal);
		void	AddVec3(const Vector3& cVal);
		void	AddColor(const Color& cVal);
		void	AddPointer(const void *pVal);
		void	AddData(const void *pData, int nLen);

		template<class T>
		void	ReadData(int &nIndex,T& data);

		template<class T> 
		T		ReadCommand(int& nIndex);

		int		GetCurrentThreadId(bool bAlwaysCheck=false);

		void	Run();

		void	Init();
		void	Quit();
		void	FlushFrame();
		void	FlushAndWait();
		void	ProcessCommands();
		int		GetThreadList();
		bool	IsRenderThread(bool bAlwaysCheck=false);
		bool	IsMainThread(bool bAlwaysCheck=false);
		bool	IsMultithreaded();
		int		CurThreadFill() const;

		void	RC_Init();
		void	RC_BeginFrame();
		void	RC_EndFrame();
		void	RC_TexStreamComplete(Texture* pTexture,DataStream* pDataStream);
		void	RC_Render();
		void	RC_CreateShader(ShaderProgram* pShader);
		void	RC_CreateRenderTarget(RenderTarget* pRenderTarget);
		void	RC_PushRenderTarget(RenderTarget* pTexture);
		void	RC_PushViewPort(Rectangle& viewPort);
		void	RC_PopRenderTargert();
		void	RC_PopViewPort();
		void	RC_ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s);

	};


	ENGINE_API RenderThread* GetRenderThread();

	ENGINE_API void SetRenderThread(RenderThread* pRenderThread);

}

#include "RenderThread.inl"



#endif	// __RENDERTHREAD_H__

