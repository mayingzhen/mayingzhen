#ifndef  _BtPhysicsSystem__H__
#define  _BtPhysicsSystem__H__

namespace ma
{
	class BulletPhysicsGenericJoint;
	class BulletPhysicsHingeJoint;
	class BulletPhysicsObject;
	class PhysicsThread;

	class BtPhysicsSystem : public IPhysicsSystem
	{
	public:
		BtPhysicsSystem();

		virtual	void					Init();

		virtual void					ShoutDown();

		virtual	void					Start();

		virtual void					Stop();

		virtual void					BeginUpdate();

		virtual	void					EndUpdate() ;

		virtual void					DebugRender();

		virtual void					SetLayerCollisionMask(Uint8 nLayer,Uint8 nColLayer,bool bCollide);

		virtual Vector3					GetGravity() const;

		virtual void					SetGravity(const Vector3& g);

		virtual IPhysicsObject*			CreatePhysicsObject(GameObject* pGameObj);

		virtual void					DeletePhysicsObject(IPhysicsObject* pPhysicsObject);

		virtual	IRigidBody*				CreateRigidBody(GameObject* pGameObj);

		virtual IBoxCollisionShape*		CreateBoxCollisionShape(GameObject* pGameObj);

		virtual ISphereCollisionShape*	CreateSphereCollisionShape(GameObject* pGameObj);

		virtual ICapsuleCollisionShape* CreateCapsuleCollisionShape(GameObject* pGameObj);

		virtual ICharaControll*			CreateCharaControll(GameObject* pGameObj);

		virtual	IPhysicsGenericJoint*	CreatePhysicsGenericJoint(GameObject* pGameObj);

		virtual	IPhysicsHingeJoint*		CreatePhysicsHingeJoint(GameObject* pGameObj);

		virtual	void					DeleteRigidBody(IRigidBody* pRigidBody);

		virtual void					DeleteBoxCollisionShape(IBoxCollisionShape*	pBox);

		virtual void					DeleteSphereCollisionShape(ISphereCollisionShape* pSphere);

		virtual	void					DeleteCapsuleCollisionShape(ICapsuleCollisionShape* pCapsule);

		virtual void					DeleteCharaControll(ICharaControll* pCharcontrol);

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

		PhysicsThread*								m_pPhysicsThread;
	};

	btDiscreteDynamicsWorld* GetDynamicsWorld()
	{
		return ((BtPhysicsSystem*)GetPhysicsSystem())->GetDynamicsWorld();
	}
}

#endif
