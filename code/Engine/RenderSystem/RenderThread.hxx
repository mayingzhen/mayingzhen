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


	void RenderThread::RC_Init(void* wndhandle)
	{
		if (IsRenderThread())
		{
			return GetRenderSystem()->RT_Init(wndhandle);
		}

		AddCommand(eRC_Init);
		AddPointer(wndhandle);

		FlushAndWait();
	}

	void RenderThread::RC_Reset(uint32_t nWidth,uint32_t nHeight)
	{
		if (IsRenderThread())
		{
			return GetRenderSystem()->RT_Reset(nWidth,nHeight);
		}

		AddCommand(eRC_Reset);
		AddDWORD(nWidth);
		AddDWORD(nHeight);

		FlushAndWait();
	}

	void RenderThread::RC_ShutDown()
	{
		if (IsRenderThread())
		{
			return GetRenderSystem()->RT_ShutDown();
		}

		AddCommand(eRC_ShutDown);

		FlushAndWait();
	}

	void RenderThread::RC_BeginRender()
	{
		if (IsRenderThread())
		{
			GetRenderSystem()->RT_BeginRender();
			return;
		}

		AddCommand(eRC_BeginRender);
	}

	void RenderThread::RC_EndRender()
	{
		if (IsRenderThread())
		{
			GetRenderSystem()->RT_EndRender();
			return;
		}

		AddCommand(eRC_EndRender);
	}

	void RenderThread::RC_TechniqueStreamComplete(Technique* pTech)
	{
		ASSERT(pTech);

		if (IsRenderThread())
		{
			pTech->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_TechniqueStreamComplete);
		AddPointer(pTech);
	}

	void RenderThread::RC_TexStreamComplete(Texture* pTexture)
	{
		ASSERT(pTexture);

		if (IsRenderThread())
		{
			pTexture->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_TexStreamComplete);
		AddPointer(pTexture);
	}


	void RenderThread::RC_ShaderStreamComplete(ShaderProgram* pShader)
	{
		ASSERT(pShader);

		if (IsRenderThread())
		{
			pShader->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_ShaderStreamComplete);
		AddPointer(pShader);
	}

	void RenderThread::RC_VertexDeclaComplete(VertexDeclaration* pDecl)
	{
		ASSERT(pDecl);

		if (IsRenderThread())
		{
			pDecl->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_VertexDeclaComplete);
		AddPointer(pDecl);
	}

	void RenderThread::RC_HardwareBufferStreamComplete(HardwareBuffer* pHB)
	{
		ASSERT(pHB);

		if (IsRenderThread())
		{
			pHB->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_HBStreamComplete);
		AddPointer(pHB);
	}

	void RenderThread::RC_RenderPassStreamComplete(RenderPass* pRenderPass)
	{
		ASSERT(pRenderPass);

		if (IsRenderThread())
		{
			pRenderPass->Create();
			return;
		}

		AddCommand(eRC_RenderPassStreamComplete);
		AddPointer(pRenderPass);
	}

	void RenderThread::RC_Render()
	{
		if (IsRenderThread())
		{
			GetRenderSystem()->RT_Render();
			return;
		}

		AddCommand(eRC_Render);

		FlushFrame();
	}

	void RenderThread::RC_CreateShader(ShaderProgram* pShader)
	{
		if (IsRenderThread())
		{
			pShader->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_CreateShader);
		AddPointer(pShader);
	}

	void RenderThread::RC_CreateTexture(Texture* pRenderTarget)
	{
		if (IsRenderThread())
		{
			if (pRenderTarget->GetType() == TEXTYPE_CUBE)
			{
				pRenderTarget->RT_CreateCubeTexture();
			}
			else
			{
				pRenderTarget->RT_CreateTexture();
			}
			
			return;
		}

		AddCommand(eRC_CreateTexture);
		AddPointer(pRenderTarget);
	}

	void RenderThread::RC_SetUniformValue(Uniform* pUniform, const void* data, uint32_t nSize)
	{
		// job 线程也会调用到这边
		if (IsRenderThread() || !IsMainThread())
		{
			ASSERT(nSize <= pUniform->GetSize());
			pUniform->GetParent()->SetParameter(pUniform->GetOffset(), pUniform->GetSize(), data);
			return;
		}

		AddCommand(eRC_SetUniformValue);
		AddPointer(pUniform);
		AddData(data, nSize);
	}

	void RenderThread::RC_SetSampler(Uniform* pUniform, SamplerState* pSampler)
	{
		// job 线程也会调用到这边
		if (IsRenderThread() || !IsMainThread())
		{
			pUniform->GetTechnique()->RT_SetSampler(pUniform, pSampler);
			return;
		}

		AddCommand(eRC_SetSampler);
		AddPointer(pUniform);
		AddPointer(pSampler);
	}

	void RenderThread::RC_SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer)
	{
		// job 线程也会调用到这边
		if (IsRenderThread() || !IsMainThread())
		{
			pUniform->GetTechnique()->RT_SetStorageBuffer(pUniform, pBuffer);
			return;
		}

		AddCommand(eRC_SetStorageBuffer);
		AddPointer(pUniform);
		AddPointer(pBuffer);
	}

	void RenderThread::RC_SetPoolId(uint32_t poolId)
	{
		if (IsRenderThread())
		{
			GetRenderSystem()->RT_SetPoolId(poolId);
			return;
		}

		AddCommand(eRC_SetPoolId);
		AddDWORD(poolId);
	}

	void RenderThread::RC_BeginProfile(const char* pszLale)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->BeginProfile(pszLale);
			return;
		}

		AddCommand(eRC_BeginProfile);
		AddData(pszLale,strlen(pszLale));
	}

	void RenderThread::RC_EndProfile()
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->EndProfile();
			return;
		}
	
		AddCommand(eRC_EndProfile);

		//RC_AddRenderCommad( []() { GetRenderDevice()->EndProfile(); } );
	}

// 	void RenderThread::RC_AddRenderCommad(std::function<void()> fun)
// 	{
// 		RenderCommad comd;
// 		comd.m_funtion = fun;
// 		RC_AddRenderCommad(&comd);
// 	}
// 
// 	void RenderThread::RC_AddRenderCommad(RenderCommad* pCommad)
// 	{
// 		if (IsRenderThread())
// 		{
// 			pCommad->Do();
// 			return;
// 		}
// 
// 		m_vecCommand[m_nCurThreadFill].push_back(pCommad);
// 	}

	void RenderThread::ProcessCommands()
	{
		ASSERT(std::this_thread::get_id() == m_thread.get_id());

		if (!CheckFlushCond())
			return;

// 		for (uint32_t i = 0; i < m_vecCommand[m_nCurThreadProcess].size(); ++i)
// 		{
// 			RenderCommad* pCommmad = m_vecCommand[m_nCurThreadProcess][i].get();
// 			pCommmad->Do();
// 		}
// 		m_vecCommand[m_nCurThreadProcess].clear();

		int n = 0;
		while (n < (int)m_Commands[m_nCurThreadProcess].Num())
		{
			uint8_t nC = m_Commands[m_nCurThreadProcess][n++];
			switch (nC)
			{
			case eRC_Init:
				{
					void* wndhandle = ReadCommand<void*>(n);
					GetRenderSystem()->RT_Init(wndhandle);
				}
				break;
			case eRC_Reset:
				{
					uint32_t nWidth = ReadCommand<uint32_t>(n);
					uint32_t nHeight = ReadCommand<uint32_t>(n);
					GetRenderSystem()->RT_Reset(nWidth,nHeight);
				}
				break;
			case eRC_ShutDown:
				{
					GetRenderSystem()->RT_ShutDown();
				}
				break;
			case eRC_BeginRender:
				{
					GetRenderSystem()->RT_BeginRender();
				}
				break;
			case eRC_EndRender:
				{
					GetRenderSystem()->RT_EndRender();
				}
				break;
			case eRC_Render:
				{
					GetRenderSystem()->RT_Render();
				}
				break;
			case  eRC_TechniqueStreamComplete:
				{
					Technique* pTech = ReadCommand<Technique*>(n);
					pTech->RT_StreamComplete();
				}
				break;
			case  eRC_TexStreamComplete:
				{
					Texture* pTexture = ReadCommand<Texture*>(n);
					pTexture->RT_StreamComplete();
				}
				break;
			case  eRC_ShaderStreamComplete:
				{
					ShaderProgram* pShader = ReadCommand<ShaderProgram*>(n);
					pShader->RT_StreamComplete();
				}
				break;
			case  eRC_VertexDeclaComplete:
				{
					VertexDeclaration* pDecl = ReadCommand<VertexDeclaration*>(n);
					pDecl->RT_StreamComplete();
				}
				break;
			case  eRC_HBStreamComplete:
				{
					HardwareBuffer* pHB = ReadCommand<HardwareBuffer*>(n);
					pHB->RT_StreamComplete();
				}
				break;
			case eRC_RenderPassStreamComplete:
				{
					RenderPass* pRenderPass = ReadCommand<RenderPass*>(n);
					pRenderPass->Create();
				}
				break;
			case  eRC_CreateShader:
				{
					ShaderProgram* pShader = ReadCommand<ShaderProgram*>(n);
					pShader->RT_StreamComplete();
				}
				break;
			case  eRC_CreateTexture:
				{
					Texture* pTarget = ReadCommand<Texture*>(n);
					if (pTarget->GetType() == TEXTYPE_CUBE)
					{
						pTarget->RT_CreateCubeTexture();
					}
					else
					{
						pTarget->RT_CreateTexture();
					}
				}
				break;
			case eRC_SetUniformValue:
				{
					Uniform* pUniform = ReadCommand<Uniform*>(n);
					void* data = NULL;
					uint32_t nSize = 0;
					ReadData(n, data, nSize);

					ASSERT(nSize <= pUniform->GetSize());
					pUniform->GetParent()->SetParameter(pUniform->GetOffset(), pUniform->GetSize(), data);
				}
				break;
			case eRC_SetSampler:
				{
					Uniform* pUniform = ReadCommand<Uniform*>(n);
					SamplerState* pSampler = ReadCommand<SamplerState*>(n);
					pUniform->GetTechnique()->RT_SetSampler(pUniform, pSampler);
				}
				break;
			case eRC_SetStorageBuffer:
				{
					Uniform* pUniform = ReadCommand<Uniform*>(n);
					HardwareBuffer* pBuffer = ReadCommand<HardwareBuffer*>(n);
					pUniform->GetTechnique()->RT_SetStorageBuffer(pUniform, pBuffer);
				}
				break;
			case  eRC_SetPoolId:
				{
					uint32_t nId = ReadCommand<uint32_t>(n);
					GetRenderSystem()->RT_SetPoolId(nId);
				}
				break;
			case eRC_BeginProfile:
				{
					char pStr[256] = {0};
					ReadString(n,pStr,256);
					GetRenderDevice()->BeginProfile(pStr);
				}
				break;
			case eRC_EndProfile:
				{
					GetRenderDevice()->EndProfile();
				}	
				break;
			default:
				ASSERT(false);
				break;
			}
		}
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
		m_Commands[m_nCurThreadFill].SetUse(0);
	}


	// Flush current frame without waiting (should be called from main thread)
	void RenderThread::FlushFrame()
	{
		//FUNCTION_PROFILER_FAST( GetISystem(),PROFILE_RENDERER,g_bProfilerEnabled );

		//if (!IsMultithreaded())
		//	return;

		uint64_t nTime = GetTimer()->GetMillisceonds();
		WaitFlushFinishedCond();
		uint64_t nTimeWaitForRender = GetTimer()->GetMillisceonds() - nTime;
		//LogInfo("fTimeWaitForRender = %d",nTimeWaitForRender);
		//gRenDev->m_fTimeWaitForRender[m_nCurThreadFill] = iTimer->GetAsyncCurTime() - fTime;
		
		m_nCurThreadProcess = m_nCurThreadFill;
		m_nCurThreadFill = (m_nCurThreadProcess+1) & 1;

		m_Commands[m_nCurThreadFill].SetUse(0);
		//gRenDev->m_fTimeProcessedRT[m_nCurThreadFill] = 0;
		//gRenDev->m_fTimeWaitForMain[m_nCurThreadProcess] = 0;

		SignalFlushCond();
	}
}


