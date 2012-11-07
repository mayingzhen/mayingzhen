#ifndef  _COMPONENT__H__
#define  _COMPONENT__H__

namespace ma
{
	class Component
	{
	public:
		Component();
		virtual ~Component();

		virtual void Render();

		virtual void Update();

		virtual void ParalleUpdate(float fTimeElapsed);

		virtual void FixedUpdate(float fTimeElapsed);

		virtual void LateUpdate(float fTimeElapsed);

		virtual void Start();

		virtual void Stop();

	};
}



#endif