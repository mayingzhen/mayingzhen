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

		virtual	IPhysicsGenericJoint*	CreatePhysicsGenericJoint(IPhysicsObject* pPhyObjB);

		virtual	IPhysicsHingeJoint*		CreatePhysicsHingeJoint(IPhysicsObject* pPhyObjB);

		virtual void					SetTransformWS(const NodeTransform& tsfWS);

		virtual NodeTransform			GetTransformWS();

		bool							Start();

		bool							Stop();

		void							SyncToPhysics();

		void							SyncFromPhysics();

		GameObject*						GetGameObject() {return m_pGameObject;}

		btCollisionObject*				GetbtCollisionObject() {return m_pBtCollObject;}

		int								GetCollLayer() {return m_nCollLayer;}

	private:
		void							InitCollObject();
		
		void							InitJoint();

	private:
		btCollisionObject*					m_pBtCollObject;
		
		IRigidBody*							m_pRigidBody;
		std::vector<IBoxCollisionShape*>	m_vBoxCollisionShape;
		std::vector<ISphereCollisionShape*>	m_vSphereCollisionShape;

		std::vector<IPhysicsGenericJoint*>	m_vGenericJoint;
		std::vector<IPhysicsHingeJoint*>	m_vHingeJoint;

		NodeTransform						m_tsfWS;

		int									m_nCollLayer;

		GameObject*							m_pGameObject;
		BulletScene*						m_pPhyScene;
	};
}

#endif

