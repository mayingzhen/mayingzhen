#ifndef __RenderThread_H__
#define __RenderThread_H__


namespace ma
{
	class Uniform;
	
	enum ERenderCommand
	{
		eRC_Unknown = 0,

		eRC_Init,
		eRC_Reset,
		eRC_ShutDown,
		eRC_TexStreamComplete,
		eRC_ShaderStreamComplete,
		eRC_VertexDeclaComplete,
		eRC_HBStreamComplete,
		eRC_BeginRender,
		eRC_EndRender,
		eRC_Render,
		eRC_DrawRenderable,
		eRC_DrawDyRenderable,
		eRC_CreateShader,
		eRC_CreateTexture,
		eRC_CreateDepthStencil,
 		eRC_ClearBuffer,
 		eRC_SetRenderTarget,
		eRC_SetDepthStencil,
 		eRC_SetViewPort,
		eRC_SetShader,

		// RenderState
		eRC_SetDepthCheckMode,
		eRC_SetDepthWrite,
		eRC_SetColorWrite,
		eRC_SetCullMode,
		eRC_SetDepthBias,
		eRC_SetBlendMode,
		eRC_SetSRGBWite,
		eRC_SetStenCilEnabled,
		eRC_SetStenCilParam,

		// Unfiform
		eRC_SetFloat,
		eRC_SetVector2,
		eRC_SetVector3,
		eRC_SetVector4,
		eRC_SetMatrix4,
		eRC_SetColourValue,
 		eRC_SetTexture,
		eRC_SetSamplerState,

		eRC_SetPoolId,

		eRC_BeginProfile,
		eRC_EndProfile,
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
		void	AddInt(int nVal);
		void	AddBool(bool bVal);
		void	AddFloat(float fVal);
		void	AddVec2(const Vector2& cVal);
		void	AddVec3(const Vector3& cVal);
		void	AddVec4(const Vector4& cVal);
		void	AddColor(const ColourValue& cVal);
		void	AddMatrix4(const Matrix4& cVal);
		void	AddPointer(const void *pVal);
		void	AddData(const void *pData, int nLen);

		template<class T>
		void	ReadData(int &nIndex,T& data);
		
		template<class T>
		T		ReadDataPtr(int &nIndex,int nLen);

		void	ReadString(int &nIndex,char* pStr,UINT nInLen);

		template<class T> 
		T		ReadCommand(int& nIndex);

		void	ThreadLoop();
		void	Start();
		void	Stop();

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
		void	RC_Reset(uint32 nWidth,uint32 nHeight);
		void	RC_ShutDown();
		void	RC_BeginRender();
		void	RC_EndRender();
		void	RC_TexStreamComplete(Texture* pTexture);
		void	RC_ShaderStreamComplete(ShaderProgram* pShader);
		void	RC_VertexDeclaComplete(VertexDeclaration* pDecl);
		void	RC_HardwareBufferStreamComplete(HardwareBuffer* pHB);
		void	RC_Render();
		void	RC_DrawRenderable(Renderable* pRenderable,Technique* pTechnique);
		void	RC_DrawDyRenderable(Renderable* pRenderable,Technique* pTechnique);
		void	RC_CreateShader(ShaderProgram* pShader);
		void	RC_CreateTexture(Texture* pRenderTarget);
		void	RC_CreateDepthStencil(Texture* pRenderTarget);
		void	RC_SetShaderProgram(ShaderProgram* pShader);
		void	RC_SetRenderTarget(Texture* pTexture,int index);
		void	RC_SetDepthStencil(Texture* pTexture);
		void	RC_SetViewPort(const Rectangle& viewPort);
		void	RC_ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s);
		
		void	RC_SetDepthCheckMode(CompareFunction eDepthCheckMode);
		void	RC_SetDepthWrite(bool b);
		void	RC_SetColorWrite(bool b);
		void	RC_SetSRGBWite(bool b);
		void	RC_SetCullMode(CULL_MODE eCullMode);
		void	RC_SetDepthBias(float fConstantBias,float slopeScaleBias);
		void	RC_SetBlendMode(BLEND_MODE eBlendMode);
		void	RC_SetStencilCheckEnabled(bool b);
		void	RC_SetStencilBufferParams(CompareFunction func, 
			uint32 refValue, uint32 mask, uint32 writeMask,
			StencilOperation stencilFailOp, 
			StencilOperation depthFailOp,
			StencilOperation passOp, 
			bool twoSidedOperatio);

		void	RC_SetFloat(Uniform* uniform, float value);
		void	RC_SetVector2(Uniform* uniform, const Vector2& value);
		void	RC_SetVector3(Uniform* uniform, const Vector3& value);
		void	RC_SetVector4(Uniform* uniform, const Vector4* values, UINT count);
		void	RC_SetMatrix4(Uniform* uniform, const Matrix4* values, UINT count);
		void	RC_SetColourValue(Uniform* uniform, const ColourValue& value);
 		void	RC_SetTexture(Uniform* uniform, const Texture* sampler);
		void	RC_SetSamplerState(Uniform* uniform, const SamplerState* sampler);

		void	RC_SetPoolId(uint32 poolId);

		void	RC_BeginProfile(const char* pszLale);
		void	RC_EndProfile();

	private:
		bool			m_bMultithread;

		bool			m_bSuccessful;

		int				m_nCurThreadProcess;
		int				m_nCurThreadFill;

		volatile int	m_nFlush;

		UINT			m_nMainThread;
		HRESULT			m_hResult;
		TArray<BYTE>	m_Commands[2]; // m_nCurThreadFill shows which commands are filled by main thread

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

	inline void RenderThread::AddVec2(const Vector2& vVal)
	{
		Byte* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(Vector2));
		memcpy(pDest,&vVal,sizeof(Vector2));
	}
    
	inline void RenderThread::AddVec3(const Vector3& vVal)
	{
        Byte* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(Vector3));
        memcpy(pDest,&vVal,sizeof(Vector3));
	}

	inline void RenderThread::AddVec4(const Vector4& cVal)
	{
		Byte* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(Vector4));
		memcpy(pDest,&cVal,sizeof(Vector4));
	}
    
	inline void RenderThread::AddColor(const ColourValue& cVal)
	{
		Byte* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(ColourValue));
        memcpy(pDest,&cVal,sizeof(ColourValue));  
	}

	inline void RenderThread::AddMatrix4(const Matrix4& cVal)
	{
		Byte* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(Matrix4));
		memcpy(pDest,&cVal,sizeof(Matrix4));  
	}
    
	inline void RenderThread::AddPointer(const void *pVal)
	{
		*(const void **)(m_Commands[m_nCurThreadFill].Grow(sizeof(void *))) = pVal;
	}
    
	inline void RenderThread::AddData(const void *pData, int nLen)
	{
		AddDWORD(nLen);
		BYTE *pDst = m_Commands[m_nCurThreadFill].Grow(nLen);
		memcpy(pDst, pData, nLen);
	}

	template<class T>
	T RenderThread::ReadDataPtr(int &nIndex,int nLen)
	{
		DWORD nReadLen = ReadCommand<DWORD>(nIndex);
		ASSERT(nLen == nReadLen);
		BYTE* pSrc = &m_Commands[m_nCurThreadProcess][nIndex]; 
		nIndex += nReadLen;
		return (T)pSrc;
	}
    
	template<class T>
	inline void	RenderThread::ReadData(int &nIndex,T& data)
	{
		DWORD nLen = ReadCommand<DWORD>(nIndex);
		ASSERT(nLen == sizeof(T));
		BYTE* pSrc = &m_Commands[m_nCurThreadProcess][nIndex]; 
		memcpy(&data,pSrc,nLen);
		nIndex += nLen;
	}

	inline void	RenderThread::ReadString(int &nIndex,char* pStr,UINT nInLen)
	{
		DWORD nLen = ReadCommand<DWORD>(nIndex);
		ASSERT(nLen <= nInLen);
		BYTE* pSrc = &m_Commands[m_nCurThreadProcess][nIndex]; 
		memcpy(pStr,pSrc,nLen);
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

	inline void RenderThread::WaitFlushCond() // wait maiThread
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

#ifdef WIN32
	HWND GetRenderWindowHandle();
#endif

	inline void RenderThread::WaitFlushFinishedCond() // wait RenderThread
	{
		while(*(volatile int*)&m_nFlush)
		{
#ifdef WIN32
//			Sleep(0);
			MSG msg;		
			while (PeekMessage(&msg, GetRenderWindowHandle(), 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
#else
			::usleep(0);
#endif
		}
	}

	inline bool RenderThread::CheckFlushCond()
	{
		return *(int*)&m_nFlush != 0;
	}

// 	inline int RenderThread::GetCurrentThreadId(bool bAlwaysCheck = false)
// 	{
// 		if (!bAlwaysCheck && GetThreadId() == m_nMainThread)
// 			return GetThreadId();
// 		return ::GetCurrentThreadId();
// 	}

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
}





#endif	// __RENDERTHREAD_H__

