#include "RenderThread.h"
#include "FrameBuffer.h"

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

		FlushAndWait();
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

		FlushAndWait();
	}

	void RenderThread::RC_BlendStateStreamComplete(BlendState* pBSState)
	{
		if (IsRenderThread())
		{
			pBSState->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_BlendStateStreamComplete);
		AddPointer(pBSState);

		FlushAndWait();
	}

	void RenderThread::RC_DepthStencilStateStreamComplete(DepthStencilState* pDSState)
	{
		ASSERT(pDSState);

		if (IsRenderThread())
		{
			pDSState->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_DepthStencilStreamComplete);
		AddPointer(pDSState);

		FlushAndWait();
	}

	void RenderThread::RC_RasterizerStateStreamComplete(RasterizerState* pRSState)
	{
		ASSERT(pRSState);

		if (IsRenderThread())
		{
			pRSState->RT_StreamComplete();
			return;
		}

		AddCommand(eRC_RasterizerStateStreamComplete);
		AddPointer(pRSState);

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

	void RenderThread::RC_SetFrameBuffer(FrameBuffer* pFB)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetFrameBuffer(pFB);
			return;
		}

		AddCommand(eRC_SetFrameBuffer);
		AddPointer(pFB);
	}

	void RenderThread::RC_SetRenderTarget(int index,Texture* pTexture,int level, int array_index, int face)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetRenderTarget(index,pTexture,level,array_index,face);
			return;
		}

		AddCommand(eRC_SetRenderTarget);
		AddInt(index);
		AddPointer(pTexture);
		AddInt(level);
		AddInt(array_index);
		AddInt(face);
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

	void RenderThread::RC_SetBlendState(const BlendState* pBlendState)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetBlendState(pBlendState);
			return;
		}

		AddCommand(eRC_SetBlendState);
		AddPointer(pBlendState);
	}

	void RenderThread::RC_SetDepthStencilState(const DepthStencilState* pDSState, UINT nStencilRef)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetDepthStencilState(pDSState,nStencilRef);
			return;
		}

		AddCommand(eRC_SetDepthStencilState);
		AddPointer(pDSState);
		AddDWORD(nStencilRef);
	}

	void RenderThread::RC_SetRasterizerState(const RasterizerState* pRSState)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetRasterizerState(pRSState);
			return;
		}

		AddCommand(eRC_SetRasterizerState);
		AddPointer(pRSState);
	}

	void RenderThread::RC_SetVertexDeclation(const VertexDeclaration* pVertexDecl)
	{
		if (IsRenderThread())
		{
			GetRenderDevice()->SetVertexDeclaration(pVertexDecl);
			return;
		}

		AddCommand(eRC_SetVertexDeclation);
		AddPointer(pVertexDecl);
	}

// 	void RenderThread::RC_SetInt(Uniform* uniform, int value)
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->SetValue(uniform,value);
// 			return;
// 		}
// 
// 		AddCommand(eRC_SetInt);
// 		AddPointer(uniform);
// 		AddInt(value);
// 	}
// 
// 	void RenderThread::RC_SetFloat(Uniform* uniform, float value)
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->SetValue(uniform,value);
// 			return;
// 		}
// 
// 		AddCommand(eRC_SetFloat);
// 		AddPointer(uniform);
// 		AddFloat(value);
// 	}
// 
// 	void RenderThread::RC_SetVector2(Uniform* uniform, const Vector2& value)
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->SetValue(uniform,value);
// 			return;
// 		}
// 
// 		AddCommand(eRC_SetVector2);
// 		AddPointer(uniform);
// 		AddVec2(value);
// 	}
// 
// 	void RenderThread::RC_SetVector3(Uniform* uniform, const Vector3& value)
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->SetValue(uniform,value);
// 			return;
// 		}
// 
// 		AddCommand(eRC_SetVector3);
// 		AddPointer(uniform);
// 		AddVec3(value);
// 	}
// 
// 
// 	void RenderThread::RC_SetVector4(Uniform* uniform, const Vector4* values, UINT count)
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->SetValue(uniform,values,count);
// 			return;
// 		}
// 
// 		AddCommand(eRC_SetVector4);
// 		AddPointer(uniform);
// 		AddDWORD(count);
// 		AddData(values,sizeof(Vector4) * count);
// 	}
// 
// 	void RenderThread::RC_SetMatrix4(Uniform* uniform, const Matrix4* values, UINT count)
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->SetValue(uniform,values,count);
// 			return;
// 		}
// 
// 		AddCommand(eRC_SetMatrix4);
// 		AddPointer(uniform);
// 		AddDWORD(count);
// 		AddData(values,sizeof(Matrix4) * count);
// 	}
// 
// 	void RenderThread::RC_SetColourValue(Uniform* uniform, const ColourValue& value)
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->SetValue(uniform,value);
// 			return;
// 		}
// 
// 		AddCommand(eRC_SetColourValue);
// 		AddPointer(uniform);
// 		AddColor(value);
// 	}
// 
// 	void RenderThread::RC_SetTexture(Uniform* uniform, const Texture* sampler)
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->SetTexture(uniform,(Texture*)sampler);
// 			return;
// 		}
// 
// 		AddCommand(eRC_SetTexture);
// 		AddPointer(uniform);
// 		AddPointer(sampler);
// 	}
// 
// 	void RenderThread::RC_SetSamplerState(Uniform* uniform, const SamplerState* sampler)
// 	{
// 		if (IsRenderThread())
// 		{
// 			GetRenderDevice()->SetSamplerState(uniform,(SamplerState*)sampler);
// 			return;
// 		}
// 
// 		AddCommand(eRC_SetSamplerState);
// 		AddPointer(uniform);
// 		AddPointer(sampler);
// 	}


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
			case  eRC_SetShader:
				{
					ShaderProgram* pShader = ReadCommand<ShaderProgram*>(n);
					pShader->RT_SetShader();
				}
				break;
			case eRC_SetFrameBuffer:
				{
					FrameBuffer* pFB= ReadCommand<FrameBuffer*>(n);
					GetRenderDevice()->SetFrameBuffer(pFB);
				}
				break;
			case eRC_SetRenderTarget:
				{
					int index = ReadCommand<int>(n);
					Texture* pTarget = ReadCommand<Texture*>(n);
					int level = ReadCommand<int>(n);
					int array_index = ReadCommand<int>(n);
					int face = ReadCommand<int>(n);
					GetRenderDevice()->SetRenderTarget(index,pTarget,level,array_index,face);
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
			case eRC_SetBlendState:
				{
					BlendState* pBlendState = ReadCommand<BlendState*>(n);
					GetRenderDevice()->SetBlendState(pBlendState);
				}
				break;
			case eRC_SetDepthStencilState:
				{
					DepthStencilState* pDSState = ReadCommand<DepthStencilState*>(n);
					UINT nRef = ReadCommand<UINT>(n);
					GetRenderDevice()->SetDepthStencilState(pDSState, nRef);
				}
				break;
			case eRC_SetRasterizerState:
				{
					RasterizerState* pRSState = ReadCommand<RasterizerState*>(n);
					GetRenderDevice()->SetRasterizerState(pRSState);
				}
				break;
			case eRC_SetVertexDeclation:
				{
					VertexDeclaration* pVertexDecl = ReadCommand<VertexDeclaration*>(n);
					GetRenderDevice()->SetVertexDeclaration(pVertexDecl);
				}
			break;
// 			case eRC_SetInt:
// 				{
// 					Uniform* pUnform = ReadCommand<Uniform*>(n);
// 					int nValue = ReadCommand<int>(n);
// 					GetRenderDevice()->SetValue(pUnform,nValue);
// 				}
// 				break;
// 			case eRC_SetFloat:
// 				{
// 					Uniform* pUnform = ReadCommand<Uniform*>(n);
// 					float fValue = ReadCommand<float>(n);
// 					GetRenderDevice()->SetValue(pUnform,fValue);
// 				}
// 				break;
// 			case eRC_SetVector2:
// 				{
// 					Uniform* pUnform = ReadCommand<Uniform*>(n);
// 					Vector2 value = ReadCommand<Vector2>(n);
// 					GetRenderDevice()->SetValue(pUnform,value);
// 				}
// 				break;
// 			case eRC_SetVector3:
// 				{
// 					Uniform* pUnform = ReadCommand<Uniform*>(n);
// 					Vector3 value = ReadCommand<Vector3>(n);
// 					GetRenderDevice()->SetValue(pUnform,value);
// 				}
// 				break;
// 			case eRC_SetVector4:
// 				{
// 					Uniform* pUnform = ReadCommand<Uniform*>(n);
// 					UINT nCount = ReadCommand<UINT>(n);
// 					Vector4* nValue = ReadDataPtr<Vector4*>(n,nCount * sizeof(Vector4));
// 					GetRenderDevice()->SetValue(pUnform,nValue,nCount);
// 				}
// 				break;
// 			case eRC_SetMatrix4:
// 				{
// 					Uniform* pUnform = ReadCommand<Uniform*>(n);
// 					UINT nCount = ReadCommand<UINT>(n);
// 					Matrix4* nValue = ReadDataPtr<Matrix4*>(n,nCount * sizeof(Matrix4));
// 					GetRenderDevice()->SetValue(pUnform,nValue,nCount);
// 				}
// 				break;
// 			case eRC_SetColourValue:
// 				{
// 					Uniform* pUnform = ReadCommand<Uniform*>(n);
// 					ColourValue cValue = ReadCommand<ColourValue>(n);
// 					GetRenderDevice()->SetValue(pUnform,cValue);
// 				}
// 				break;
// 			case eRC_SetTexture:
// 				{
// 					Uniform* pUnform = ReadCommand<Uniform*>(n);
// 					Texture* pTexture = ReadCommand<Texture*>(n);
// 					GetRenderDevice()->SetTexture(pUnform,pTexture);
// 				}
// 				break;
// 			case eRC_SetSamplerState:
// 				{
// 					Uniform* pUnform = ReadCommand<Uniform*>(n);
// 					SamplerState* pTexture = ReadCommand<SamplerState*>(n);
// 					GetRenderDevice()->SetSamplerState(pUnform,pTexture);
// 				}
// 				break;
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


