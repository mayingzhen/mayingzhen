#include "BtPhysicsThread.h"

namespace ma
{
	void PhysicsThread::Update()
	{
		m_pBeginEvent->Wait();

		m_bEnd = false;

		GetDynamicsWorld()->stepSimulation(GetTimer()->GetFrameDeltaTime());

		m_bEnd = true;
	}

	PhysicsThread::PhysicsThread()
	{
		m_pBeginEvent = new CMyEvent();
	}

	PhysicsThread::~PhysicsThread()
	{
        SAFE_DELETE(m_pBeginEvent);
	}

	void PhysicsThread::BeginUpdate()
	{
		m_pBeginEvent->Signal();
	}

	void PhysicsThread::EndUpdate()
	{
		while(!m_bEnd)
		{
#ifdef WIN32
			MSG msg;
			while (PeekMessage(&msg, Platform::GetInstance().GetWindId(), 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
#else
			::usleep(1);
#endif
		}
	}
}