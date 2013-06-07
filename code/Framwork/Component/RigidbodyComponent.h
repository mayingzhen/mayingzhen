#ifndef __RigidbodyComponent_H__
#define __RigidbodyComponent_H__

#include "Component.h"

namespace ma
{

	class IRigidBody;
	class IPhysicsObject;

	class FRAMWORK_API RigidBodyComponent : public Component
	{
		DECL_OBJECT(RigidBodyComponent)
	public:
		RigidBodyComponent();

		~RigidBodyComponent();

		IRigidBody*		GetRigidBody() {return m_pRigidBody;}

		virtual void	SetGameObject(GameObject* pGameObj);

	protected:
		IRigidBody*			m_pRigidBody;

		IPhysicsObject*		m_pPhysicsObject;
	};
}
#endif //__RigidbodyComponent_H__

