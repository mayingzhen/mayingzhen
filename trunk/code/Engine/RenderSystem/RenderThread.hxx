#include "RenderThread.h"


namespace ma
{

	RenderThread* gpRenderThead = NULL;

	RenderThread* GetRenderThread()
	{
		return gpRenderThead;
	}

	void SetRenderThread(RenderThread* pRenderThread)
	{
		gpRenderThead = pRenderThread;
	}

	RenderThread::RenderThread()
	{
	}

	void RenderThread::Init()
	{
		m_bQuit = false;

		m_nCurThreadFill = 0;
		m_nCurThreadProcess = 0;

		InitFlushCond();
	
		m_nMainThread = ::GetCurrentThreadId();
		m_bSuccessful = true;
		m_nRenderThread = GetThreadId();
		m_nCurThreadProcess = 1;

		m_Commands[0].Free();
		m_Commands[1].Free();
	}

	RenderThread::~RenderThread()
	{
		Quit();
	}


	void RenderThread::RC_Init()
	{
		if (IsRenderThread())
		{
			return GetRenderSystem()->RT_Init();
		}

		AddCommand(eRC_Init);

		FlushAndWait();
	}

	void RenderThread::RC_BeginFrame()
	{
		if (IsRenderThread())
		{
			GetRenderSystem()->RT_BeginFrame();
			return;
		}

		AddCommand(eRC_BeginFrame);
	}

	void RenderThread::RC_EndFrame()
	{
		if (IsRenderThread())
		{
			GetRenderSystem()->RT_EndFrame();
			return;
		}

		AddCommand(eRC_EndFrame);
		FlushFrame();
	}

	void RenderThread::RC_TexStreamComplete(Texture* pTexture,DataStream* pDataStream)
	{
		if (IsRenderThread())
		{
			pTexture->Load(pDataStream);
			return;
		}

		AddCommand(eRC_TexStreamComplete);
		AddPointer(pTexture);
		AddPointer(pDataStream);
	}

	void RenderThread::RC_Flush()
	{
		if (IsRenderThread())
		{
			GetRenderSystem()->RT_Flush();
			return;
		}

		AddCommand(eRC_Flush);
	}

	void RenderThread::RC_CreateShader(ShaderProgram* pShader)
	{
		if (IsRenderThread())
		{
			pShader->Create();
			return;
		}

		AddCommand(eRC_CreateShader);
		AddPointer(pShader);

		FlushAndWait();
	}

	void RenderThread::RC_CreateRenderTarget(RenderTarget* pRenderTarget)
	{
		if (IsRenderThread())
		{
			pRenderTarget->Create();
			return;
		}

		AddCommand(eRC_CreateRenderTarget);
		AddPointer(pRenderTarget);

		FlushAndWait();
	}

	void RenderThread::RC_PushRenderTarget(RenderTarget* pTexture)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->PushRenderTarget(pTexture);
			return;
		}

		AddCommand(eRC_PushRenderTarget);
		AddPointer(pTexture);
	}

	void RenderThread::RC_PushViewPort(Rectangle& viewPort)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->PushViewport(viewPort);
			return;
		}

		AddCommand(eRC_PushViewPort);
		AddData(&viewPort,sizeof(Rectangle));
	}

	void RenderThread::RC_PopRenderTargert()
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->PopRenderTarget();
			return;
		}

		AddCommand(eRC_PopRenderTarget);
	}

	void RenderThread::RC_PopViewPort()
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->PopViewport();
			return;
		}

		AddCommand(eRC_PopViewPort);
	}

	void RenderThread::RC_ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->ClearBuffer(bColor,bDepth,bStencil,c,z,s);
			return;
		}

		AddCommand(eRC_ClearBuffer);
		AddBool(bColor);
		AddBool(bDepth);
		AddBool(bStencil);
		AddColor(c);
		AddFloat(z);
		AddInt(s);
	}

	void RenderThread::ProcessCommands()
	{
		assert (GetCurrentThreadId() == m_nRenderThread);

		if (!CheckFlushCond())
			return;

		int n = 0;
		m_bSuccessful = true;
		m_hResult = S_OK;
		byte *pP;
		while (n < (int)m_Commands[m_nCurThreadProcess].Num())
		{
			byte nC = m_Commands[m_nCurThreadProcess][n++];
			switch (nC)
			{
			case eRC_Init:
				GetRenderSystem()->RT_Init();
				break;
			case eRC_BeginFrame:
				GetRenderSystem()->RT_BeginFrame();
				break;
			case eRC_EndFrame:
				GetRenderSystem()->RT_EndFrame();
				break;
			case  eRC_Flush:
				GetRenderSystem()->RT_Flush();
				break;
			case  eRC_TexStreamComplete:
				{
					Texture* pTexture = ReadCommand<Texture*>(n);
					DataStream* pDataStream = ReadCommand<DataStream*>(n);
					pTexture->Load(pDataStream);
				}
				break;
			case  eRC_CreateShader:
				{
					ShaderProgram* pShader = ReadCommand<ShaderProgram*>(n);
					pShader->Create();
				}
				break;
			case  eRC_CreateRenderTarget:
				{
					RenderTarget* pTarget = ReadCommand<RenderTarget*>(n);
					pTarget->Create();
				}
				break;
			case eRC_PushRenderTarget:
				{
					RenderTarget* pTarget = ReadCommand<RenderTarget*>(n);
					GetRenderDevice()->PushRenderTarget(pTarget);
				}
				break;
			case  eRC_PopRenderTarget:
				GetRenderDevice()->PopRenderTarget();
				break;
			case  eRC_PushViewPort:
				{
					Rectangle viewPort;
					ReadData(n,viewPort);
					GetRenderDevice()->PushViewport(viewPort);
				}
				break;
			case  eRC_PopViewPort:
				{
					GetRenderDevice()->PopViewport();
				}
				break;
			case  eRC_ClearBuffer:
				{
					bool bColor = ReadCommand<bool>(n);
					bool bDepth = ReadCommand<bool>(n);
					bool bStencil = ReadCommand<bool>(n);
					Color c = ReadCommand<Color>(n);
					float z = ReadCommand<float>(n);
					int s = ReadCommand<int>(n);
					GetRenderDevice()->ClearBuffer(bColor,bDepth,bStencil,c,z,s);
				}
				break;
			default:
				ASSERT(false);
				break;
			}
		}
		SignalFlushFinishedCond();
	}

	void RenderThread::Run()
	{
		while(true)
		{
			//float fTime = iTimer->GetAsyncCurTime();
			this->WaitFlushCond();
			if(this->m_bQuit)	
			{
				this->SignalFlushFinishedCond();
				break;//put it here to safely shut down
			}
			//float fTimeAfterWait = iTimer->GetAsyncCurTime();
			//gRenDev->m_fTimeWaitForMain[m_nCurThreadProcess] += fTimeAfterWait - fTime;
			this->ProcessCommands();
			//float fTimeAfterProcess = iTimer->GetAsyncCurTime();
			//gRenDev->m_fTimeProcessedRT[m_nCurThreadFill] += fTimeAfterProcess - fTimeAfterWait;
		}
	}


	// Flush current frame and wait for result
	void RenderThread::FlushAndWait()
	{
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

		if (!IsMultithreaded())
			return;

		//	gRenDev->FlushMainThreadAuxGeomCB();
		//float fTime = iTimer->GetAsyncCurTime();
		WaitFlushFinishedCond();
		//gRenDev->m_fTimeWaitForRender[m_nCurThreadFill] = iTimer->GetAsyncCurTime() - fTime;
		
		m_nCurThreadProcess = m_nCurThreadFill;
		m_nCurThreadFill = (m_nCurThreadProcess+1) & 1;

		m_Commands[m_nCurThreadFill].SetUse(0);
		//gRenDev->m_fTimeProcessedRT[m_nCurThreadFill] = 0;
		//gRenDev->m_fTimeWaitForMain[m_nCurThreadProcess] = 0;

		GetRenderSystem()->OnFlushFrame();

		SignalFlushCond();
	}

	void RenderThread::Quit()
	{
		if (IsMultithreaded())
		{
			SignalQuitCond();
			//while (m_pThread->IsRunning()) 
			//{
			//}
			//m_pThread->WaitForThread();
		}
		m_bQuit = 1;
	}

	bool RenderThread::IsFailed()
	{
		return !m_bSuccessful;
	}
}


