#ifndef __BulletRigidBody_H__
#define __BulletRigidBody_H__

#include "Engine/Physics/IRigidBody.h"

class btRigidBody;
class btCompoundShape;


namespace ma
{
	class BULLETPHYSICS_API BulletRigidBody : public IRigidBody
	{
	public:
		BulletRigidBody();

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

		btRigidBody*			CreatebtRigidBody(btCompoundShape* pCompoundShape,btDiscreteDynamicsWorld* pm_pBtDynamicsWorld); 

		void					SetbtRigidBody(btRigidBody* pbtRigidBody); 
	
	private:
		btRigidBody*   m_pRigidBody;

		bool		   m_bUseGravity;
		bool		   m_bKinematic;
		btRigidBody::btRigidBodyConstructionInfo* m_rbInfo;

		btDiscreteDynamicsWorld* m_pBtDynamicsWorld;
	};


}

#endif