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
	class CollisionMaterial;


	class ENGINE_API IPhysicsObject
	{
	public:

		virtual	IRigidBody*				CreateRigidBody() = 0;

		virtual IBoxCollisionShape*		CreateBoxCollisionShape() = 0;

		virtual ISphereCollisionShape*	CreateSphereCollisionShape() = 0;

		virtual ICapsuleCollisionShape* CreateCapsuleCollisionShape() = 0;

		virtual ICharaControll*			CreateCharaControll() = 0;

		virtual CollisionMaterial*		GetCollisionMaterial() = 0;

		virtual void					SetTransformWS(const NodeTransform& tsfWS) = 0;

		virtual NodeTransform			GetTransformWS() = 0;

		virtual void					AddCollisionListener(IPhysicsObject* objectB,CollisionListener* pListener) = 0;

		virtual void					AddCollisionListener(ICharaControll* objectB) = 0;
	};

	class ENGINE_API CollisionMaterial
	{
	public:
		int		m_nCollLayer;
		float	m_friction;
		float	m_restitution;
		float	m_rollingFriction;

		CollisionMaterial()
		{
			m_nCollLayer = 0;
			m_friction = 0;
			m_restitution = 0;
			m_rollingFriction = 0;
		}

	};

	class ENGINE_API CollisionListener
    {
    public:
        enum EventType
        {
            COLLIDING,		// Event fired when the two rigid bodies start colliding.
            NOT_COLLIDING	// Event fired when the two rigid bodies no longer collide.
        };

	
		struct CollisionData
		{
			EventType		m_eType;
			IPhysicsObject*	m_pObjectA;
			IPhysicsObject*	m_pObjectB;
			Vector3			m_vContactPointA;  // world space
			Vector3			m_vContactPointB;  // world space
			Vector3			m_vContactNoramlA; // world space
			Vector3			m_vContactNoramlB; // world space

			CollisionData()
			{
				m_pObjectA = NULL;
				m_pObjectB = NULL;
				m_vContactPointA = Vector3(0,0,0);
				m_vContactPointB = Vector3(0,0,0);
				m_vContactNoramlA = Vector3(0,0,0);
				m_vContactNoramlB = Vector3(0,0,0);
			}
		};

        virtual ~CollisionListener() { }

 
		virtual void collisionEvent(const CollisionData& eventData) = 0;
    };
}


#endif // _IPhysicsObject__H__

