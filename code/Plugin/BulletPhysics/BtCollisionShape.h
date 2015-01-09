#ifndef __Bullet_CollisionShape_H__
#define __Bullet_CollisionShape_H__


namespace ma
{
	class BulletCollisionShape : public Component
	{

	public : 
		virtual Transform	GetTransformLS() {return m_tsfLS;} 
		virtual void		SetTransformLS(const Transform& tsfLS) { m_tsfLS = tsfLS; }

		virtual void*		Create() {return NULL;}

	protected: 
		Transform			m_tsfLS;

		btCollisionShape*	m_pBtShape; 
	};

	class BulletBoxCollisionShape : public BulletCollisionShape
	{	

	public:	
		BulletBoxCollisionShape();

		~BulletBoxCollisionShape();

		virtual void			SetSize(const Vector3& vSize);

		virtual	Vector3			GetSize(); 

		virtual void			Serialize(Serializer& sl, const char* pszLable = "IBoxCollisionShape");

		virtual	void*			Create();

	private:

		Vector3					m_vSize;
	};

	
	class BulletSphereCollisionShape : public BulletCollisionShape
	{

	public:	
		BulletSphereCollisionShape();

		~BulletSphereCollisionShape();

		virtual void			SetRadius(float fRadius);

		virtual	float			GetRadius(); 

		virtual void			Serialize(Serializer& sl, const char* pszLable = "ISphereCollisionShape");
		
		virtual	void*			Create();

	private:

		float					m_fRadius;
	};

	class BulletCapsuleCollisionShape : public BulletCollisionShape
	{

	public:
		BulletCapsuleCollisionShape();	

		~BulletCapsuleCollisionShape();

		virtual void			SetHeight(float fHeight);

		virtual float			GetHeight() const;

		virtual void			SetRadius(float fRadius);

		virtual float			GetRadius() const;

		virtual void			Serialize(Serializer& sl, const char* pszLable = "ICapsuleCollisionShape");

		virtual	void*			Create();

	private:
		float					m_fHeight;
		float					m_fRadius;
	};	

	class BulletCollisionMaterial : public Component
	{
		DECL_OBJECT(BulletCollisionMaterial)

	public:
		BulletCollisionMaterial();

		virtual void			SetCollLayer(int nLayer) {m_nCollLayer = nLayer;}

		virtual	int				GetCollLayer() {return m_nCollLayer;}

		virtual	void			SetFriction(float fFriction) {m_friction = fFriction;}

		virtual	float			GetFriction() {return m_friction;}

		virtual void			SetRestitution(float fRestitution) {m_restitution = fRestitution;}

		virtual float			GetRestitution() {return m_restitution;}

		virtual void			SetRollingFriction(float fRollingFriction) {m_rollingFriction = fRollingFriction;}

		virtual float			GetRollingFriction() {return m_rollingFriction;}
		
		virtual void			Serialize(Serializer& sl, const char* pszLable = "ICollisionMaterial");

		void					Start(btCollisionObject* pBtCollObject);

	private:
		int						m_nCollLayer;
		float					m_friction;
		float					m_restitution;
		float					m_rollingFriction;
	};


	struct  Collision
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

	class  CollisionListener
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
