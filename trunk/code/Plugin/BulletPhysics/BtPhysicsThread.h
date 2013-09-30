#ifndef __BulletPhysicsThread_H__
#define __BulletPhysicsThread_H__


namespace ma
{
	class PhysicsThread : public Thread
	{
	public:
		PhysicsThread();

		~PhysicsThread();

		void BeginUpdate();

		void EndUpdate();

	private:
		virtual void Run();

	public:
		CMyEvent*	m_pBeginEvent;
		CMyEvent*	m_pEndEvent;
	};
}

#endif // __BulletPhysicsThread_H__
