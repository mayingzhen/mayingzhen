#ifndef __Bullet_PhysicsObject_H__
#define __Bullet_PhysicsObject_H__

#include "Framework/Physics/IPhysicsObject.h"

class btRigidBody;

namespace ma
{
	class IRigidBody;
	class IBoxCollisionShape;
	class ISphereCollisionShape;
	
	class BULLETPHYSICS_API BulletPhysicsObject : public IPhysicsObject
	{
	public:
		BulletPhysicsObject();

		virtual bool			Start(GameObject* pOwner);

		virtual	bool			Stop(GameObject* pOwner);

		virtual	IRigidBody*		GetRigidBody();

		virtual void			SetRigidBody(IRigidBody* pRigidBody);

		virtual void			AddBoxCollisionShape(IBoxCollisionShape* pCollisionShape);

		virtual	void			AddSphereCollisionShape(ISphereCollisionShape* pSphereCollisionShape);

		virtual void			SetTransformWS(const maNodeTransform& tsfWS);

		virtual maNodeTransform GetTransformWS();

		virtual	void			ApplyForce(D3DXVECTOR3 vForce);

		int						GetCollLayer() {return m_nCollLayer;}

	private:
		btRigidBody*						m_pbtRigidBody;
		

		IRigidBody*							m_pXmRigidBody;
		std::vector<IBoxCollisionShape*>	m_vBoxCollisionShape;
		std::vector<ISphereCollisionShape*>	m_vSphereCollisionShape;

		maNodeTransform m_tsfWS;

		int m_nCollLayer;
	};
}

#endif

