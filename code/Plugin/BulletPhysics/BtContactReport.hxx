#include "BtContactReport.h"
#include "BtCharacterController.h"
#include "BtPhysicsObject.h"

namespace ma
{
	void BulletContactReport(btDynamicsWorld* sDynamicsWorld)
	{
		int numManifolds = sDynamicsWorld->getDispatcher()->getNumManifolds();
		for(int i=0;i<numManifolds;i++)
		{
			btPersistentManifold * contactManifold = sDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			int numContacts= contactManifold->getNumContacts();
			if (numContacts <= 0)
				continue;

			const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
			const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
			
			GameObject* pGameObjA = NULL;
			GameObject* pGameObjB = NULL;
			if ( IsCharacterController(obA) )
			{
				BulletCharacterController* pCharControl = (BulletCharacterController*)obA->getUserPointer();
				pGameObjA = pCharControl->GetGameObject();
			}
			else
			{
				BulletPhysicsObject* pPhysicsObj = (BulletPhysicsObject*)obB->getUserPointer();
				pGameObjA = pPhysicsObj->GetGameObject();
			}

			if ( IsCharacterController(obB) )
			{
				BulletCharacterController* pCharControl = (BulletCharacterController*)obB->getUserPointer();
				pGameObjB = pCharControl->GetGameObject();
			}
			else
			{
				BulletPhysicsObject* pPhysicsObj = (BulletPhysicsObject*)obB->getUserPointer();
				pGameObjB = pPhysicsObj->GetGameObject();
			}
			
			if (pGameObjA)
			{
				//pGameObjA->OnContactNotify(pGameObjB);
			}

			if (pGameObjB)
			{
				//pGameObjB->OnContactNotify(pGameObjA);
			}
		}
	}

	MyClosestRayResultCallbackCallback::MyClosestRayResultCallbackCallback(const btVector3&	rayFromWorld,const btVector3&	rayToWorld):
	btCollisionWorld::ClosestRayResultCallback(rayFromWorld,rayToWorld)	
	{

	}

	bool MyClosestRayResultCallbackCallback::needsCollision(btBroadphaseProxy* proxy0) const
	{
		if (proxy0 == NULL)
			return false;

		if ( !btCollisionWorld::ClosestRayResultCallback::needsCollision(proxy0) )
			return false;

		btCollisionObject* pbtCollObj = (btCollisionObject*)proxy0->m_clientObject;
		if ( IsCharacterController(pbtCollObj) )
		{
			BulletCharacterController* pBulletCharContr = (BulletCharacterController*)pbtCollObj->getUserPointer();
			if (m_nTestLayer == pBulletCharContr->GetColllLayer())
			{
				return true;
			}
		}
		else
		{
			BulletPhysicsObject* pPhysicsObject = (BulletPhysicsObject*)pbtCollObj->getUserPointer();
			if (pPhysicsObject && pPhysicsObject->GetCollLayer() == m_nTestLayer)
			{
				return true;
			}
		}

		return false;
	}
}

