#ifndef  _IPhysicsScene__H__
#define  _IPhysicsScene__H__

namespace ma
{
	class ENGINE_API IPhysicsScene
	{
	public:
		virtual bool		Start()	= 0;

		virtual void		Stop() = 0;

		virtual void		SetLayerCollisionMask(xmUint8 nLayer,xmUint8 nColLayer,bool bCollide) = 0;

		virtual D3DXVECTOR3	GetGravity() const = 0;

		virtual void		SetGravity(const D3DXVECTOR3& g) = 0;

		virtual void		BeginSimulation(float fDeltaTime) = 0;

		virtual void		EndSimulation() = 0;

		virtual GameObject* RayCastCloseGameObj(const D3DXVECTOR3& rayOrig, const D3DXVECTOR3& rayDir, int nCollLayer, D3DXVECTOR3& hitPosWS) = 0;
	};
}

#endif

