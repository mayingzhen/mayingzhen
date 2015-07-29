#ifndef __Bullet_CollisionShape_H__
#define __Bullet_CollisionShape_H__


namespace ma
{
	class CollisionShape : public Component
	{
	
	public:
		DECL_OBJECT(CollisionShape);

		static void			RegisterAttribute();

		const Transform&	GetTransformLS() const {return m_tsfLS;} 
		void				SetTransformLS(const Transform& tsfLS); 

		const Vector3&		GetPosLS() const {return m_tsfLS.m_vPos;}
		void				SetPosLS(const Vector3& vPos);

		const Quaternion&	GetRotateLS() const {return m_tsfLS.m_qRot;}
		void				SetRotateLS(const Quaternion& qRot);

		const Vector3&		GetScaleLS() const {return m_tsfLS.m_vScale;}
		void				SetScaleLS(const Vector3& vScale);

		virtual void*		Create() {return NULL;}

		virtual	void		OnAddToSceneNode(SceneNode* pGameObj);

	protected: 
		Transform			m_tsfLS;

		btCollisionShape*	m_pBtShape; 
	};

	class BoxCollisionShape : public CollisionShape
	{	
	
	public:	
		BoxCollisionShape();

		~BoxCollisionShape();
		
		DECL_OBJECT(BoxCollisionShape);

		static void				RegisterAttribute();

		void					SetSize(const Vector3& vSize);

		const Vector3&			GetSize() const;

		virtual	void*			Create();

	private:

		Vector3					m_vSize;
	};

	
	class SphereCollisionShape : public CollisionShape
	{
		
	public:	
		SphereCollisionShape();

		~SphereCollisionShape();

		DECL_OBJECT(SphereCollisionShape);

		static void				RegisterAttribute();

		virtual void			SetRadius(float fRadius);

		virtual	float			GetRadius() const; 
		
		virtual	void*			Create();

	private:

		float					m_fRadius;
	};

	class CapsuleCollisionShape : public CollisionShape
	{
		
	public:
		CapsuleCollisionShape();	

		~CapsuleCollisionShape();

		DECL_OBJECT(CapsuleCollisionShape);

		static void				RegisterAttribute();

		virtual void			SetHeight(float fHeight);

		virtual float			GetHeight() const;

		virtual void			SetRadius(float fRadius);

		virtual float			GetRadius() const;

		virtual	void*			Create();

	private:
		float					m_fHeight;
		float					m_fRadius;
	};	

	class CollisionMaterial : public Component
	{
	
	public:
		CollisionMaterial();

		DECL_OBJECT(CollisionMaterial)

		static void		RegisterAttribute();

		void			SetCollLayer(int nLayer) {m_nCollLayer = nLayer;}
		int				GetCollLayer() const {return m_nCollLayer;}

		void			SetFriction(float fFriction) {m_friction = fFriction;}
		float			GetFriction() const {return m_friction;}

		void			SetRestitution(float fRestitution) {m_restitution = fRestitution;}
		float			GetRestitution() const {return m_restitution;}

		void			SetRollingFriction(float fRollingFriction) {m_rollingFriction = fRollingFriction;}
		float			GetRollingFriction() const {return m_rollingFriction;}

		void			Start(btCollisionObject* pBtCollObject);

	private:
		int				m_nCollLayer;
		float			m_friction;
		float			m_restitution;
		float			m_rollingFriction;
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

	class CollisionListener
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
