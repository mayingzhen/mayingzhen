#pragma once

#ifdef WIN32
#else
#include <unistd.h>
#endif

namespace ma
{
	class Uniform;
	class RenderPass;
	
	enum ERenderCommand
	{
		eRC_Unknown = 0,

		eRC_Init,
		eRC_Reset,
		eRC_ShutDown,

		eRC_TechniqueStreamComplete,
		eRC_TexStreamComplete,
		eRC_ShaderStreamComplete,
		eRC_VertexDeclaComplete,
		eRC_HBStreamComplete,
		eRC_RenderPassStreamComplete,

		eRC_BeginRender,
		eRC_EndRender,
		eRC_Render,
		eRC_CreateShader,
		eRC_CreateTexture,

		eRC_SetUniformValue,
		eRC_SetSampler,
		eRC_SetStorageBuffer,
		
		eRC_SetPoolId,

		eRC_BeginProfile,
		eRC_EndProfile,
	};

	struct RenderCommad : public Referenced
	{
		std::function<void()> m_funtion;

		virtual void Do() { m_funtion(); }
	};


	class RenderThread 
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

		// Command
		void	AddCommand(ERenderCommand eRC);
		void	AddDWORD(uint32_t nVal);
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
		void	ReadData(int &nIndex,T& data, uint32_t& nSize);
		
		template<class T>
		T		ReadDataPtr(int &nIndex,int nLen);

		void	ReadString(int &nIndex,char* pStr, uint32_t nInLen);

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
		int		CurThreadFill() const;
		int		CurThreadProcess() const;

		void	RC_Init(void* wndhandle);
		void	RC_Reset(uint32_t nWidth,uint32_t nHeight);
		void	RC_ShutDown();
		void	RC_BeginRender();
		void	RC_EndRender();

		void	RC_TechniqueStreamComplete(Technique* pTech);
		void	RC_TexStreamComplete(Texture* pTexture);
		void	RC_ShaderStreamComplete(ShaderProgram* pShader);
		void	RC_VertexDeclaComplete(VertexDeclaration* pDecl);
		void	RC_HardwareBufferStreamComplete(HardwareBuffer* pHB);
		void	RC_RenderPassStreamComplete(RenderPass* pRenderPass);
		void	RC_Render();
		void	RC_CreateShader(ShaderProgram* pShader);
		void	RC_CreateTexture(Texture* pRenderTarget);
		
		void	RC_SetUniformValue(Uniform* pUniform, const void* data, uint32_t nSize);
		void	RC_SetSampler(Uniform* pUniform, SamplerState* pSampler);
		void	RC_SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer);

		void	RC_SetPoolId(uint32_t poolId);

		void	RC_BeginProfile(const char* pszLale);
		void	RC_EndProfile();

		//void    RC_AddRenderCommad(std::function<void()> fun);
		//void    RC_AddRenderCommad(RenderCommad* pCommad);

	private:
		bool			m_bMultithread;

		bool			m_bSuccessful;

		int				m_nCurThreadProcess;
		int				m_nCurThreadFill;

		volatile int	m_nFlush;

		std::thread::id	m_nMainThread;
		TArray<uint8_t>	m_Commands[2]; // m_nCurThreadFill shows which commands are filled by main thread

		std::thread		m_thread;

		bool			m_bExit = false;

		//std::vector< RefPtr<RenderCommad> > m_vecCommand[2];
	};
    
    
    inline void RenderThread::AddCommand(ERenderCommand eRC)
	{
		m_Commands[m_nCurThreadFill].AddElem(eRC);
	}
    
	inline void RenderThread::AddDWORD(uint32_t nVal)
	{
		uint8_t* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(uint32_t));
        memcpy(pDest,&nVal,sizeof(uint32_t));
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
        uint8_t* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(float));
        memcpy(pDest,&fVal,sizeof(float)); // ARM 下不能使用指针强转赋值
	}

	inline void RenderThread::AddVec2(const Vector2& vVal)
	{
		uint8_t* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(Vector2));
		memcpy(pDest,&vVal,sizeof(Vector2));
	}
    
	inline void RenderThread::AddVec3(const Vector3& vVal)
	{
        uint8_t* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(Vector3));
        memcpy(pDest,&vVal,sizeof(Vector3));
	}

	inline void RenderThread::AddVec4(const Vector4& cVal)
	{
		uint8_t* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(Vector4));
		memcpy(pDest,&cVal,sizeof(Vector4));
	}
    
	inline void RenderThread::AddColor(const ColourValue& cVal)
	{
		uint8_t* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(ColourValue));
        memcpy(pDest,&cVal,sizeof(ColourValue));  
	}

	inline void RenderThread::AddMatrix4(const Matrix4& cVal)
	{
		uint8_t* pDest = m_Commands[m_nCurThreadFill].Grow(sizeof(Matrix4));
		memcpy(pDest,&cVal,sizeof(Matrix4));  
	}
    
	inline void RenderThread::AddPointer(const void *pVal)
	{
		*(const void **)(m_Commands[m_nCurThreadFill].Grow(sizeof(void *))) = pVal;
	}
    
	inline void RenderThread::AddData(const void *pData, int nLen)
	{
		AddDWORD(nLen);
		uint8_t *pDst = m_Commands[m_nCurThreadFill].Grow(nLen);
		memcpy(pDst, pData, nLen);
	}

	template<class T>
	T RenderThread::ReadDataPtr(int &nIndex,int nLen)
	{
		uint32_t nReadLen = ReadCommand<uint32_t>(nIndex);
		ASSERT(nLen == nReadLen);
		uint8_t* pSrc = &m_Commands[m_nCurThreadProcess][nIndex]; 
		nIndex += nReadLen;
		return (T)pSrc;
	}
    
	template<class T>
	inline void	RenderThread::ReadData(int &nIndex,T& data,uint32_t& nSize)
	{
		uint32_t nLen = ReadCommand<uint32_t>(nIndex);
		//ASSERT(nLen == sizeof(T));
		uint8_t* pSrc = &m_Commands[m_nCurThreadProcess][nIndex]; 
		//memcpy(&data,pSrc,nLen);
		data = (T)pSrc;
		nSize = nLen;
		nIndex += nLen;
	}

	inline void	RenderThread::ReadString(int &nIndex,char* pStr,uint32_t nInLen)
	{
		uint32_t nLen = ReadCommand<uint32_t>(nIndex);
		ASSERT(nLen <= nInLen);
		uint8_t* pSrc = &m_Commands[m_nCurThreadProcess][nIndex]; 
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

			std::this_thread::yield();
		}
	}

	inline void RenderThread::WaitFlushFinishedCond() // wait RenderThread
	{
		while(*(volatile int*)&m_nFlush)
		{
			std::this_thread::yield();
		}
	}

	inline bool RenderThread::CheckFlushCond()
	{
		return *(int*)&m_nFlush != 0;
	}

	inline int RenderThread::GetThreadList()
	{
		return std::this_thread::get_id() == m_thread.get_id() ? m_nCurThreadProcess : m_nCurThreadFill;
	}

	inline bool RenderThread::IsRenderThread()
	{
		return !m_bMultithread || std::this_thread::get_id()== m_thread.get_id();
	}

	inline bool RenderThread::IsMainThread()
	{
		return !m_bMultithread || std::this_thread::get_id() == m_nMainThread;
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

