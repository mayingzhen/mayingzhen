#ifndef __RigidbodyComponent_H__
#define __RigidbodyComponent_H__

#include "Component.h"

namespace ma
{

	class IRigidBody;
	class IPhysicsObject;

	class ENTITYSYSTEM_API RigidBodyComponent : public Component
	{
		DECL_OBJECT(RigidBodyComponent)
	public:
		RigidBodyComponent(GameObject* pGameObj);

		~RigidBodyComponent();

		IRigidBody*		GetRigidBody() {return m_pRigidBody;}

		virtual void	Serialize(Serializer& sl, const char* pszLable = "RigidBodyComponent");

	protected:
		IRigidBody*			m_pRigidBody;

		IPhysicsObject*		m_pPhysicsObject;
	};
}
#endif //__RigidbodyComponent_H__

