#ifndef __IPhysicsCollisionShape_H__
#define __IPhysicsCollisionShape_H__


namespace ma
{
	class ENGINE_API ICollisionShape : public Component
	{
		DECL_OBJECT(ICollisionShape)

	public:
		virtual void			SetTransformLS(const Transform& tsfLS) = 0;

		virtual Transform		GetTransformLS() = 0;

		virtual void*			Create() = 0;
	};

	class ENGINE_API IBoxCollisionShape : public ICollisionShape
	{
		DECL_OBJECT(IBoxCollisionShape)

	public:
		virtual void			SetSize(const Vector3& vSize) = 0;

		virtual	Vector3			GetSize() = 0;
	};

	class ENGINE_API ISphereCollisionShape : public ICollisionShape
	{
		DECL_OBJECT(ISphereCollisionShape)

	public:
		virtual void			SetRadius(float fRadius) = 0;

		virtual	float			GetRadius() = 0;
	};

	class ENGINE_API ICapsuleCollisionShape : public ICollisionShape
	{
		DECL_OBJECT(ICapsuleCollisionShape)

	public:
		virtual void			SetHeight(float fHeight) = 0;

		virtual float			GetHeight() const = 0;

		virtual void			SetRadius(float fRadius) = 0;

		virtual float			GetRadius() const = 0;
	};

	class ENGINE_API ICollisionMaterial :  public Component 
	{
		DECL_OBJECT(ICollisionMaterial)

	public:
		virtual void			SetCollLayer(int nLayer) = 0;

		virtual	int				GetCollLayer() = 0;

		virtual	void			SetFriction(float fFriction) = 0;

		virtual	float			GetFriction() = 0;

		virtual void			SetRestitution(float fRestitution) = 0;

		virtual float			GetRestitution() = 0;

		virtual void			SetRollingFriction(float fRollingFriction) = 0;

		virtual float			GetRollingFriction() = 0;

	};

	DeclareRefPtr(IBoxCollisionShape);
	DeclareRefPtr(ISphereCollisionShape);
	DeclareRefPtr(ICapsuleCollisionShape);
	DeclareRefPtr(ICollisionMaterial)

	struct ENGINE_API Collision
	{
		SceneNode* m_pGameEntity;
		Vector3	m_vContactsPointWS;
		Vector3	m_vContactsNormalWS;

		Collision()
		{
			m_pGameEntity = NULL;
			m_vContactsPointWS = Vector3(0,0,0);
			m_vContactsNormalWS = Vector3(0,0,0);
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
			SceneNode*		m_pObjectA;
			SceneNode*		m_pObjectB;
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

#endif

