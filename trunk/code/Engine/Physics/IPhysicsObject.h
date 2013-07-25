#ifndef  _IPhysicsObject__H__
#define  _IPhysicsObject__H__

namespace ma
{
	class IRigidBody;
	class IBoxCollisionShape;
	class ISphereCollisionShape;
	class IPhysicsGenericJoint;
	class IPhysicsHingeJoint;
	class CollisionListener;


	class ENGINE_API IPhysicsObject
	{
	public:

		virtual	IRigidBody*				CreateRigidBody() = 0;

		virtual IBoxCollisionShape*		CreateBoxCollisionShape() = 0;

		virtual ISphereCollisionShape*	CreateSphereCollisionShape() = 0;

		virtual	IPhysicsGenericJoint*	CreatePhysicsGenericJoint(IPhysicsObject* pPhyObjB) = 0;

		virtual	IPhysicsHingeJoint*		CreatePhysicsHingeJoint(IPhysicsObject* pPhyObjB) = 0;

		virtual void					SetTransformWS(const NodeTransform& tsfWS) = 0;

		virtual NodeTransform			GetTransformWS() = 0;

		virtual void					AddCollisionListener(IPhysicsObject* objectB,CollisionListener* pListener) = 0;

		virtual void					AddCollisionListener(ICharaControll* objectB) = 0;
	};

	class CollisionPair
	{
	public:
		CollisionPair(void* objectA, void* objectB)
		{
			m_pObjectA = objectA;
			m_pObjectB = objectB;
		}

		bool operator < (const CollisionPair& collisionPair) const
		{
			// If the pairs are equal, then return false.
			if ((m_pObjectA == collisionPair.m_pObjectA && m_pObjectB == collisionPair.m_pObjectB) || 
				(m_pObjectA == collisionPair.m_pObjectB && m_pObjectB == collisionPair.m_pObjectA))
				return false;

			// We choose to compare based on objectA arbitrarily.
			if (m_pObjectA < collisionPair.m_pObjectA)
				return true;

			if (m_pObjectA == collisionPair.m_pObjectA)
				return m_pObjectB < collisionPair.m_pObjectB;

			return false;

		}

		void* m_pObjectA;

		void* m_pObjectB;
	};


	class CollisionListener
    {
    public:
        enum EventType
        {
            COLLIDING,		// Event fired when the two rigid bodies start colliding.
            NOT_COLLIDING	// Event fired when the two rigid bodies no longer collide.
        };

	
		struct EventData
		{
			EventType	m_eType;
			GameObject*	m_pGameObjectA;
			GameObject*	m_pGameObjectB;
			Vector3		m_vContactPointA;
			Vector3		m_vContactPointB;
		};

        /**
         * Virtual destructor.
         */
        virtual ~CollisionListener() { }

        /**
         * Called when a collision occurs between two objects in the physics world.
         * 
         * NOTE: You are not permitted to disable physics objects from within this callback. Disabling physics on a collision object
         *  removes the object from the physics world. This is not permitted during the PhysicsController::update.
         *
         * @param type The type of collision event.
         * @param collisionPair The two collision objects involved in the collision.
         * @param contactPointA The contact point with the first object (in world space).
         * @param contactPointB The contact point with the second object (in world space).
         */
		virtual void collisionEvent(CollisionListener::EventType type,
                                    const CollisionPair& collisionPair,
                                    const Vector3& contactPointA,
                                    const Vector3& contactPointB) = 0;
    };
}


#endif // _IPhysicsObject__H__

