#include "RenderThread.h"


namespace ma
{
	RenderThread::RenderThread()
	{
		m_nCurThreadFill = 0;
		m_nCurThreadProcess = 0;
		
		m_nFlush = 0; 
		
		m_nMainThread = GetCurrentThreadId();
		
		m_bSuccessful = true;

		m_bMultithread = false;
	}

	RenderThread::~RenderThread()
	{

	}

	void RenderThread::Start()
	{
		Thread::Start();

		m_bMultithread = true;
		m_nCurThreadProcess = 1;
	}

	void RenderThread::ThreadUpdate()
	{
		float fTime = GetTimer()->GetMillisceonds();
		WaitFlushCond();
		float fTimeAfterWait = GetTimer()->GetMillisceonds();
		float fTimeWaitForMain = fTimeAfterWait - fTime;
		//Log("fTimeWaitForMain = %f",fTimeWaitForMain);
		//gRenDev->m_fTimeWaitForMain[m_nCurThreadProcess] += fTimeAfterWait - fTime;
		ProcessCommands();
		float fTimeAfterProcess = GetTimer()->GetMillisceonds();
		float fTimeProcessedRT = fTimeAfterProcess - fTimeAfterWait;
		//Log("fTimeProcessedRT = %f",fTimeProcessedRT);
		//gRenDev->m_fTimeProcessedRT[m_nCurThreadFill] += fTimeAfterProcess - fTimeAfterWait;
	}


	void RenderThread::RC_Init(HWND wndhandle)
	{
		if (IsRenderThread())
		{
			return GetRenderSystem()->RT_Init(wndhandle);
		}

		AddCommand(eRC_Init);
		AddPointer(wndhandle);

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

	void RenderThread::RC_Render()
	{
		if (IsRenderThread())
		{
			GetRenderSystem()->RT_Render();
			return;
		}

		AddCommand(eRC_Render);
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

	void RenderThread::RC_CreateRenderTarget(Texture* pRenderTarget)
	{
		if (IsRenderThread())
		{
			pRenderTarget->CreateRT();
			return;
		}

		AddCommand(eRC_CreateRenderTarget);
		AddPointer(pRenderTarget);

		FlushAndWait();
	}

	void RenderThread::RC_SetRenderTarget(Texture* pTexture,int index)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetRenderTarget(pTexture,index);
			return;
		}

		AddCommand(eRC_SetRenderTarget);
		AddPointer(pTexture);
		AddInt(index);
	}

	void RenderThread::RC_SetDepthStencil(Texture* pTexture)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetDepthStencil(pTexture);
			return;
		}

		AddCommand(eRC_SetDepthStencil);
		AddPointer(pTexture);
	}

	void RenderThread::RC_SetViewPort(const Rectangle& viewPort)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetViewport(viewPort);
			return;
		}

		AddCommand(eRC_SetViewPort);
		AddData(&viewPort,sizeof(Rectangle));
	}

// 	void RenderThread::RC_PopRenderTargert(int index)
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->PopRenderTarget(index);
// 			return;
// 		}
// 
// 		AddCommand(eRC_PopRenderTarget);
// 		AddInt(index);
// 	}

// 	void RenderThread::RC_PopDepthStencil()
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->PopDepthStencil();
// 			return;
// 		}
// 
// 		AddCommand(eRC_PopDepthStencil);
// 	}

// 	void RenderThread::RC_PopViewPort()
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->PopViewport();
// 			return;
// 		}
// 
// 		AddCommand(eRC_PopViewPort);
// 	}

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
		ASSERT(GetCurrentThreadId() == GetThreadId());

		if (!CheckFlushCond())
			return;

		int n = 0;
		m_bSuccessful = true;
		m_hResult = S_OK;
		while (n < (int)m_Commands[m_nCurThreadProcess].Num())
		{
			byte nC = m_Commands[m_nCurThreadProcess][n++];
			switch (nC)
			{
			case eRC_Init:
				{
					HWND wndhandle = ReadCommand<HWND>(n);
					GetRenderSystem()->RT_Init(wndhandle);
				}
				break;
			case eRC_BeginFrame:
				GetRenderSystem()->RT_BeginFrame();
				break;
			case eRC_EndFrame:
				GetRenderSystem()->RT_EndFrame();
				break;
			case  eRC_Render:
				GetRenderSystem()->RT_Render();
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
					Texture* pTarget = ReadCommand<Texture*>(n);
					pTarget->CreateRT();
				}
				break;
			case eRC_SetRenderTarget:
				{
					Texture* pTarget = ReadCommand<Texture*>(n);
					int index = ReadCommand<int>(n);
					GetRenderDevice()->SetRenderTarget(pTarget,index);
				}
				break;
			case  eRC_SetDepthStencil:
				{
					Texture* pTarget = ReadCommand<Texture*>(n);
					GetRenderDevice()->SetDepthStencil(pTarget);
				}
				break;
// 			case  eRC_PopRenderTarget:
// 				{
// 					int index = ReadCommand<int>(n);
// 					GetRenderDevice()->PopRenderTarget(index);
// 				}
// 				break;
// 			case eRC_PopDepthStencil:
// 				GetRenderDevice()->PopDepthStencil();
// 				break;
			case  eRC_SetViewPort:
				{
					Rectangle viewPort;
					ReadData(n,viewPort);
					GetRenderDevice()->SetViewport(viewPort);
				}
				break;
// 			case  eRC_PopViewPort:
// 				{
// 					GetRenderDevice()->PopViewport();
// 				}
// 				break;
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

		float fTime = GetTimer()->GetMillisceonds();
		WaitFlushFinishedCond();
		float fTimeWaitForRender = GetTimer()->GetMillisceonds() - fTime;
		//Log("fTimeWaitForRender = %f",fTimeWaitForRender);
		//gRenDev->m_fTimeWaitForRender[m_nCurThreadFill] = iTimer->GetAsyncCurTime() - fTime;
		
		m_nCurThreadProcess = m_nCurThreadFill;
		m_nCurThreadFill = (m_nCurThreadProcess+1) & 1;

		m_Commands[m_nCurThreadFill].SetUse(0);
		//gRenDev->m_fTimeProcessedRT[m_nCurThreadFill] = 0;
		//gRenDev->m_fTimeWaitForMain[m_nCurThreadProcess] = 0;

		SignalFlushCond();
	}


	bool RenderThread::IsFailed()
	{
		return !m_bSuccessful;
	}
}


