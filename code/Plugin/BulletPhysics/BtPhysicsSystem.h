#ifndef  _BtPhysicsSystem__H__
#define  _BtPhysicsSystem__H__

namespace ma
{
	class BulletPhysicsGenericJoint;
	class BulletPhysicsHingeJoint;
	class BulletPhysicsObject;
	class PhysicsThread;

	class BtPhysicsSystem : public IPhysicsSystem, public btIDebugDraw
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

		virtual	IRigidBody*				CreateRigidBody(SceneNode* pGameObj);

		virtual IBoxCollisionShape*		CreateBoxCollisionShape(SceneNode* pGameObj);

		virtual ISphereCollisionShape*	CreateSphereCollisionShape(SceneNode* pGameObj);

		virtual ICapsuleCollisionShape* CreateCapsuleCollisionShape(SceneNode* pGameObj);
		
		virtual ICollisionMaterial*		CreateCollisionMaterial(SceneNode* pGameObj);

		virtual ICharaControll*			CreateCharaControll(SceneNode* pGameObj);

		virtual	IPhysicsGenericJoint*	CreatePhysicsGenericJoint(SceneNode* pGameObj);

		virtual	IPhysicsHingeJoint*		CreatePhysicsHingeJoint(SceneNode* pGameObj);

		virtual	void					DeleteRigidBody(IRigidBody* pRigidBody);

		virtual void					DeleteBoxCollisionShape(IBoxCollisionShape*	pBox);

		virtual void					DeleteSphereCollisionShape(ISphereCollisionShape* pSphere);

		virtual	void					DeleteCapsuleCollisionShape(ICapsuleCollisionShape* pCapsule);

		virtual void					DeleteCharaControll(ICharaControll* pCharcontrol);

		virtual	void					DeletePhysicsGenericJoint(IPhysicsGenericJoint* pJoint);

		virtual	void					DeletePhysicsHingeJoint(IPhysicsHingeJoint* pJoint);

		virtual SceneNode*				RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer, Vector3& hitPosWS);

		btDiscreteDynamicsWorld*		GetDynamicsWorld() {return m_pDynamicsWorld;}

		// btIDebugDraw
		//////////////////////////////////////////////////////////////////////////
		virtual bool					isVisible(const btVector3& aabbMin, const btVector3& aabbMax);

		virtual void					drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

		virtual void					reportErrorWarning(const char* warningString);

		virtual void					drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {}

		virtual void					draw3dText(const btVector3& location,const char* textString) {}

		virtual void					setDebugMode(int debugMode) {m_debugMode = debugMode;}

		virtual int						getDebugMode() const { return m_debugMode; }

	protected:
		void							AddPhysicsObject(SceneNode* pGameObj);

		btCollisionObject*				InitCollObject(SceneNode* pGameObj);

		void							StartPhysicsObject(SceneNode* pGameObj);	

	private:
		btDefaultCollisionConfiguration*			m_pCollisionConfiguration;
		btCollisionDispatcher*						m_pDispatcher;
		btBroadphaseInterface*						m_pOverlappingPairCache;
		btSequentialImpulseConstraintSolver*		m_pSolver;
		btDiscreteDynamicsWorld*					m_pDynamicsWorld;

		std::vector<SceneNode*>						m_arrPhysicsObject;

		btVector3									m_vGravity;

		PhysicsThread*								m_pPhysicsThread;

		int											m_debugMode;
	};

	btDiscreteDynamicsWorld* GetDynamicsWorld()
	{
		return ((BtPhysicsSystem*)GetPhysicsSystem())->GetDynamicsWorld();
	}
}

#endif
