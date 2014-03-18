#ifndef __IPhysicsCollisionShape_H__
#define __IPhysicsCollisionShape_H__


namespace ma
{
	class ENGINE_API ICollisionShape : public Component
	{
		DECL_OBJECT(ICollisionShape)

	public:
		ICollisionShape(GameObject* pGameObj);

		virtual void			SetTransformLS(const NodeTransform& tsfLS) = 0;

		virtual NodeTransform	GetTransformLS() = 0;
	};

	class ENGINE_API IBoxCollisionShape : public ICollisionShape
	{
		DECL_OBJECT(IBoxCollisionShape)

	public:
		IBoxCollisionShape(GameObject* pGameObj);

		virtual void			SetSize(const Vector3& vSize) = 0;

		virtual	Vector3			GetSize() = 0;
	};

	class ENGINE_API ISphereCollisionShape : public ICollisionShape
	{
		DECL_OBJECT(ISphereCollisionShape)

	public:
		ISphereCollisionShape(GameObject* pGameObj);

		virtual void			SetRadius(float fRadius) = 0;

		virtual	float			GetRadius() = 0;
	};

	class ENGINE_API ICapsuleCollisionShape : public ICollisionShape
	{
		DECL_OBJECT(ICapsuleCollisionShape)

	public:
		ICapsuleCollisionShape(GameObject* pGameObj);

		virtual void			SetHeight(float fHeight) = 0;

		virtual float			GetHeight() const = 0;

		virtual void			SetRadius(float fRadius) = 0;

		virtual float			GetRadius() const = 0;
	};


	DeclareRefPtr(IBoxCollisionShape);
	DeclareRefPtr(ISphereCollisionShape);
	DeclareRefPtr(ICapsuleCollisionShape);

	struct ENGINE_API Collision
	{
		GameObject* m_pGameEntity;
		Vector3	m_vContactsPointWS;
		Vector3	m_vContactsNormalWS;

		Collision()
		{
			m_pGameEntity = NULL;
			m_vContactsPointWS = Vector3(0,0,0);
			m_vContactsNormalWS = Vector3(0,0,0);
		}
	};
}

#endif

