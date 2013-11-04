#ifndef __Bullet_PhysicsObject_H__
#define __Bullet_PhysicsObject_H__

namespace ma
{
	class BulletRigidBody;
	class BulletBoxCollisionShape;
	class BulletSphereCollisionShape;
	class BulletPhysicsGenericJoint;
	class BulletPhysicsHingeJoint;
	class CollisionListener;
	
	class BulletPhysicsObject : public IPhysicsObject
	{
	public:
		BulletPhysicsObject(GameObject*	pGameObject);

		~BulletPhysicsObject();

		virtual	IRigidBody*				CreateRigidBody();

		virtual IBoxCollisionShape*		CreateBoxCollisionShape();

		virtual ISphereCollisionShape*	CreateSphereCollisionShape();

		virtual ICapsuleCollisionShape* CreateCapsuleCollisionShape();

		virtual ICharaControll*			CreateCharaControll();

		virtual CollisionMaterial*		GetCollisionMaterial();

		virtual void					SetTransformWS(const NodeTransform& tsfWS);

		virtual NodeTransform			GetTransformWS();

		virtual void					AddCollisionListener(IPhysicsObject* objectB,CollisionListener* pListener);

		virtual void					AddCollisionListener(ICharaControll* objectB);

		bool							Start();

		bool							Stop();

		void							SyncToPhysics();

		void							SyncFromPhysics();

		GameObject*						GetGameObject() {return m_pGameObject;}

		btCollisionObject*				GetbtCollisionObject() {return m_pBtCollObject;}

		void							DebugRender();

	private:
		void							InitCharControll();

		void							InitCollObject();
		
		void							InitMaterial();

	private:
		btCollisionObject*							m_pBtCollObject;
		
		BulletRigidBody*							m_pRigidBody;
		std::vector<BulletBoxCollisionShape*>		m_vBoxCollisionShape;
		std::vector<BulletSphereCollisionShape*>	m_vSphereCollisionShape;
		std::vector<BulletCapsuleCollisionShape*>	m_vCapsuleCollisionShape;

		BulletCharacterController*					m_pCharaControll;

		NodeTransform								m_tsfWS;

		//int											m_nCollLayer;
		CollisionMaterial							m_material;

		GameObject*									m_pGameObject;
	};
}

#endif

