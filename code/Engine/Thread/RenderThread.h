#ifndef __RenderThread_H__
#define __RenderThread_H__



namespace ma
{
	enum ERenderCommand
	{
		eRC_Unknown = 0,

		eRC_Init,
		eRC_BeginFrame,
		eRC_EndFrame,
	};

	struct RenderThread
	{
		Thread*		m_pThread;
		bool		m_bQuit;
		bool		m_bSuccessful;

		int			m_nCurThreadProcess;
		int			m_nCurThreadFill;

		volatile int m_nFlush;

		UINT m_nRenderThread;
		UINT m_nMainThread;
		HRESULT m_hResult;
		TArray<byte> m_Commands[2]; // m_nCurThreadFill shows which commands are filled by main thread

		RenderThread();

		~RenderThread();

		// flushCond
		void SignalFlushFinishedCond();
		void SignalFlushCond();
		void WaitFlushCond();
		void WaitFlushFinishedCond();
		void InitFlushCond();
		bool CheckFlushCond();

		void SignalQuitCond();

		bool IsFailed();

		// Command
		void AddCommand(ERenderCommand eRC);
		void AddDWORD(UINT nVal);
		void AddDWORD64(Int64 nVal);
		void AddFloat(const float fVal);
		void AddVec3(const Vector3& cVal);
		void AddColor(const Color& cVal);
		void AddPointer(const void *pVal);
		void AddData(const void *pData, int nLen);

		template<class T> 
		T	ReadCommand(int& nIndex);

		int GetCurrentThreadId(bool bAlwaysCheck=false);

		void Init(int nCPU);
		void Quit();
		void FlushFrame();
		void FlushAndWait();
		void ProcessCommands();
		void Process();
		int  GetThreadList();
		bool IsRenderThread(bool bAlwaysCheck=false);
		bool IsMainThread(bool bAlwaysCheck=false);
		bool IsMultithreaded();
		int	 CurThreadFill() const;

		void  RC_Init(HWND hWnd);
		void  RC_BeginFrame();
		void  RC_EndFrame();

	};

}

#include "RenderThread.inl"



#endif	// __RENDERTHREAD_H__

