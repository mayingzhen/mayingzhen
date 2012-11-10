#ifndef  _IPhysicsScene__H__
#define  _IPhysicsScene__H__

namespace ma
{
	class COMMON_API IPhysicsScene
	{
	public:
		virtual void BeginSimulation() = 0;
	
		virtual void EndSimulation() = 0;
	};
}


#endif
