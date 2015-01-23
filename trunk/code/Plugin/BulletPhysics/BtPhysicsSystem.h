#ifndef  _BtPhysicsSystem__H__
#define  _BtPhysicsSystem__H__

namespace ma
{
	class PhysicsGenericJoint;
	class PhysicsHingeJoint;
	class BulletPhysicsObject;
	class PhysicsThread;

	class BULLETPHYSICS_API PhysicsSystem :  public btIDebugDraw
	{
	public:
		PhysicsSystem();

		void							Init();

		void							ShoutDown();

		void							Start();

		void							Stop();

		void							BeginUpdate();

		void							EndUpdate() ;

		void							DebugRender();

		void							SetLayerCollisionMask(uint8 nLayer,uint8 nColLayer,bool bCollide);

		Vector3							GetGravity() const;

		void							SetGravity(const Vector3& g);

		SceneNode*						RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer, Vector3& hitPosWS);
		
		void							AddPhysicsObject(SceneNode* pGameObj);

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

		btCollisionObject*				InitCollObject(SceneNode* pGameObj);

		void							StartPhysicsObject(SceneNode* pGameObj);	

	private:
		btDefaultCollisionConfiguration*			m_pCollisionConfiguration;
		btCollisionDispatcher*						m_pDispatcher;
		btBroadphaseInterface*						m_pOverlappingPairCache;
		btSequentialImpulseConstraintSolver*		m_pSolver;
		btDiscreteDynamicsWorld*					m_pDynamicsWorld;
		btGhostPairCallback*						m_pbtGhostPairCallback;

		std::vector<SceneNode*>						m_arrPhysicsObject;

		btVector3									m_vGravity;

		PhysicsThread*								m_pPhysicsThread;

		int											m_debugMode;
	};

	BULLETPHYSICS_API void SetPhysicsSystem(PhysicsSystem* pPhysicsSystem);

	BULLETPHYSICS_API PhysicsSystem* GetPhysicsSystem();
}

#endif
