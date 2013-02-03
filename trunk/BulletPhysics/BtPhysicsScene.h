#ifndef  _BtPhysicsScene__H__
#define  _BtPhysicsScene__H__

#include "Framework/Physics/IPhysicsScene.h"


class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace ma
{
	class BULLETPHYSICS_API BulletScene : public IPhysicsScene
	{

	public:
		BulletScene();

		virtual bool		Start();

		virtual void		Stop();

		virtual void		SetLayerCollisionMask(xmUint8 nLayer,xmUint8 nColLayer,bool bCollide);

		virtual D3DXVECTOR3	GetGravity() const;

		virtual void		SetGravity(const D3DXVECTOR3& g);

		virtual void		BeginSimulation(float fDeltaTime);

		virtual void		EndSimulation();

		virtual GameObject* RayCastCloseGameObj(const D3DXVECTOR3& rayOrig, const D3DXVECTOR3& rayDir, int nCollLayer, D3DXVECTOR3& hitPosWS);

		btDiscreteDynamicsWorld* GetDynamicsWorld() {return m_pDynamicsWorld;}

	private:
		btDefaultCollisionConfiguration*	 m_pCollisionConfiguration;
		btCollisionDispatcher*				 m_pDispatcher;
		btBroadphaseInterface*				 m_pOverlappingPairCache;
		btSequentialImpulseConstraintSolver* m_pSolver;
		btDiscreteDynamicsWorld*			 m_pDynamicsWorld;
	};

	inline btDiscreteDynamicsWorld* GetbtDiscreteDynamicsWorld(GameObject* pGameObj)
	{
		if (pGameObj == NULL)
			return NULL;

		Scene* pScene = pGameObj->GetScene();
		if (pScene == NULL)
			return NULL;

		BulletScene* pBtScene = (BulletScene*)pScene->GetPhysicsScene();
		if (pBtScene == NULL)
			return NULL;

		return pBtScene->GetDynamicsWorld();
	}
}


#endif
