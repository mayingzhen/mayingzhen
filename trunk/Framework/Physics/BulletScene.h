#ifndef __BULLET_SCENE_H__
#define __BULLET_SCENE_H__

#include "Common/IPhysicsScene.h"

namespace xm
{
	class FRAMEWORK_API PhysicsScene/* : public IPhysicsScene*/
	{

	public:
		PhysicsScene();

		virtual bool Start();

		virtual void Stop();


		//------------------------------------------------------------------------------
		//Collision
		//------------------------------------------------------------------------------
		virtual void		SetLayerCollisionMask(xmUint8 nLayer,xmUint8 nColLayer,bool bCollide);

		//------------------------------------------------------------------------------
		//Dynamic
		//------------------------------------------------------------------------------
		virtual D3DXVECTOR3	GetGravity() const;

		virtual void		SetGravity(const D3DXVECTOR3& g);

		//------------------------------------------------------------------------------
		//Simulation
		//------------------------------------------------------------------------------
		virtual void		BeginSimulation();

		virtual void		EndSimulation();

		btDiscreteDynamicsWorld* GetBulletWorld() {return m_pDynamicsWorld;}

	private:
		btDefaultCollisionConfiguration*	 m_pCollisionConfiguration;
		btCollisionDispatcher*				 m_pDispatcher;
		btBroadphaseInterface*				 m_pOverlappingPairCache;
		btSequentialImpulseConstraintSolver* m_pSolver;
		btDiscreteDynamicsWorld*			 m_pDynamicsWorld;
	};
}


#endif