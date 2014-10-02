#ifndef __BulletRigidBody_H__
#define __BulletRigidBody_H__


class btRigidBody;

namespace ma
{
	class BulletRigidBody : public IRigidBody
	{
	public:
		BulletRigidBody(SceneNode* pGameObj);

		virtual void			SetUseGravity(bool bUseGravity);

		virtual bool			IsUseGravity();

		virtual bool			IsKinematic();

		virtual	void			SetKinematic(bool bKinematic);

		virtual void			SetMass(float fMass);

		virtual float			GetMass();

		virtual void			SetLinearDamping(float fLinearDamping);

		virtual float			GetLinearDamping();

		virtual void			SetAngularDamping(float fAngularDamping);

		virtual float			GetAngularDamping();

		virtual	void			SetDeactivationTime(float fTime);

		virtual	void			SetSleepingThresholds(float fLinear,float fAngular);

		virtual	void			ApplyForce(Vector3 vForce);

		virtual void			Serialize(Serializer& sl, const char* pszLable = "IRigidBody");

		btRigidBody*			CreatebtRigidBody(btCompoundShape* pCompoundShape,btDiscreteDynamicsWorld* pm_pBtDynamicsWorld); 

		void					SetbtRigidBody(btRigidBody* pbtRigidBody); 

		btRigidBody*			GetbtRigidBody() {return m_pRigidBody;}
	
	private:
		btRigidBody*   m_pRigidBody;

		bool		   m_bUseGravity;
		bool		   m_bKinematic;
		float		   m_fDeactivationTime;
		btRigidBody::btRigidBodyConstructionInfo* m_rbInfo;
	};


}

#endif