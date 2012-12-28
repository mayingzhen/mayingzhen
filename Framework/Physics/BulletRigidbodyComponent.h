#ifndef __BulletRigidbodyComponent_H__
#define __BulletRigidbodyComponent_H__


namespace ma
{
	class FRAMEWORK_API BulletRigidBodyComponent : public Component
	{

	public:
		BulletRigidBodyComponent();

		~BulletRigidBodyComponent();

		virtual int		GetPriority() const;

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

		void	SyncFromScene();

		void	SyncToScene();

	private:
		int			m_collisionFlags;
		bool		m_bUseGravity;
		bool		m_bKinematic;
		btRigidBody::btRigidBodyConstructionInfo m_rbInfo;

		btRigidBody* m_pRigudBody;
	};

}
#endif //__BulletRigidbodyComponent_H__
