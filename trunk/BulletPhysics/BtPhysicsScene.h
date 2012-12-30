#ifndef  _BtPhysicsScene__H__
#define  _BtPhysicsScene__H__

namespace ma
{
	class BULLETPHYSICS_API BtPhysicsScene : public IPhysicsScene
	{
	public:
		BtPhysicsScene();
			
		virtual void Start();

		virtual void Stop();

		virtual void BeginSimulation();
	
		virtual void EndSimulation();

		virtual D3DXVECTOR3	GetGravity() const;

		virtual void		SetGravity(const D3DXVECTOR3& g);

		btDiscreteDynamicsWorld* GetDynamicsWorld() {return m_pDynamicsWorld;}

	private:
		btDefaultCollisionConfiguration*	 m_pCollisionConfiguration;
		btCollisionDispatcher*				 m_pDispatcher;
		btBroadphaseInterface*				 m_pOverlappingPairCache;
		btSequentialImpulseConstraintSolver* m_pSolver;
		btDiscreteDynamicsWorld*			 m_pDynamicsWorld;
	};
}


#endif
