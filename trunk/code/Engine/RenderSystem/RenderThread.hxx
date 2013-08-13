#include "RenderThread.h"





namespace ma
{




	//void RenderThread::Run()
	//{
		//CryThreadSetName(-1, RENDER_THREAD_NAME);
		//gEnv->pSystem->GetIThreadTaskManager()->MarkThisThreadForDebugging(RENDER_THREAD_NAME,true);





		//UINT renderThreadId = ::GetCurrentThreadId();
		//gRenDev->m_pRT->m_nRenderThread = renderThreadId;
		//gEnv->pCryPak->SetRenderThreadId( renderThreadId );

		//gRenDev->m_pRT->m_nCurThreadProcess = 1;

		//m_started.Set();
		//gRenDev->m_pRT->Process();
		//gEnv->pSystem->GetIThreadTaskManager()->MarkThisThreadForDebugging(RENDER_THREAD_NAME,false);
	//}

	RenderThread::RenderThread()
	{
		Init(2);
	}

	void RenderThread::Init(int nCPU)
	{
		m_bQuit = false;

		m_nCurThreadFill = 0;
		m_nCurThreadProcess = 0;

		InitFlushCond();
		m_nRenderThread = ::GetCurrentThreadId();
		m_nMainThread = m_nRenderThread;
		m_bSuccessful = true;
		//m_pThread = new Thread(Process,this);
		m_Commands[0].Free();
		m_Commands[1].Free();

		//RenderQueue::m_nProcessThreadID = m_nCurThreadProcess;
		//RenderQueue:::m_nFillThreadID = m_nCurThreadFill;
		//m_nProcessThreadID = m_nCurThreadProcess;
		//gRenDev->m_RP.m_nFillThreadID = m_nCurThreadFill;
// 		gRenDev->m_fTimeWaitForMain[0] = 0;
// 		gRenDev->m_fTimeWaitForMain[1] = 0;
// 		gRenDev->m_fTimeWaitForRender[0] = 0;
// 		gRenDev->m_fTimeWaitForRender[1] = 0;
// 		gRenDev->m_fTimeProcessedRT[0] = 0;
// 		gRenDev->m_fTimeProcessedRT[1] = 0;
	}

	RenderThread::~RenderThread()
	{
		Quit();
	}


	void RenderThread::RC_Init(HWND hWnd)
	{
		if (IsRenderThread())
		{
			return GetRenderDevice()->Init(hWnd);
		}
		AddCommand(eRC_Init);
		AddDWORD((DWORD)hWnd);

		FlushAndWait();
	}

	void RenderThread::RC_BeginFrame()
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->BeginRender();
			return;
		}

		AddCommand(eRC_BeginFrame);
	}

	void RenderThread::RC_EndFrame()
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->EndRender();
			return;
		}

		AddCommand(eRC_EndFrame);
		FlushFrame();
	}

	void RenderThread::RC_DrawRenderable(Renderable* pRenderable)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->DrawRenderable(pRenderable);
			return;
		}

		AddCommand(eRC_DrawRenderable);

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
				{
					HWND hWnd = (HWND)ReadCommand<DWORD>(n);
					GetRenderDevice()->Init(hWnd);
				}
				break;
			case eRC_BeginFrame:
				GetRenderDevice()->BeginRender();
				break;
			case eRC_EndFrame:
				GetRenderDevice()->EndRender();
				break;

			default:
				ASSERT(false);
				break;
			}
		}
		SignalFlushFinishedCond();
	}

	void RenderThread::Process()
	{
		while(true)
		{
			//float fTime = iTimer->GetAsyncCurTime();
			WaitFlushCond();
			if(m_bQuit)	
			{
				SignalFlushFinishedCond();
				break;//put it here to safely shut down
			}
			//float fTimeAfterWait = iTimer->GetAsyncCurTime();
			//gRenDev->m_fTimeWaitForMain[m_nCurThreadProcess] += fTimeAfterWait - fTime;
			ProcessCommands();
			//float fTimeAfterProcess = iTimer->GetAsyncCurTime();
			//gRenDev->m_fTimeProcessedRT[m_nCurThreadFill] += fTimeAfterProcess - fTimeAfterWait;
		}
	}


	// Flush current frame and wait for result
	void RenderThread::FlushAndWait()
	{
		if (!m_pThread)
			return;

		WaitFlushFinishedCond();

		int nCurProcess = m_nCurThreadProcess;
		m_nCurThreadProcess = m_nCurThreadFill;
		//gRenDev->m_RP.m_nProcessThreadID = m_nCurThreadProcess;
		SignalFlushCond();

		WaitFlushFinishedCond();

		m_nCurThreadProcess = nCurProcess;
		//gRenDev->m_RP.m_nProcessThreadID = m_nCurThreadProcess;
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
		//	gRenDev->ToggleMainThreadAuxGeomCB();
		//gRenDev->m_RP.m_TI[m_nCurThreadProcess].m_nFrameUpdateID = gRenDev->m_RP.m_TI[m_nCurThreadFill].m_nFrameUpdateID;
		//gRenDev->m_RP.m_TI[m_nCurThreadProcess].m_nFrameID = gRenDev->m_RP.m_TI[m_nCurThreadFill].m_nFrameID;
		m_nCurThreadProcess = m_nCurThreadFill;
		m_nCurThreadFill = (m_nCurThreadProcess+1) & 1;
		//gRenDev->m_RP.m_nProcessThreadID = m_nCurThreadProcess;
		//gRenDev->m_RP.m_nFillThreadID = m_nCurThreadFill;
		m_Commands[m_nCurThreadFill].SetUse(0);
		//gRenDev->m_fTimeProcessedRT[m_nCurThreadFill] = 0;
		//gRenDev->m_fTimeWaitForMain[m_nCurThreadProcess] = 0;

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
			SAFE_DELETE(m_pThread);
		}
		m_bQuit = 1;
	}

	bool RenderThread::IsFailed()
	{
		return !m_bSuccessful;
	}
}


