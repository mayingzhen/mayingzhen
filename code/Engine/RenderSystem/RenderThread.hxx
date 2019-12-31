#include "RenderThread.h"
#include "RenderCommand.h"
#include "RingBuffer.h"

namespace ma
{
#ifdef WIN32
	void* GetRenderWindowHandle()
	{
		return GetRenderSystem()->GetMainWnd();
	}
#endif
	
	RenderThread::RenderThread():
		m_render_command_buffer(1024 * 800, 16),
		m_frame_sema("frame-sema",1)
	{
		m_nCurThreadFill = 0;
		m_nCurThreadProcess = 0;
	
		
		m_nMainThread = std::this_thread::get_id();

		m_bMultithread = false;
	}

	RenderThread::~RenderThread()
	{

	}

	void RenderThread::Start()
	{
		m_thread = std::thread(&RenderThread::ThreadLoop, this);

		m_bMultithread = true;
		m_nCurThreadProcess = 1;
	}

	void RenderThread::Stop()
	{
		FlushAndWait();

		m_thread.join();
	}

	void RenderThread::ThreadLoop()
	{
		void* read_pointer = NULL;
		uint32_t num_read_bytes = 0;

		while (!m_bExit)
		{
			// Command processing loop.
			uint32_t count = 0;
			while (m_render_command_buffer.BeginRead(read_pointer, num_read_bytes))
			{
				// Process one render command.
				CommadInfo* Command = (CommadInfo*)read_pointer;

				ASSERT(Command != NULL);
				uint32_t command_size = Command->Execute();

				Command->~CommadInfo();
				m_render_command_buffer.FinishRead(command_size);
			}

			while (!m_bExit && !m_render_command_buffer.BeginRead(read_pointer, num_read_bytes))
			{
				// Wait for up to 16ms for rendering commands.
				m_render_command_buffer.WaitForRead(1);
			}
		}
	}


	void RenderThread::RC_Init(void* wndhandle,uint32_t nWidth, uint32_t nHeight)
	{
		RC_AddRenderCommad([wndhandle, nWidth, nHeight]() {
			GetRenderSystem()->RT_Init(wndhandle, nWidth, nHeight);
		});

		if (IsRenderThread())
			return;

		FlushAndWait();
	}

	void RenderThread::RC_Reset(uint32_t nWidth,uint32_t nHeight)
	{
		RC_AddRenderCommad([nWidth,nHeight]() {
			GetRenderSystem()->RT_Reset(nWidth, nHeight);
		});

		if (IsRenderThread())
			return;

		FlushAndWait();
	}

	void RenderThread::RC_ShutDown()
	{
		RC_AddRenderCommad([]() {
			GetRenderSystem()->RT_ShutDown();
		});

		if (IsRenderThread())
			return;

		FlushAndWait();
	}

	void RenderThread::RC_BeginRender()
	{
		RC_AddRenderCommad([]() {
			GetRenderSystem()->RT_BeginRender();
		});
	}

	void RenderThread::RC_EndRender()
	{
		RC_AddRenderCommad([]() {
			GetRenderSystem()->RT_EndRender();
		});

		RC_AddRenderCommad( [this]() {
			m_frame_sema.Signal();
		});
	}

	void RenderThread::RC_TechniqueStreamComplete(Technique* pTech)
	{
		ASSERT(pTech);

		RC_AddRenderCommad([pTech]() {
			pTech->RT_StreamComplete();
		});
	}

	void RenderThread::RC_TexStreamComplete(Texture* pTexture)
	{
		ASSERT(pTexture);

		RC_AddRenderCommad([pTexture]() {
			pTexture->RT_StreamComplete();
		});
	}


	void RenderThread::RC_ShaderStreamComplete(ShaderProgram* pShader)
	{
		ASSERT(pShader);

		RC_AddRenderCommad([pShader]() {
			pShader->RT_StreamComplete();
		});
	}

	void RenderThread::RC_VertexDeclaComplete(VertexDeclaration* pDecl)
	{
		ASSERT(pDecl);

		RC_AddRenderCommad([pDecl]() {
			pDecl->RT_StreamComplete();
		});
	}

	void RenderThread::RC_HardwareBufferStreamComplete(HardwareBuffer* pHB)
	{
		ASSERT(pHB);

		RC_AddRenderCommad([pHB]() {
			pHB->RT_StreamComplete();
		});
	}

	void RenderThread::RC_RenderPassStreamComplete(RenderPass* pRenderPass)
	{
		ASSERT(pRenderPass);

		RC_AddRenderCommad([pRenderPass]() {
			pRenderPass->Create();
		});
	}

	void RenderThread::RC_Render()
	{
		RC_AddRenderCommad([]() {
			GetRenderSystem()->RT_Render();
		});

		if (IsRenderThread())
			return;

		//FlushFrame();
	}

	void RenderThread::RC_CreateShader(ShaderProgram* pShader)
	{
		RC_AddRenderCommad( [pShader]() {
				pShader->RT_StreamComplete();
		});
	}

	void RenderThread::RC_CreateTexture(Texture* pRenderTarget)
	{
		RC_AddRenderCommad( 
			[pRenderTarget]() {
			if (pRenderTarget->GetType() == TEXTYPE_CUBE)
			{
				pRenderTarget->RT_CreateCubeTexture();
			}
			else
			{
				pRenderTarget->RT_CreateTexture();
			}
		});
	}

	void RenderThread::RC_SetUniformValue(Uniform* pUniform, const void* data, uint32_t nSize)
	{
		uint8_t* pCopyData = (uint8_t*)data;
		if (!IsRenderThread() && IsMainThread()) // job 线程也会调用到这边
		{
			pCopyData = AddData(data, nSize);
		}

		RC_AddRenderCommad( [pUniform,pCopyData,nSize]() {
			ASSERT(nSize <= pUniform->GetSize());
			pUniform->GetParent()->SetParameter(pUniform->GetOffset(), pUniform->GetSize(), pCopyData);
			} );
	}

	void RenderThread::RC_SetSampler(Uniform* pUniform, SamplerState* pSampler)
	{
		// job 线程也会调用到这边
		if (IsRenderThread() || !IsMainThread())
		{
			pUniform->GetTechnique()->RT_SetSampler(pUniform, pSampler);
			return;
		}

		RC_AddRenderCommad([pUniform, pSampler]() {
			pUniform->GetTechnique()->RT_SetSampler(pUniform, pSampler);
		});
	}

	void RenderThread::RC_SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer)
	{
		// job 线程也会调用到这边
		if (IsRenderThread() || !IsMainThread())
		{
			pUniform->GetTechnique()->RT_SetStorageBuffer(pUniform, pBuffer);
			return;
		}

		RC_AddRenderCommad([pUniform, pBuffer]() {
			pUniform->GetTechnique()->RT_SetStorageBuffer(pUniform, pBuffer);
		});
	}


	void RenderThread::RC_BeginProfile(const char* pszLale)
	{
		const char* pszParamLable = pszLale;
		if (!IsRenderThread())
		{
			pszParamLable = (const char*)AddData(pszLale, strlen(pszLale));
		}

		RC_AddRenderCommad( [pszParamLable]() { 
			GetRenderDevice()->BeginProfile(pszParamLable); 
		} );
	}

	void RenderThread::RC_EndProfile()
	{
		RC_AddRenderCommad( []() { 
			GetRenderDevice()->EndProfile(); 
		} );
	}

	bool RenderThread::RC_AddRenderCommad(const std::function<void()>& fun)
	{
		if (IsRenderThread())
		{
			fun();
			return true;
		}
		else
		{
			RingBuffer::AllocationContext AllocationContext(m_render_command_buffer,sizeof(CommadInfo));
			ASSERT(AllocationContext.GetAllocatedSize() >= sizeof(CommadInfo));
			if (AllocationContext.GetAllocatedSize() < sizeof(CommadInfo))
			{
				return false;
			}
		
			new(AllocationContext.GetAllocation()) CommadInfo(fun);
		
			return true;
		}
	}


	// Flush current frame and wait for result
	void RenderThread::FlushAndWait()
	{
 		if (!m_bMultithread)
 			return;
 
		RC_AddRenderCommad( [this]() {
			m_flush_event.Signal();
		});

		m_flush_event.Wait(-1);
		m_flush_event.Reset();
	}


	// Flush current frame without waiting (should be called from main thread)
	void RenderThread::FlushFrame()
	{
		if (!m_bMultithread)
			return;

		m_frame_sema.WaitForSignal();

		uint32_t nCurThreadFill = m_nCurThreadFill;
		RC_AddRenderCommad([this, nCurThreadFill]() {
			ASSERT((m_nCurThreadProcess + 1) % 2 == nCurThreadFill);
			m_nCurThreadProcess = nCurThreadFill;
		});

		m_nCurThreadFill = (m_nCurThreadFill + 1) % 2;

		m_CommandData[m_nCurThreadFill].SetUse(0);
	}
}


