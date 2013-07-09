#ifndef __Bullet_PhysicsObject_H__
#define __Bullet_PhysicsObject_H__

#include "Engine/Physics/IPhysicsObject.h"

class btRigidBody;

namespace ma
{
	class IRigidBody;
	class IBoxCollisionShape;
	class ISphereCollisionShape;
	class BulletScene;
	
	class BulletPhysicsObject : public IPhysicsObject
	{
	public:
		BulletPhysicsObject(GameObject*	pGameObject,BulletScene* pPhyScene);

		virtual	IRigidBody*				CreateRigidBody();

		virtual IBoxCollisionShape*		CreateBoxCollisionShape();

		virtual ISphereCollisionShape*	CreateSphereCollisionShape();

		virtual void					SetTransformWS(const NodeTransform& tsfWS);

		virtual NodeTransform			GetTransformWS();

		virtual	void					ApplyForce(Vector3 vForce);

		bool							Start();

		bool							Stop();

		void							SyncToPhysics();

		void							SyncFromPhysics();

		GameObject*						GetGameObject() {return m_pGameObject;}

		int								GetCollLayer() {return m_nCollLayer;}

	private:
		btRigidBody*						m_pbtRigidBody;
		

		IRigidBody*							m_pXmRigidBody;
		std::vector<IBoxCollisionShape*>	m_vBoxCollisionShape;
		std::vector<ISphereCollisionShape*>	m_vSphereCollisionShape;

		NodeTransform						m_tsfWS;

		int									m_nCollLayer;

		GameObject*							m_pGameObject;
		BulletScene*						m_pPhyScene;
	};
}

#endif

