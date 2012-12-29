#ifndef __BULLET_SCENE_H__
#define __BULLET_SCENE_H__


namespace ma
{
	class FRAMEWORK_API PhysicsScene/* : public IPhysicsScene*/
	{

	public:
		PhysicsScene();

		bool Start();

		void Stop();


		//------------------------------------------------------------------------------
		//Collision
		//------------------------------------------------------------------------------
		void		SetLayerCollisionMask(xmUint8 nLayer,xmUint8 nColLayer,bool bCollide);

		//------------------------------------------------------------------------------
		//Dynamic
		//------------------------------------------------------------------------------
		D3DXVECTOR3	GetGravity() const;

		void		SetGravity(const D3DXVECTOR3& g);

		//------------------------------------------------------------------------------
		//Simulation
		//------------------------------------------------------------------------------
		void		BeginSimulation();

		void		EndSimulation();

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