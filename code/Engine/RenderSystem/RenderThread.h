#pragma once

#ifdef WIN32
#else
#include <unistd.h>
#endif

namespace ma
{
	class Uniform;
	class RenderPass;
	


	struct CommadInfo 
	{
		std::function<void()> m_funtion;
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

		bool    RC_AddRenderCommad(const std::function<void()>& fun);

	private:
		uint8_t*	AddData(const void *pData, int nLen);

	private:
		bool			m_bMultithread;

		bool			m_bSuccessful;

		int				m_nCurThreadProcess;
		int				m_nCurThreadFill;

		volatile int	m_nFlush;

		std::thread::id	m_nMainThread;
		
		std::thread		m_thread;

		bool			m_bExit = false;

		std::vector< CommadInfo > m_vecCommand[2];

		TArray<uint8_t>	m_CommandData[2]; // m_nCurThreadFill shows which commands are filled by main thread
	};
    
	inline uint8_t * RenderThread::AddData(const void *pData, int nLen)
	{
		uint8_t *pDst = m_CommandData[m_nCurThreadFill].Grow(nLen);
		memcpy(pDst, pData, nLen);
		return pDst;
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

