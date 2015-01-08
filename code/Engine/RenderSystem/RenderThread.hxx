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

	void RenderThread::Stop()
	{
		m_bExit = true;
		FlushAndWait();

		Thread::Stop();
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

	void RenderThread::RC_ShutDown()
	{
		if (IsRenderThread())
		{
			return GetRenderSystem()->RT_ShutDown();
		}

		AddCommand(eRC_ShutDown);

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

	void RenderThread::RC_TexStreamComplete(Texture* pTexture)
	{
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
		if (IsRenderThread())
		{
			pShader->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_ShaderStreamComplete);
		AddPointer(pShader);
	
		FlushAndWait();
	}

	void RenderThread::RC_VertexDeclaComplete(VertexDeclaration* pDecl)
	{
		if (IsRenderThread())
		{
			pDecl->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_VertexDeclaComplete);
		AddPointer(pDecl);

		FlushAndWait();
	}

	void RenderThread::RC_HardwareBufferStreamComplete(HardwareBuffer* pHB)
	{
		if (IsRenderThread())
		{
			pHB->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_HBStreamComplete);
		AddPointer(pHB);

		FlushAndWait();
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
			pShader->RT_StreamComplete();
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
			pRenderTarget->RT_Create();
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

	void RenderThread::RC_ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s)
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

	void RenderThread::RC_SetDepthCheckMode(DEPTH_CHECK_MODE eDepthCheckMode)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetDepthCheckMode(eDepthCheckMode);
			return;
		}

		AddCommand(eRC_SetDepthCheckMode);
		AddInt(eDepthCheckMode);
	}

	void RenderThread::RC_SetDepthWrite(bool b)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetDepthWrite(b);
			return;
		}

		AddCommand(eRC_SetDepthWrite);
		AddBool(b);
	}
	
	void RenderThread::RC_SetColorWrite(bool b)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetColorWrite(b);
			return;
		}

		AddCommand(eRC_SetColorWrite);
		AddBool(b);
	}

	void RenderThread::RC_SetCullMode(CULL_MODE eCullMode)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetCullingMode(eCullMode);
			return;
		}

		AddCommand(eRC_SetCullMode);
		AddInt(eCullMode);
	}

	void RenderThread::RC_SetDepthBias(float fConstantBias)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetDepthBias(fConstantBias);
			return;
		}

		AddCommand(eRC_SetDepthBias);
		AddFloat(fConstantBias);
	}

	void RenderThread::RC_SetBlendMode(BLEND_MODE eBlendMode)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetBlendMode(eBlendMode);
			return;
		}

		AddCommand(eRC_SetBlendMode);
		AddInt(eBlendMode);
	}

	void RenderThread::RC_SetFloat(Uniform* uniform, float value)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetValue(uniform,value);
			return;
		}

		AddCommand(eRC_SetFloat);
		AddPointer(uniform);
		AddFloat(value);
	}

	void RenderThread::RC_SetMatrix4(Uniform* uniform, const Matrix4& value)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetValue(uniform,value);
			return;
		}

		AddCommand(eRC_SetMatrix4);
		AddPointer(uniform);
		AddMatrix4(value);
	}

	void RenderThread::RC_SetVector2(Uniform* uniform, const Vector2& value)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetValue(uniform,value);
			return;
		}

		AddCommand(eRC_SetVector2);
		AddPointer(uniform);
		AddVec2(value);
	}

	void RenderThread::RC_SetVector3(Uniform* uniform, const Vector3& value)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetValue(uniform,value);
			return;
		}

		AddCommand(eRC_SetVector3);
		AddPointer(uniform);
		AddVec3(value);
	}

	void RenderThread::RC_SetVector4(Uniform* uniform, const Vector4& value)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetValue(uniform,value);
			return;
		}

		AddCommand(eRC_SetVector4);
		AddPointer(uniform);
		AddVec4(value);
	}

	void RenderThread::RC_SetVector4Count(Uniform* uniform, const Vector4* values, UINT count)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetValue(uniform,values,count);
			return;
		}

		AddCommand(eRC_SetVector4Count);
		AddPointer(uniform);
		AddDWORD(count);
		AddData(values,sizeof(Vector4) * count);
	}

	void RenderThread::RC_SetColourValue(Uniform* uniform, const ColourValue& value)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetValue(uniform,value);
			return;
		}

		AddCommand(eRC_SetColourValue);
		AddPointer(uniform);
		AddColor(value);
	}

	void RenderThread::RC_SetTexture(Uniform* uniform, const Texture* sampler)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetTexture(uniform,(Texture*)sampler);
			return;
		}

		AddCommand(eRC_SetTexture);
		AddPointer(uniform);
		AddPointer(sampler);
	}

	void RenderThread::RC_SetTextureWrap(int index, Wrap eWrap )
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetTextureWrap(index,eWrap);
			return;
		}

		AddCommand(eRC_SetTextureWrap);
		AddInt(index);
		AddInt(eWrap);
	}

	void RenderThread::RC_SetTextureFilter(int index, FilterOptions eFiler)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetTextureFilter(index,eFiler);
			return;
		}

		AddCommand(eRC_SetTextureFilter);
		AddInt(index);
		AddInt(eFiler);
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
			case eRC_ShutDown:
				{
					GetRenderSystem()->RT_ShutDown();
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
			case  eRC_CreateShader:
				{
					ShaderProgram* pShader = ReadCommand<ShaderProgram*>(n);
					pShader->RT_StreamComplete();
				}
				break;
			case  eRC_CreateRenderTarget:
				{
					Texture* pTarget = ReadCommand<Texture*>(n);
					pTarget->RT_Create();
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
			case  eRC_SetViewPort:
				{
					Rectangle viewPort;
					ReadData(n,viewPort);
					GetRenderDevice()->SetViewport(viewPort);
				}
				break;
			case  eRC_ClearBuffer:
				{
					bool bColor = ReadCommand<bool>(n);
					bool bDepth = ReadCommand<bool>(n);
					bool bStencil = ReadCommand<bool>(n);
					ColourValue c = ReadCommand<ColourValue>(n);
					float z = ReadCommand<float>(n);
					int s = ReadCommand<int>(n);
					GetRenderDevice()->ClearBuffer(bColor,bDepth,bStencil,c,z,s);
				}
				break;
			case eRC_SetDepthCheckMode:
				{
					DEPTH_CHECK_MODE eDepthChekMode = (DEPTH_CHECK_MODE)ReadCommand<int>(n);
					GetRenderDevice()->SetDepthCheckMode(eDepthChekMode);
				}
				break;
			case eRC_SetDepthWrite:
				{
					bool bDepthWrite = ReadCommand<bool>(n);
					GetRenderDevice()->SetDepthWrite(bDepthWrite);
				}
				break;
			case eRC_SetColorWrite:
				{
					bool bColorWrite = ReadCommand<bool>(n);
					GetRenderDevice()->SetColorWrite(bColorWrite);
				}
				break;
			case eRC_SetCullMode:
				{
					CULL_MODE eCullMode = (CULL_MODE)ReadCommand<int>(n);
					GetRenderDevice()->SetCullingMode(eCullMode);
				}
				break;
			case eRC_SetDepthBias:
				{
					float fDepthBias = ReadCommand<float>(n);
					GetRenderDevice()->SetDepthBias(fDepthBias);
				}
				break;
			case eRC_SetBlendMode:
				{
					BLEND_MODE eBlendMode = (BLEND_MODE)ReadCommand<int>(n);
					GetRenderDevice()->SetBlendMode(eBlendMode);
				}
				break;

			case eRC_SetFloat:
				{
					Uniform* pUnform = ReadCommand<Uniform*>(n);
					float fValue = ReadCommand<float>(n);
					GetRenderDevice()->SetValue(pUnform,fValue);
				}
				break;
			case eRC_SetMatrix4:
				{
					Uniform* pUnform = ReadCommand<Uniform*>(n);
					Matrix4 fValue = ReadCommand<Matrix4>(n);
					GetRenderDevice()->SetValue(pUnform,fValue);
				}
				break;
			case eRC_SetVector2:
				{
					Uniform* pUnform = ReadCommand<Uniform*>(n);
					Vector2 value = ReadCommand<Vector2>(n);
					GetRenderDevice()->SetValue(pUnform,value);
				}
				break;
			case eRC_SetVector3:
				{
					Uniform* pUnform = ReadCommand<Uniform*>(n);
					Vector3 value = ReadCommand<Vector3>(n);
					GetRenderDevice()->SetValue(pUnform,value);
				}
				break;
			case eRC_SetVector4:
				{
					Uniform* pUnform = ReadCommand<Uniform*>(n);
					Vector4 value = ReadCommand<Vector4>(n);
					GetRenderDevice()->SetValue(pUnform,value);
				}
				break;
			case eRC_SetVector4Count:
				{
					Uniform* pUnform = ReadCommand<Uniform*>(n);
					UINT nCount = ReadCommand<UINT>(n);
					Vector4* nValue = ReadDataPtr<Vector4*>(n,nCount * sizeof(Vector4));
					GetRenderDevice()->SetValue(pUnform,nValue,nCount);
				}
				break;
			case eRC_SetColourValue:
				{
					Uniform* pUnform = ReadCommand<Uniform*>(n);
					ColourValue cValue = ReadCommand<ColourValue>(n);
					GetRenderDevice()->SetValue(pUnform,cValue);
				}
				break;
			case eRC_SetTexture:
				{
					Uniform* pUnform = ReadCommand<Uniform*>(n);
					Texture* pTexture = ReadCommand<Texture*>(n);
					GetRenderDevice()->SetTexture(pUnform,pTexture);
				}
				break;
			case eRC_SetTextureWrap:
				{
					int index = ReadCommand<int>(n);
					Wrap eWarp = (Wrap)ReadCommand<int>(n);
					GetRenderDevice()->SetTextureWrap(index,eWarp);
				}
				break;
			case  eRC_SetTextureFilter:
				{
					int index = ReadCommand<int>(n);
					FilterOptions eFilter = (FilterOptions)ReadCommand<int>(n);
					GetRenderDevice()->SetTextureFilter(index,eFilter);
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


