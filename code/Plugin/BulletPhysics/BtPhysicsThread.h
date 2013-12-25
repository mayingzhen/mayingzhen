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
		virtual void Update();

	public:
		CMyEvent*		m_pBeginEvent;
	
		volatile bool	m_bEnd;
	
	};
}

#endif // __BulletPhysicsThread_H__
