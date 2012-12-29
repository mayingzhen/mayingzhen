#ifndef  _IPhysicsScene__H__
#define  _IPhysicsScene__H__

namespace ma
{
	class COMMON_API IPhysicsScene
	{
	public:
		virtual void Start() = 0;

		virtual void Stop() = 0;

		virtual void BeginSimulation() = 0;
	
		virtual void EndSimulation() = 0;

		virtual D3DXVECTOR3	GetGravity() const = 0;

		virtual void		SetGravity(const D3DXVECTOR3& g) = 0;
	};
}


#endif
