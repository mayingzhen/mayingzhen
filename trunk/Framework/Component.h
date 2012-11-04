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
	};
}



#endif