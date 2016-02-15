#ifndef __BulletRigidBody_H__
#define __BulletRigidBody_H__


class btRigidBody;

namespace ma
{
	class RigidBody : public Component
	{

	public:
		RigidBody();

		DECL_OBJECT(RigidBody)

		static void				RegisterAttribute();

		virtual void			SetUseGravity(bool bUseGravity);
		virtual bool			IsUseGravity() const;
	
		virtual	void			SetKinematic(bool bKinematic);
		virtual bool			IsKinematic() const;

		virtual void			SetMass(float fMass);
		virtual float			GetMass() const;

		virtual void			SetLinearDamping(float fLinearDamping);
		virtual float			GetLinearDamping() const;

		virtual void			SetAngularDamping(float fAngularDamping);
		virtual float			GetAngularDamping() const;

		virtual	void			SetDeactivationTime(float fTime);

		virtual	void			SetSleepingThresholds(float fLinear,float fAngular);

		virtual	void			ApplyForce(Vector3 vForce);

		btRigidBody*			CreatebtRigidBody(btCompoundShape* pCompoundShape,btDiscreteDynamicsWorld* pm_pBtDynamicsWorld); 

		void					SetbtRigidBody(btRigidBody* pbtRigidBody); 

		btRigidBody*			GetbtRigidBody() {return m_pRigidBody;}
	
	private:
		btRigidBody*   m_pRigidBody;

		bool		   m_bUseGravity;
		bool		   m_bKinematic;
		float		   m_fDeactivationTime;
		btRigidBody::btRigidBodyConstructionInfo m_rbInfo;
	};


}

#endif