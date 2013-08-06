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

	class FRAMWORK_API BoxCollisionComponent : public Component
	{
		DECL_OBJECT(BoxCollisionComponent)
	public:
		BoxCollisionComponent(GameObject* pGameObj);

		IBoxCollisionShape* GetBoxCollisionShape() {return m_pBoxCollisionShape;}

		virtual void		Serialize(Serializer& sl, const char* pszLable = "BoxCollisionComponent");

		virtual	void		Render();

	private:
		IBoxCollisionShape* m_pBoxCollisionShape;
	};


	class FRAMWORK_API SphereCollisionComponent : public Component
	{
		DECL_OBJECT(SphereCollisionComponent)
	public:
		SphereCollisionComponent(GameObject* pGameObj);

		ISphereCollisionShape*	GetSphereCollisionShape() {return m_pSphereCollisionShape;}

		virtual void			Serialize(Serializer& sl, const char* pszLable = "SphereCollisionComponent");
		
		virtual	void			Render();

	private:
		ISphereCollisionShape* m_pSphereCollisionShape;
	};

	class FRAMWORK_API CapsuleCollisionComponent : public Component
	{
		DECL_OBJECT(CapsuleCollisionComponent)
	public:
		CapsuleCollisionComponent(GameObject* pGameObj);

		ICapsuleCollisionShape* GetCapsuleCollisionShape() {return m_pCapsuleCollisionShape;}

		virtual void			Serialize(Serializer& sl, const char* pszLable = "CapsuleCollisionComponent");

		virtual void			Render();

	private:
		ICapsuleCollisionShape*	m_pCapsuleCollisionShape;
	};

}

#endif
