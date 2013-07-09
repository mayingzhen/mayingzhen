#ifndef  _BtPhysicsScene__H__
#define  _BtPhysicsScene__H__

#include "Engine/Physics/IPhysicsScene.h"


class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace ma
{
	class BulletCharacterController;
	class BulletPhysicsObject;

	class BulletScene : public IPhysicsScene
	{

	public:
		BulletScene();

		virtual bool			Start();

		virtual void			Stop();

		virtual void			SetLayerCollisionMask(Uint8 nLayer,Uint8 nColLayer,bool bCollide);

		virtual Vector3			GetGravity() const;

		virtual void			SetGravity(const Vector3& g);

		virtual void			BeginSimulation(float fDeltaTime);

		virtual void			EndSimulation();

		virtual	IPhysicsObject*	CreatePhysicsObject(GameObject* pGameObj);

		virtual	ICharacterController* CreateCharacterController(GameObject* pGameObj);

		virtual GameObject*		RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer, Vector3& hitPosWS);

		btDiscreteDynamicsWorld* GetDynamicsWorld() {return m_pDynamicsWorld;}

	private:
		btDefaultCollisionConfiguration*	 m_pCollisionConfiguration;
		btCollisionDispatcher*				 m_pDispatcher;
		btBroadphaseInterface*				 m_pOverlappingPairCache;
		btSequentialImpulseConstraintSolver* m_pSolver;
		btDiscreteDynamicsWorld*			 m_pDynamicsWorld;

		std::vector<BulletPhysicsObject*>	 m_arrPhysicsObject;

		std::vector<BulletCharacterController*>	m_arrCharControl;

		btVector3 m_vGravity;
	};
}


#endif
