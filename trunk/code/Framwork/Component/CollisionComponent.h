#ifndef __CollisionComponent_H__
#define __CollisionComponent_H__

#include "Component.h"

namespace ma
{
	class ICollisionShape;
	class IBoxCollisionShape;
	class ISphereCollisionShape;
	class IPhysicsObject;

	enum CollisionLayer
	{
		ColLayer_Default = 1,
		ColLayer_Terrain = 2,
	};


	class FRAMWORK_API CollisionComponent : public Component
	{
		DECL_OBJECT(CollisionComponent)
	public:
		CollisionComponent(GameObject* pGameObj); 

		//virtual ~CollisionComponent();

		NodeTransform GetTransformLS() const; 

		void SetTransformLS(const NodeTransform& tsfLS); 

		void SetCollisionLayer(int eCollLayer);

		int	GetCollisionLayer();

		virtual void	Serialize(Serializer& sl, const char* pszLable = "CollisionComponent");

	protected:
		Matrix4x4 GetWorldMatrix();

	protected:
		ICollisionShape* m_pCollisionShape;

		IPhysicsObject* m_pPhysicsObject;
	};


	class FRAMWORK_API BoxCollisionComponent : public CollisionComponent
	{
		DECL_OBJECT(BoxCollisionComponent)
	public:
		BoxCollisionComponent(GameObject* pGameObj);

		void		SetSize(const Vector3& vSize);

		Vector3	GetSize() const; 

		virtual void Render();

		virtual void DbgRender(BitField flag);

		virtual void	Serialize(Serializer& sl, const char* pszLable = "BoxCollisionComponent");

	private:
		IBoxCollisionShape* m_pBoxCollisionShape;
	};


	class FRAMWORK_API SphereCollisionComponent : public CollisionComponent
	{
		DECL_OBJECT(SphereCollisionComponent)
	public:
		SphereCollisionComponent(GameObject* pGameObj);

		void		SetRadius(float fRadius);

		float		GetRadius() const;

		virtual void DbgRender(BitField flag);

		virtual void	Serialize(Serializer& sl, const char* pszLable = "SphereCollisionComponent");

	private:
		ISphereCollisionShape* m_pSphereCollisionShape;
	};

}

#endif
