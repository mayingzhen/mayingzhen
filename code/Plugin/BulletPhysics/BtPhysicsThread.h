#ifndef __BulletPhysicsThread_H__
#define __BulletPhysicsThread_H__


namespace ma
{
	class PhysicsThread 
	{
	public:
		PhysicsThread();

		~PhysicsThread();
		
		void Stop();

		void BeginUpdate();

		void EndUpdate();

	private:
		virtual void ThreadLoop();

	public:
		CMyEvent*		m_pBeginEvent;
	
		volatile bool	m_bEnd;
	
	};
}

#endif // __BulletPhysicsThread_H__
