
namespace ma
{

	inline void RenderThread::SignalFlushFinishedCond()
	{
		m_nFlush = 0;
	}

	inline void RenderThread::SignalFlushCond()
	{
		m_nFlush = 1;
	}

	inline void RenderThread::SignalQuitCond()
	{
		m_bQuit = 1;
	}

	inline void RenderThread::WaitFlushCond()
	{
		while (!*(volatile int*)&m_nFlush)
		{
			if (m_bQuit)
				break;
			::Sleep(0);
		}
	}

	inline void RenderThread::WaitFlushFinishedCond()
	{
		while(*(volatile int*)&m_nFlush)
		{
		#ifdef WIN32
			MSG msg;
			while (PeekMessage(&msg, Platform::GetInstance().GetWindId(), 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		#endif
		}
	}

	inline void RenderThread::InitFlushCond()
	{
		m_nFlush = 0;
	}

	inline bool RenderThread::CheckFlushCond()
	{
		return *(int*)&m_nFlush != 0;
	}

	inline void RenderThread::AddCommand(ERenderCommand eRC)
	{
		m_Commands[m_nCurThreadFill].AddElem(eRC);
	}

	inline void RenderThread::AddDWORD(UINT nVal)
	{
		*(UINT *)(m_Commands[m_nCurThreadFill].Grow(sizeof(UINT))) = nVal;
	}

	inline void RenderThread::AddDWORD64(Int64 nVal)
	{
		*(Int64 *)(m_Commands[m_nCurThreadFill].Grow(sizeof(Int64))) = nVal;
	}

	inline void RenderThread::AddFloat(const float fVal)
	{
		*(float *)(m_Commands[m_nCurThreadFill].Grow(sizeof(float))) = fVal;
	}

	inline void RenderThread::AddVec3(const Vector3& cVal)
	{
		Vector3 *vData = (Vector3 *)m_Commands[m_nCurThreadFill].Grow(sizeof(Vector3));
		*vData = cVal;
	}

	inline void RenderThread::AddColor(const Color& cVal)
	{
		float *fData = (float *)m_Commands[m_nCurThreadFill].Grow(sizeof(Color));
		fData[0] = cVal[0];
		fData[1] = cVal[1];
		fData[2] = cVal[2];
		fData[3] = cVal[3];
	}

	inline void RenderThread::AddPointer(const void *pVal)
	{
		*(const void **)(m_Commands[m_nCurThreadFill].Grow(sizeof(void *))) = pVal;
	}

	inline void RenderThread::AddData(const void *pData, int nLen)
	{
		unsigned pad = 0;
		AddDWORD(nLen + pad);
		byte *pDst = m_Commands[m_nCurThreadFill].Grow(nLen + pad);
		memcpy(pDst, pData, nLen);
	}

	template<class T> 
	inline T RenderThread::ReadCommand(int& nIndex)
	{
		T Res = *(T*)&m_Commands[m_nCurThreadProcess][nIndex];
		nIndex += sizeof(T);
		return Res;
	}

	inline int RenderThread::GetCurrentThreadId(bool bAlwaysCheck)
	{
		if (!bAlwaysCheck && m_nRenderThread == m_nMainThread)
			return m_nRenderThread;
		return ::GetCurrentThreadId();
	}

	inline int RenderThread::GetThreadList()
	{
		DWORD d = this->GetCurrentThreadId();
		if (d == m_nRenderThread)
			return m_nCurThreadProcess;
		return m_nCurThreadFill;
	}

	inline bool RenderThread::IsRenderThread(bool bAlwaysCheck)
	{
		DWORD d = this->GetCurrentThreadId(bAlwaysCheck);
		if (d == m_nRenderThread)
			return true;
		return false;
	}

	inline bool RenderThread::IsMainThread(bool bAlwaysCheck)
	{
		DWORD d = this->GetCurrentThreadId(bAlwaysCheck);
		if (d == m_nMainThread)
			return true;
		return false;
	}

	inline bool RenderThread::IsMultithreaded()
	{
		return m_pThread != NULL;
	}

	inline int RenderThread::CurThreadFill() const
	{
		return m_nCurThreadFill;
	}
}



