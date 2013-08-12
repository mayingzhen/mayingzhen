#ifndef  _BtPhysicsScene__H__
#define  _BtPhysicsScene__H__

#include "Engine/Physics/IPhysicsScene.h"


namespace ma
{
	class BulletCharacterController;
	class BulletPhysicsObject;
	class BulletPhysicsGenericJoint;
	class BulletPhysicsHingeJoint;

	class BulletScene : public IPhysicsScene
	{

	public:
		BulletScene();

		virtual bool					Start();

		virtual void					Stop();

		virtual void					SetLayerCollisionMask(Uint8 nLayer,Uint8 nColLayer,bool bCollide);

		virtual Vector3					GetGravity() const;

		virtual void					SetGravity(const Vector3& g);

		virtual void					BeginSimulation(float fDeltaTime);

		virtual void					EndSimulation();

		virtual	IPhysicsObject*			CreatePhysicsObject(GameObject* pGameObj);

		virtual	void					DeletePhysicsObject(IPhysicsObject* pPhysicsObject);

		virtual	IPhysicsGenericJoint*	CreatePhysicsGenericJoint(IPhysicsObject* pPhyObjA, IPhysicsObject* pPhyObjB);

		virtual	IPhysicsHingeJoint*		CreatePhysicsHingeJoint(IPhysicsObject* pPhyObjA, IPhysicsObject* pPhyObjB);

		virtual GameObject*				RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer, Vector3& hitPosWS);

		btDiscreteDynamicsWorld*		GetDynamicsWorld() {return m_pDynamicsWorld;}

	private:
		btDefaultCollisionConfiguration*			m_pCollisionConfiguration;
		btCollisionDispatcher*						m_pDispatcher;
		btBroadphaseInterface*						m_pOverlappingPairCache;
		btSequentialImpulseConstraintSolver*		m_pSolver;
		btDiscreteDynamicsWorld*					m_pDynamicsWorld;

		std::vector<BulletPhysicsObject*>			m_arrPhysicsObject;

		std::vector<BulletPhysicsGenericJoint*>		m_vGenericJoint;
		std::vector<BulletPhysicsHingeJoint*>		m_vHingeJoint;

		btVector3									m_vGravity;
	};
}


#endif
