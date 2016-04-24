#include "RenderThread.h"


namespace ma
{
#ifdef WIN32
	HWND GetRenderWindowHandle()
	{
		return GetRenderSystem()->GetMainWnd();
	}
#endif
	
	RenderThread::RenderThread():Thread("RenderThread")
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
		SignalFlushFinishedCond();

		Thread::Stop();
	}

	void RenderThread::ThreadLoop()
	{
		while (!m_bExit)
		{
			uint64 nTime = GetTimer()->GetMillisceonds();
			WaitFlushCond();
			uint64 nTimeAfterWait = GetTimer()->GetMillisceonds();
			uint64 nTimeWaitForMain = nTimeAfterWait - nTime;
			LogInfo("fTimeWaitForMain = %d",nTimeWaitForMain);
			//gRenDev->m_fTimeWaitForMain[m_nCurThreadProcess] += fTimeAfterWait - fTime;
			ProcessCommands();
			uint64 nTimeAfterProcess = GetTimer()->GetMillisceonds();
			uint64 nTimeProcessedRT = nTimeAfterProcess - nTimeAfterWait;
			LogInfo("fTimeProcessedRT = %d",nTimeProcessedRT);
			//gRenDev->m_fTimeProcessedRT[m_nCurThreadFill] += fTimeAfterProcess - fTimeAfterWait;
		}
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

	void RenderThread::RC_Reset(uint32 nWidth,uint32 nHeight)
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

	void RenderThread::RC_DrawRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		if (IsRenderThread())
		{
			GetRenderSystem()->RT_DrawRenderable(pRenderable,pTechnique);
			return;
		}

		AddCommand(eRC_DrawRenderable);
		AddPointer(pRenderable);
		AddPointer(pTechnique);
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
			pRenderTarget->RT_CreateTexture();
			return;
		}

		AddCommand(eRC_CreateTexture);
		AddPointer(pRenderTarget);
	}

	void RenderThread::RC_CreateDepthStencil(Texture* pRenderTarget)
	{
		if (IsRenderThread())
		{
			pRenderTarget->RT_CreateDepthStencil();
			return;
		}

		AddCommand(eRC_CreateDepthStencil);
		AddPointer(pRenderTarget);
	}

	void RenderThread::RC_SetShaderProgram(ShaderProgram* pShader)
	{
		if (IsRenderThread())
		{
			pShader->RT_SetShader();	
 
			return;
		}

		AddCommand(eRC_SetShader);
		AddPointer(pShader);
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

	void RenderThread::RC_SetDepthCheckMode(CompareFunction eDepthCheckMode)
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

	void RenderThread::RC_SetSRGBWite(bool b)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetSRGBWrite(b);
			return;
		}

		AddCommand(eRC_SetSRGBWite);
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

	void RenderThread::RC_SetDepthBias(float fConstantBias,float slopeScaleBias)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetDepthBias(fConstantBias,slopeScaleBias);
			return;
		}

		AddCommand(eRC_SetDepthBias);
		AddFloat(fConstantBias);
		AddFloat(slopeScaleBias);
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

	void RenderThread::RC_SetStencilCheckEnabled(bool b)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetStencilEnable(b);
			return;
		}

		AddCommand(eRC_SetStenCilEnabled);
		AddBool(b);
	}

	void RenderThread::RC_SetStencilBufferParams(CompareFunction func, 
		uint32 refValue, uint32 mask, uint32 writeMask,
		StencilOperation stencilFailOp, 
		StencilOperation depthFailOp,
		StencilOperation passOp, 
		bool twoSidedOperatio)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetStencilBufferParams(func,refValue,mask,writeMask,stencilFailOp,depthFailOp,passOp,twoSidedOperatio);
			return;
		}

		AddCommand(eRC_SetStenCilParam);
		AddInt(func);
		AddDWORD(refValue);
		AddDWORD(mask);
		AddDWORD(writeMask);
		AddInt(stencilFailOp);
		AddInt(depthFailOp);
		AddInt(passOp);
		AddBool(twoSidedOperatio);
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


	void RenderThread::RC_SetVector4(Uniform* uniform, const Vector4* values, UINT count)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetValue(uniform,values,count);
			return;
		}

		AddCommand(eRC_SetVector4);
		AddPointer(uniform);
		AddDWORD(count);
		AddData(values,sizeof(Vector4) * count);
	}

	void RenderThread::RC_SetMatrix4(Uniform* uniform, const Matrix4* values, UINT count)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetValue(uniform,values,count);
			return;
		}

		AddCommand(eRC_SetMatrix4);
		AddPointer(uniform);
		AddDWORD(count);
		AddData(values,sizeof(Matrix4) * count);
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

	void RenderThread::RC_SetSamplerState(Uniform* uniform, const SamplerState* sampler)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetSamplerState(uniform,(SamplerState*)sampler);
			return;
		}

		AddCommand(eRC_SetSamplerState);
		AddPointer(uniform);
		AddPointer(sampler);
	}


	void RenderThread::RC_SetPoolId(uint32 poolId)
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
			BYTE nC = m_Commands[m_nCurThreadProcess][n++];
			switch (nC)
			{
			case eRC_Init:
				{
					HWND wndhandle = ReadCommand<HWND>(n);
					GetRenderSystem()->RT_Init(wndhandle);
				}
				break;
			case eRC_Reset:
				{
					uint32 nWidth = ReadCommand<uint32>(n);
					uint32 nHeight = ReadCommand<uint32>(n);
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
			case  eRC_DrawRenderable:
				{
					Renderable* pRenderable = ReadCommand<Renderable*>(n);
					Technique* pTech = ReadCommand<Technique*>(n);
					GetRenderSystem()->RT_DrawRenderable(pRenderable,pTech);
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
			case  eRC_CreateShader:
				{
					ShaderProgram* pShader = ReadCommand<ShaderProgram*>(n);
					pShader->RT_StreamComplete();
				}
				break;
			case  eRC_CreateTexture:
				{
					Texture* pTarget = ReadCommand<Texture*>(n);
					pTarget->RT_CreateTexture();
				}
				break;
			case  eRC_CreateDepthStencil:
				{
					Texture* pTarget = ReadCommand<Texture*>(n);
					pTarget->RT_CreateDepthStencil();
				}
				break;
			case  eRC_SetShader:
				{
					ShaderProgram* pShader = ReadCommand<ShaderProgram*>(n);
					pShader->RT_SetShader();
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
					CompareFunction eDepthChekMode = (CompareFunction)ReadCommand<int>(n);
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
			case  eRC_SetSRGBWite:
				{
					bool bSRGBWrite = ReadCommand<bool>(n);
					GetRenderDevice()->SetSRGBWrite(bSRGBWrite);
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
					float fconstantBias = ReadCommand<float>(n);
					float fSlopeScaleBias = ReadCommand<float>(n);
					GetRenderDevice()->SetDepthBias(fconstantBias,fSlopeScaleBias);
				}
				break;
			case eRC_SetBlendMode:
				{
					BLEND_MODE eBlendMode = (BLEND_MODE)ReadCommand<int>(n);
					GetRenderDevice()->SetBlendMode(eBlendMode);
				}
				break;
			case eRC_SetStenCilEnabled:
				{
					bool b = ReadCommand<bool>(n);
					GetRenderDevice()->SetStencilEnable(b);
				}
				break;
			case eRC_SetStenCilParam:
				{
					CompareFunction func = (CompareFunction)ReadCommand<int>(n);
					uint32 refValue = ReadCommand<DWORD>(n); 
					uint32 mask = ReadCommand<DWORD>(n);
					uint32 writeMask = ReadCommand<DWORD>(n);
					StencilOperation stencilFailOp = (StencilOperation)ReadCommand<int>(n);
					StencilOperation depthFailOp = (StencilOperation)ReadCommand<int>(n);
					StencilOperation passOp = (StencilOperation)ReadCommand<int>(n);
					bool twoSidedOperatio = ReadCommand<bool>(n);

					GetRenderDevice()->SetStencilBufferParams(func,refValue,mask,writeMask,stencilFailOp,depthFailOp,passOp,twoSidedOperatio);
				}
				break;
			case eRC_SetFloat:
				{
					Uniform* pUnform = ReadCommand<Uniform*>(n);
					float fValue = ReadCommand<float>(n);
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
					UINT nCount = ReadCommand<UINT>(n);
					Vector4* nValue = ReadDataPtr<Vector4*>(n,nCount * sizeof(Vector4));
					GetRenderDevice()->SetValue(pUnform,nValue,nCount);
				}
				break;
			case eRC_SetMatrix4:
				{
					Uniform* pUnform = ReadCommand<Uniform*>(n);
					UINT nCount = ReadCommand<UINT>(n);
					Matrix4* nValue = ReadDataPtr<Matrix4*>(n,nCount * sizeof(Matrix4));
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
			case eRC_SetSamplerState:
				{
					Uniform* pUnform = ReadCommand<Uniform*>(n);
					SamplerState* pTexture = ReadCommand<SamplerState*>(n);
					GetRenderDevice()->SetSamplerState(pUnform,pTexture);
				}
				break;
			case  eRC_SetPoolId:
				{
					UINT nId = ReadCommand<UINT>(n);
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

		uint64 nTime = GetTimer()->GetMillisceonds();
		WaitFlushFinishedCond();
		uint64 nTimeWaitForRender = GetTimer()->GetMillisceonds() - nTime;
		LogInfo("fTimeWaitForRender = %d",nTimeWaitForRender);
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


