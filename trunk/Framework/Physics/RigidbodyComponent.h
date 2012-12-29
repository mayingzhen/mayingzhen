#ifndef __RigidbodyComponent_H__
#define __RigidbodyComponent_H__


namespace ma
{
	class FRAMEWORK_API RigidBodyComponent : public Component
	{
	public:
		RigidBodyComponent();

		~RigidBodyComponent();

		//virtual int		GetPriority() const;

		//------------------------------------------------------------------------------
		//RigidBody Desc
		//------------------------------------------------------------------------------
		void SetUseGravity(bool bUseGravity);

		bool IsUseGravity();

		void SetKinematic(bool bKinematic);

		bool IsKinematic();

		void SetMass(float fMass);

		float GetMass();

		void SetLinearDamping(float fLinearDamping);

		float GetLinearDamping();

		void SetAngularDamping(float fAngularDamping);

		float GetAngularDamping();

		//void AddCollisionShape(CollisionShape* pCollisionShape);

		void	SyncFromScene();

		void	SyncToScene();

		virtual void Update();

		virtual void Start();

		virtual void Stop();
	private:
		RigidBodyInfo m_rbInfo;

		IPhysicsObject* m_pPhysicsObj;
	};

}
#endif //__RigidbodyComponent_H__
