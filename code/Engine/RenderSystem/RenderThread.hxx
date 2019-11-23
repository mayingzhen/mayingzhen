#include "RenderThread.h"
#include "RenderCommand.h"

namespace ma
{
#ifdef WIN32
	void* GetRenderWindowHandle()
	{
		return GetRenderSystem()->GetMainWnd();
	}
#endif
	
	RenderThread::RenderThread()
	{
		m_nCurThreadFill = 0;
		m_nCurThreadProcess = 0;
		
		m_nFlush = 0; 
		
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
		SignalFlushFinishedCond();

		m_thread.join();
	}

	void RenderThread::ThreadLoop()
	{
		while (!m_bExit)
		{
			uint64_t nTime = GetTimer()->GetMillisceonds();
			WaitFlushCond();
			uint64_t nTimeAfterWait = GetTimer()->GetMillisceonds();
			uint64_t nTimeWaitForMain = nTimeAfterWait - nTime;
			//LogInfo("fTimeWaitForMain = %d",nTimeWaitForMain);
			//gRenDev->m_fTimeWaitForMain[m_nCurThreadProcess] += fTimeAfterWait - fTime;
			ProcessCommands();
			uint64_t nTimeAfterProcess = GetTimer()->GetMillisceonds();
			uint64_t nTimeProcessedRT = nTimeAfterProcess - nTimeAfterWait;
			LogInfo("fTimeProcessedRT = %d",nTimeProcessedRT);
			//gRenDev->m_fTimeProcessedRT[m_nCurThreadFill] += fTimeAfterProcess - fTimeAfterWait;
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

		FlushFrame();
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

		FlushFrame();
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
			CommadInfo comd;
			comd.m_funtion = fun;

			m_vecCommand[m_nCurThreadFill].emplace_back(comd);

			return true;
		}
	}

	void RenderThread::ProcessCommands()
	{
		ASSERT(std::this_thread::get_id() == m_thread.get_id());

		if (!CheckFlushCond())
			return;

		for (uint32_t i = 0; i < m_vecCommand[m_nCurThreadProcess].size(); ++i)
		{
			CommadInfo& pCommmad = m_vecCommand[m_nCurThreadProcess][i];
			pCommmad.m_funtion();
		}
		m_vecCommand[m_nCurThreadProcess].clear();

		SignalFlushFinishedCond();
	}


	// Flush current frame and wait for result
	void RenderThread::FlushAndWait()
	{
		if (!m_bMultithread)
			return;

		WaitFlushFinishedCond();

		int nCurProcess = m_nCurThreadProcess;
		m_nCurThreadProcess = m_nCurThreadFill;

		SignalFlushCond();

		WaitFlushFinishedCond();

		m_nCurThreadProcess = nCurProcess;
		m_CommandData[m_nCurThreadFill].SetUse(0);
	}


	// Flush current frame without waiting (should be called from main thread)
	void RenderThread::FlushFrame()
	{
		//FUNCTION_PROFILER_FAST( GetISystem(),PROFILE_RENDERER,g_bProfilerEnabled );

		if (!m_bMultithread)
			return;

		uint64_t nTime = GetTimer()->GetMillisceonds();
		WaitFlushFinishedCond();
		uint64_t nTimeWaitForRender = GetTimer()->GetMillisceonds() - nTime;
		//LogInfo("fTimeWaitForRender = %d",nTimeWaitForRender);
		//gRenDev->m_fTimeWaitForRender[m_nCurThreadFill] = iTimer->GetAsyncCurTime() - fTime;
		
		m_nCurThreadProcess = m_nCurThreadFill;
		m_nCurThreadFill = (m_nCurThreadProcess+1) & 1;

		m_CommandData[m_nCurThreadFill].SetUse(0);
		//gRenDev->m_fTimeProcessedRT[m_nCurThreadFill] = 0;
		//gRenDev->m_fTimeWaitForMain[m_nCurThreadProcess] = 0;

		SignalFlushCond();
	}
}


