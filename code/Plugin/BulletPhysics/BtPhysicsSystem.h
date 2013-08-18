#ifndef  _BtPhysicsSystem__H__
#define  _BtPhysicsSystem__H__

namespace ma
{
	class BulletPhysicsGenericJoint;
	class BulletPhysicsHingeJoint;
	class BulletPhysicsObject;

	class BtPhysicsSystem : public IPhysicsSystem
	{
	public:
		BtPhysicsSystem();

		virtual	void					Init();

		virtual void					ShoutDown();

		virtual	void					Start();

		virtual void					Stop();

		virtual void					Update();

		virtual void					DebugRender();

		virtual void					SetLayerCollisionMask(Uint8 nLayer,Uint8 nColLayer,bool bCollide);

		virtual Vector3					GetGravity() const;

		virtual void					SetGravity(const Vector3& g);

		virtual IPhysicsObject*			CreatePhysicsObject(GameObject* pGameObj);

		virtual void					DeletePhysicsObject(IPhysicsObject* pPhysicsObject);

		virtual	IPhysicsGenericJoint*	CreatePhysicsGenericJoint(IPhysicsObject* pPhyObjA, IPhysicsObject* pPhyObjB);

		virtual	IPhysicsHingeJoint*		CreatePhysicsHingeJoint(IPhysicsObject* pPhyObjA, IPhysicsObject* pPhyObjB);

		virtual	void					DeletePhysicsGenericJoint(IPhysicsGenericJoint* pJoint);

		virtual	void					DeletePhysicsHingeJoint(IPhysicsHingeJoint* pJoint);

		virtual GameObject*				RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer, Vector3& hitPosWS);

		btDiscreteDynamicsWorld*		GetDynamicsWorld() {return m_pDynamicsWorld;}

	private:
		btDefaultCollisionConfiguration*			m_pCollisionConfiguration;
		btCollisionDispatcher*						m_pDispatcher;
		btBroadphaseInterface*						m_pOverlappingPairCache;
		btSequentialImpulseConstraintSolver*		m_pSolver;
		btDiscreteDynamicsWorld*					m_pDynamicsWorld;

		std::vector<BulletPhysicsObject*>			m_arrPhysicsObject;

		std::vector<BulletPhysicsGenericJoint*>		m_vGenericJoint;
		std::vector<BulletPhysicsHingeJoint*>		m_vHingeJoint;

		btVector3									m_vGravity;
	};

	btDiscreteDynamicsWorld* GetDynamicsWorld()
	{
		return ((BtPhysicsSystem*)GetPhysicsSystem())->GetDynamicsWorld();
	}
}

#endif
