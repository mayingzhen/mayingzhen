#ifndef __RenderThread_H__
#define __RenderThread_H__


namespace ma
{

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
 		eRC_SetRenderTarget,
		eRC_SetDepthStencil,
 		eRC_SetViewPort,
	};

	class RenderThread : public Thread
	{
	public:
		RenderThread();

		~RenderThread();

		// flushCond
		void	SignalFlushFinishedCond();
		void	SignalFlushCond();
		void	WaitFlushCond();
		void	WaitFlushFinishedCond();
		bool	CheckFlushCond();

		bool	IsFailed();

		// Command
		void	AddCommand(ERenderCommand eRC);
		void	AddDWORD(UINT nVal);
		void	AddDWORD64(Int64 nVal);
		void	AddInt(int nVal);
		void	AddBool(bool bVal);
		void	AddFloat(float fVal);
		void	AddVec3(const Vector3& cVal);
		void	AddColor(const Color& cVal);
		void	AddPointer(const void *pVal);
		void	AddData(const void *pData, int nLen);

		template<class T>
		void	ReadData(int &nIndex,T& data);

		template<class T> 
		T		ReadCommand(int& nIndex);

		void	ThreadUpdate();

		void	Start();

		void	FlushFrame();
		void	FlushAndWait();
		void	ProcessCommands();
		int		GetThreadList();
		bool	IsRenderThread();
		bool	IsMainThread();
		//bool	IsMultithreaded();
		int		CurThreadFill() const;
		int		CurThreadProcess() const;

		void	RC_Init(HWND wndhandle);
		void	RC_BeginFrame();
		void	RC_EndFrame();
		void	RC_TexStreamComplete(Texture* pTexture,DataStream* pDataStream);
		void	RC_Render();
		void	RC_CreateShader(ShaderProgram* pShader);
		void	RC_CreateRenderTarget(Texture* pRenderTarget);
		void	RC_SetRenderTarget(Texture* pTexture,int index);
		void	RC_SetDepthStencil(Texture* pTexture);
		void	RC_SetViewPort(const Rectangle& viewPort);
		void	RC_ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s);


	private:
		bool			m_bMultithread;

		bool			m_bSuccessful;

		int				m_nCurThreadProcess;
		int				m_nCurThreadFill;

		volatile int	m_nFlush;

		UINT			m_nMainThread;
		HRESULT			m_hResult;
		TArray<byte>	m_Commands[2]; // m_nCurThreadFill shows which commands are filled by main thread

	};
    
    
    inline void RenderThread::AddCommand(ERenderCommand eRC)
	{
		m_Commands[m_nCurThreadFill].AddElem(eRC);
	}
    
	inline void RenderThread::AddDWORD(UINT nVal)
	{
        Byte* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(UINT));
        memcpy(pDest,&nVal,sizeof(UINT));
	}
    
	inline void RenderThread::AddDWORD64(Int64 nVal)
	{
		*(Int64 *)(m_Commands[m_nCurThreadFill].Grow(sizeof(Int64))) = nVal;
	}
    
	inline void RenderThread::AddInt(int nVal)
	{
		*(int *)(m_Commands[m_nCurThreadFill].Grow(sizeof(int))) = nVal;
	}
    
	inline void RenderThread::AddBool(bool bVal)
	{
		*(bool *)(m_Commands[m_nCurThreadFill].Grow(sizeof(bool))) = bVal;
	}
    
    
	inline void RenderThread::AddFloat(float fVal)
	{
        Byte* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(float));
        memcpy(pDest,&fVal,sizeof(float)); // ARM 下不能使用指针强转赋值
	}
    
	inline void RenderThread::AddVec3(const Vector3& vVal)
	{
        Byte* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(Vector3));
        memcpy(pDest,&vVal,sizeof(Vector3));
	}
    
	inline void RenderThread::AddColor(const Color& cVal)
	{
		Byte* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(Color));
        memcpy(pDest,&cVal,sizeof(Color));  
	}
    
	inline void RenderThread::AddPointer(const void *pVal)
	{
		*(const void **)(m_Commands[m_nCurThreadFill].Grow(sizeof(void *))) = pVal;
	}
    
	inline void RenderThread::AddData(const void *pData, int nLen)
	{
		AddDWORD(nLen);
		byte *pDst = m_Commands[m_nCurThreadFill].Grow(nLen);
		memcpy(pDst, pData, nLen);
	}
    
	template<class T>
	inline void	RenderThread::ReadData(int &nIndex,T& data)
	{
		DWORD nLen = ReadCommand<DWORD>(nIndex);
		ASSERT(nLen == sizeof(T));
		byte* pSrc = &m_Commands[m_nCurThreadProcess][nIndex]; 
		memcpy(&data,pSrc,nLen);
		nIndex += nLen;
	}
    
	template<class T>
	inline T RenderThread::ReadCommand(int& nIndex)
	{
		T Res = *(T*)&m_Commands[m_nCurThreadProcess][nIndex];
		nIndex += sizeof(T);
		return Res;
	}
    
    
	inline void RenderThread::SignalFlushFinishedCond()
	{
		m_nFlush = 0;
	}

	inline void RenderThread::SignalFlushCond()
	{
		m_nFlush = 1;
	}

	inline void RenderThread::WaitFlushCond()
	{
		while (!*(volatile int*)&m_nFlush)
		{
			if (m_bExit)
				break;
#ifdef WIN32
			::Sleep(0);
#endif
		}
	}

	inline void RenderThread::WaitFlushFinishedCond()
	{
		while(*(volatile int*)&m_nFlush)
		{
#ifdef WIN32
			Sleep(0);
// 			MSG msg;		
// 			while (PeekMessage(&msg, GetRenderSystem()->GetMainWnd(), 0, 0, PM_REMOVE))
// 			{
// 				TranslateMessage(&msg);
// 				DispatchMessage(&msg);
// 			}
#else
			::usleep(0);
#endif
		}
	}

	inline bool RenderThread::CheckFlushCond()
	{
		return *(int*)&m_nFlush != 0;
	}

	inline int RenderThread::GetThreadList()
	{
		return GetCurrentThreadId() == GetThreadId() ? m_nCurThreadProcess : m_nCurThreadFill;
	}

	inline bool RenderThread::IsRenderThread()
	{
		return !m_bMultithread || GetCurrentThreadId() == GetThreadId();
	}

	inline bool RenderThread::IsMainThread()
	{
		return !m_bMultithread || GetCurrentThreadId() == m_nMainThread;
	}

	inline int RenderThread::CurThreadFill() const
	{
		return m_nCurThreadFill;
	}

	inline int RenderThread::CurThreadProcess() const
	{
		return m_nCurThreadProcess;
	}

	//RenderThread* GetRenderThread();

	//void SetRenderThread(RenderThread* pRenderThread);

}

//#include "RenderThread.inl"



#endif	// __RENDERTHREAD_H__

