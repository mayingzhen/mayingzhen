#ifndef __RigidbodyComponent_H__
#define __RigidbodyComponent_H__


namespace ma
{

	class IRigidBody;
	class IPhysicsObject;

	class FRAMEWORK_API RigidBodyComponent : public Component
	{
	public:
		RigidBodyComponent();

		~RigidBodyComponent();

		void SetUseGravity(bool bUseGravity);

		bool IsUseGravity();

		void SetKinematic(bool bKinematic);

		bool IsKinematic();

		void	SetMass(float fMass);

		float	GetMass();

		void	SetLinearDamping(float fLinearDamping);

		float	GetLinearDamping();

		void	SetAngularDamping(float fAngularDamping);

		float	GetAngularDamping();

		virtual void SetGameObject(GameObject* pGameObj);

	protected:
		IRigidBody*			m_pRigidBody;

		IPhysicsObject*		m_pPhysicsObject;
	};
}
#endif //__RigidbodyComponent_H__

