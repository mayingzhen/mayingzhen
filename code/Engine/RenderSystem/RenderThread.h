#pragma once

#ifdef WIN32
#else
#include <unistd.h>
#endif

#include "RingBuffer.h"
#include "Engine/Thread/Event.h"

namespace ma
{
	class Uniform;
	class RenderPass;
	


	struct CommadInfo 
	{
		CommadInfo(std::function<void()> fun)
		{
			m_funtion = fun;
		}

		void* operator new(size_t Size, void* pAllocation)
		{
			return pAllocation;
		}

		void operator delete(void *p, void* pAllocation) {}

		void operator delete(void *p) {}

		virtual ~CommadInfo() {}

		virtual uint32_t Execute() 
		{
			m_funtion(); 
			return sizeof(*this);
		}

	public:

		std::function<void()> m_funtion;
	};


	class RenderThread 
	{
	public:
		RenderThread();

		~RenderThread();


		void	ThreadLoop();
		void	Start();
		void	Stop();

		void	UpdateRenderIndex();
		void	SyncRenderFrame();
		void	FlushAndWait();
		int		GetThreadList() const;
		bool	IsRenderThread() const;
		bool	IsMainThread() const;
		int		CurThreadFill() const;
		int		CurThreadProcess() const;

		void	RC_Init(void* wndhandle,uint32_t nWidth, uint32_t nHeight);
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

		void	RC_UpdateHardwareBuffer(HardwareBuffer* pHB, const void* data, uint32_t nSize);

		void	RC_CreateTexture(Texture* pRenderTarget);
		
		void	RC_SetUniformValue(Uniform* pUniform, const void* data, uint32_t nSize);
		void	RC_SetSampler(Uniform* pUniform, SamplerState* pSampler);
		void	RC_SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer);


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

		std::thread::id	m_nMainThread;
		
		std::thread		m_thread;

		bool			m_bExit = false;

		TArray<uint8_t>	m_CommandData[2]; // m_nCurThreadFill shows which commands are filled by main thread

		RingBuffer		m_render_command_buffer;

		Semaphore*		m_frame_sema[2];

		Event			m_flush_event;
	};
    
	inline uint8_t * RenderThread::AddData(const void *pData, int nLen)
	{
		uint8_t *pDst = m_CommandData[m_nCurThreadFill].Grow(nLen);
		memcpy(pDst, pData, nLen);
		return pDst;
	}

	inline int RenderThread::GetThreadList() const
	{
		return std::this_thread::get_id() == m_thread.get_id() ? m_nCurThreadProcess : m_nCurThreadFill;
	}

	inline bool RenderThread::IsRenderThread() const
	{
		return !m_bMultithread || std::this_thread::get_id()== m_thread.get_id();
	}

	inline bool RenderThread::IsMainThread() const
	{
		return !m_bMultithread || std::this_thread::get_id() == m_nMainThread;
	}

	inline int RenderThread::CurThreadFill() const
	{
		ASSERT(!m_bMultithread || !IsRenderThread());
		return m_nCurThreadFill;
	}

	inline int RenderThread::CurThreadProcess() const
	{
		ASSERT(!m_bMultithread || IsRenderThread());
		return m_nCurThreadProcess;
	}

}

