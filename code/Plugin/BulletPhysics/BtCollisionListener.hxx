#include "BulletCharacterController.h"


namespace xm
{
	static BulletCollisionListener _btCollisionListener; 

	CollisionListener* GetBulletCollisionListener()
	{
		return &_btCollisionListener;
	}

	GameObject* GetGameEntity(btCollisionObject* pBtObj)
	{
		if (pBtObj == NULL)
			return NULL;

		if ( IsCharacterController(pBtObj) )
		{
			BulletCharacterController* pBulletCharContr = (BulletCharacterController*)pBtObj->getUserPointer();
			return pBulletCharContr->GetGameObject();
		}
		else
		{
			return (GameObject*)pBtObj->getUserPointer();
		}
	}

	void BulletCollisionListener::collisionEvent(CollisionListener::EventType type,
                                    const CollisionPair& collisionPair,
                                    const Vector3& contactPointA,
                                    const Vector3& contactPointB)
	{	
		GameObject* pGameEntityA = GetGameEntity(collisionPair.objectA);
		GameObject* pGameEntityB = GetGameEntity(collisionPair.objectB);
		ASSERT(pGameEntityA && pGameEntityB);
		if (pGameEntityA == NULL || pGameEntityB == NULL)
			return;

		Collision collDataA;
		collDataA.m_pGameEntity = pGameEntityB;
		collDataA.m_vContactsPointWS = contactPointB;

		Collision collDataB;
		collDataB.m_pGameEntity = pGameEntityB;
		collDataB.m_vContactsPointWS = contactPointB;

		if (type == COLLIDING)
		{
			pGameEntityA->OnCollisionEnter(collDataB);

			pGameEntityB->OnCollisionEnter(collDataA);

		}
		else if (type == NOT_COLLIDING)
		{
			pGameEntityA->OnCollisionExit(collDataB);

			pGameEntityB->OnCollisionExit(collDataA);
		}
    }
}



