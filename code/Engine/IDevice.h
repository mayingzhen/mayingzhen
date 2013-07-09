#ifndef __IDevice_H__
#define __IDevice_H__

namespace ma
{
	class ENGINE_API IDevice 
	{
	public:

		virtual void Init() = 0;

		virtual void Shutdown() = 0;

		virtual void Update() = 0;
	};
}


#endif