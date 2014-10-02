#ifndef __Bullet_CollisionShape_H__
#define __Bullet_CollisionShape_H__


namespace ma
{

#define  DECL_CollisionShape \
	public : \
		virtual Transform	GetTransformLS() {return m_tsfLS;} \
		virtual void			SetTransformLS(const Transform& tsfLS)\
		{\
			m_tsfLS = tsfLS;\
		}\
	protected: \
		Transform			m_tsfLS;\
		btCollisionShape*	m_pBtShape; \
    private: 

	class BulletBoxCollisionShape : public IBoxCollisionShape
	{	
		DECL_CollisionShape

	public:	
		BulletBoxCollisionShape(SceneNode* pGameObj);

		~BulletBoxCollisionShape();

		virtual void			SetSize(const Vector3& vSize);

		virtual	Vector3			GetSize(); 

		virtual void			Serialize(Serializer& sl, const char* pszLable = "IBoxCollisionShape");

		virtual	void*			Create();

	private:

		Vector3					m_vSize;
	};

	
	class BulletSphereCollisionShape : public ISphereCollisionShape
	{
		DECL_CollisionShape

	public:	
		BulletSphereCollisionShape(SceneNode* pGameObj);

		~BulletSphereCollisionShape();

		virtual void			SetRadius(float fRadius);

		virtual	float			GetRadius(); 

		virtual void			Serialize(Serializer& sl, const char* pszLable = "ISphereCollisionShape");
		
		virtual	void*			Create();

	private:

		float					m_fRadius;
	};

	class BulletCapsuleCollisionShape : public ICapsuleCollisionShape
	{
		DECL_CollisionShape

	public:
		BulletCapsuleCollisionShape(SceneNode* pGameObj);	

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

	class BulletCollisionMaterial : public ICollisionMaterial
	{
		DECL_OBJECT(BulletCollisionMaterial)

	public:
		BulletCollisionMaterial(SceneNode* pGameObj);

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

}


#endif
