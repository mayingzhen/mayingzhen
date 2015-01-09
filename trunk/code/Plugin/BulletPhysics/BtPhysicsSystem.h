#ifndef  _BtPhysicsSystem__H__
#define  _BtPhysicsSystem__H__

namespace ma
{
	class BulletPhysicsGenericJoint;
	class BulletPhysicsHingeJoint;
	class BulletPhysicsObject;
	class PhysicsThread;

	class BtPhysicsSystem :  public btIDebugDraw
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

		virtual void					SetLayerCollisionMask(uint8 nLayer,uint8 nColLayer,bool bCollide);

		virtual Vector3					GetGravity() const;

		virtual void					SetGravity(const Vector3& g);

// 		virtual	BulletRigidBody*		CreateRigidBody();
// 
// 		virtual BulletBoxCollisionShape*		CreateBoxCollisionShape();
// 
// 		virtual BulletSphereCollisionShape*	CreateSphereCollisionShape();
// 
// 		virtual BulletCapsuleCollisionShape* CreateCapsuleCollisionShape();
// 		
// 		virtual BulletCollisionMaterial*		CreateCollisionMaterial();
// 
// 		virtual BulletCharacterController*			CreateCharaControll();
// 
// 		virtual	BulletPhysicsGenericJoint*	CreatePhysicsGenericJoint();
// 
// 		virtual	BulletPhysicsHingeJoint*		CreatePhysicsHingeJoint();

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

	BULLETPHYSICS_API void SetPhysicsSystem(BtPhysicsSystem* pPhysicsSystem);

	BULLETPHYSICS_API BtPhysicsSystem* GetPhysicsSystem();

	btDiscreteDynamicsWorld* GetDynamicsWorld()
	{
		return GetPhysicsSystem()->GetDynamicsWorld();
	}
}

#endif
