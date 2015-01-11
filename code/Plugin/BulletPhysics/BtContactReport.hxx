#include "BtContactReport.h"
#include "BtCharacterController.h"
#include "BtPhysicsSystem.h"

namespace ma
{

	// Represents the collision listeners and status for a given collision pair (used by the collision status cache).
	struct CollisionInfo
	{
		enum StatusTypeMask
		{
			DIRTY         = 0x01,
			COLLISION     = 0x02,
			REGISTEREDD   = 0x04,
			REMOVE        = 0x08,
		};

		CollisionInfo() : _status(0) { }

		std::vector<CollisionListener*> _listeners;
		int _status;
	};

	class CollisionPair
	{
	public:
		CollisionPair(SceneNode* objectA, SceneNode* objectB)
		{
			m_pObjectA = objectA;
			m_pObjectB = objectB;
		}

		bool operator < (const CollisionPair& collisionPair) const
		{
			// If the pairs are equal, then return false.
			if ((m_pObjectA == collisionPair.m_pObjectA && m_pObjectB == collisionPair.m_pObjectB) || 
				(m_pObjectA == collisionPair.m_pObjectB && m_pObjectB == collisionPair.m_pObjectA))
				return false;

			// We choose to compare based on objectA arbitrarily.
			if (m_pObjectA < collisionPair.m_pObjectA)
				return true;

			if (m_pObjectA == collisionPair.m_pObjectA)
				return m_pObjectB < collisionPair.m_pObjectB;

			return false;

		}

		SceneNode* m_pObjectA;

		SceneNode* m_pObjectB;
	};



	static std::map<CollisionPair, CollisionInfo> _collisionStatus;
	static CollisionCallback _collisionCallback;

	void BulletContactReport::AddCollisionListener(SceneNode* objectA, SceneNode* objectB,CollisionListener* listener)
	{
// 		btCollisionObject* pBtObjectA = objectA->GetbtCollisionObject();
// 		btCollisionObject* pBtObjectB = objectB->GetbtCollisionObject();

		// One of the collision objects in the pair must be non-null.
		ASSERT(objectA || objectB);
		CollisionPair pair(objectA, objectB);

		// Add the listener and ensure the status includes that this collision pair is registered.
		CollisionInfo& info = _collisionStatus[pair];
		info._listeners.push_back(listener);
		info._status |= CollisionInfo::REGISTEREDD;
	}

	void BulletContactReport::RemoveCollisionListener(SceneNode* objectA, SceneNode* objectB,CollisionListener* listener)
	{
		//btCollisionObject* pBtObjectA = objectA->GetbtCollisionObject();
		//btCollisionObject* pBtObjectB = objectB->GetbtCollisionObject();

		// One of the collision objects in the pair must be non-null.
		ASSERT(objectA || objectB);
		CollisionPair pair(objectA, objectB);

		// Mark the collision pair for these objects for removal.
		if (_collisionStatus.count(pair) > 0)
		{
			_collisionStatus[pair]._status |= CollisionInfo::REMOVE;
		}
	}

	SceneNode* GetBulletPhysicsObject(const btCollisionObject* collisionObject)
	{
		ASSERT(collisionObject);
		return reinterpret_cast<SceneNode*>(collisionObject->getUserPointer());
	}

	btCollisionObject* GetBtCollisionObject(SceneNode* pGameObject)
	{
		if (pGameObject == NULL)
			return NULL;

		return (btCollisionObject*)pGameObject->GetUserData("btCollisionObject");
	}

	void BulletContactReport::ClearCollisionListener()
	{
		_collisionStatus.clear();
	}

	void BulletContactReport::Update()
	{
		btDiscreteDynamicsWorld* pDynamicsWorld = GetPhysicsSystem()->GetDynamicsWorld();
		ASSERT(pDynamicsWorld);
		if (pDynamicsWorld == NULL)
			return;

		// All statuses are set with the DIRTY bit before collision processing occurs.
		// During collision processing, if a collision occurs, the status is 
		// set to COLLISION and the DIRTY bit is cleared. Then, after collision processing 
		// is finished, if a given status is still dirty, the COLLISION bit is cleared.
		//
		// If an entry was marked for removal in the last frame, remove it now.

		// Dirty the collision status cache entries.
		std::map<CollisionPair, CollisionInfo>::iterator iter = _collisionStatus.begin();
		for (; iter != _collisionStatus.end();)
		{
			if ((iter->second._status & CollisionInfo::REMOVE) != 0)
			{
				std::map<CollisionPair, CollisionInfo>::iterator eraseIter = iter;
				iter++;
				_collisionStatus.erase(eraseIter);
			}
			else
			{
				iter->second._status |= CollisionInfo::DIRTY;
				iter++;
			}
		}

		// Go through the collision status cache and perform all registered collision tests.
		iter = _collisionStatus.begin();
		for (; iter != _collisionStatus.end(); iter++)
		{
			btCollisionObject* pBtObjectA = GetBtCollisionObject(iter->first.m_pObjectA);
			btCollisionObject* pBtObjectB = GetBtCollisionObject(iter->first.m_pObjectB);

			// If this collision pair was one that was registered for listening, then perform the collision test.
			// (In the case where we register for all collisions with a rigid body, there will be a lot
			// of collision pairs in the status cache that we did not explicitly register for.)
			if ((iter->second._status & CollisionInfo::REGISTEREDD) != 0 && (iter->second._status & CollisionInfo::REMOVE) == 0)
			{
				if (iter->first.m_pObjectB)
					pDynamicsWorld->contactPairTest(pBtObjectA, pBtObjectB, _collisionCallback);
				else
					pDynamicsWorld->contactTest(pBtObjectA, _collisionCallback);
			}
		}

		// Update all the collision status cache entries.
		iter = _collisionStatus.begin();
		for ( ; iter != _collisionStatus.end(); iter++)
		{
			if ((iter->second._status & CollisionInfo::DIRTY) != 0)
			{
				if ((iter->second._status & CollisionInfo::COLLISION) != 0 && iter->first.m_pObjectB)
				{
					size_t size = iter->second._listeners.size();
					for (size_t i = 0; i < size; i++)
					{
						CollisionListener::CollisionData eventData;
						eventData.m_eType = CollisionListener::NOT_COLLIDING;
						eventData.m_pObjectA = iter->first.m_pObjectA;
						eventData.m_pObjectB = iter->first.m_pObjectB;

						iter->second._listeners[i]->collisionEvent(eventData);
					}
				}

				iter->second._status &= ~CollisionInfo::COLLISION;
			}
		}
	}

	btScalar CollisionCallback::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* a, int partIdA, int indexA, 
		const btCollisionObjectWrapper* b, int partIdB, int indexB)
	{
		// Get pointers to the PhysicsCollisionObject objects.
		SceneNode* objectA = GetBulletPhysicsObject(a->m_collisionObject);
		SceneNode* objectB = GetBulletPhysicsObject(b->m_collisionObject);

		// If the given collision object pair has collided in the past, then
		// we notify the listeners only if the pair was not colliding
		// during the previous frame. Otherwise, it's a new pair, so add a
		// new entry to the cache with the appropriate listeners and notify them.
		CollisionPair pair(objectA, objectB);

		CollisionInfo* collisionInfo;
		if (_collisionStatus.count(pair) > 0)
		{
			collisionInfo = &_collisionStatus[pair];
		}
		else
		{
			CollisionPair p1(pair.m_pObjectA, NULL);
			CollisionPair p2(pair.m_pObjectB, NULL);
			if (_collisionStatus.count(p1) > 0)
			{
				collisionInfo = &_collisionStatus[p1];
			}
			else if (_collisionStatus.count(p2) > 0)
			{
				collisionInfo = &_collisionStatus[p2];
			}
		}

		// Fire collision event.
		if ((collisionInfo->_status & CollisionInfo::COLLISION) == 0)
		{
			std::vector<CollisionListener*>::const_iterator iter = collisionInfo->_listeners.begin();
			for (; iter != collisionInfo->_listeners.end(); iter++)
			{
				ASSERT(*iter);
				if ((collisionInfo->_status & CollisionInfo::REMOVE) == 0)
				{
					CollisionListener::CollisionData eventData;
					eventData.m_eType = CollisionListener::NOT_COLLIDING;
					eventData.m_pObjectA = pair.m_pObjectA;
					eventData.m_pObjectB = pair.m_pObjectB;
					eventData.m_vContactPointA = ToMaUnit( cp.getPositionWorldOnA() );
					eventData.m_vContactPointB = ToMaUnit( cp.getPositionWorldOnB() );
					eventData.m_vContactNoramlA = ToMaUnit(-cp.m_normalWorldOnB);
					eventData.m_vContactNoramlB = ToMaUnit(cp.m_normalWorldOnB);

					(*iter)->collisionEvent(eventData);
				}
			}
		}

		// Update the collision status cache (we remove the dirty bit
		// set in the controller's update so that this particular collision pair's
		// status is not reset to 'no collision' when the controller's update completes).
		collisionInfo->_status &= ~CollisionInfo::DIRTY;
		collisionInfo->_status |= CollisionInfo::COLLISION;
		return 0.0f;
	}



	bool MyClosestRayResultCallbackCallback::needsCollision(btBroadphaseProxy* proxy0) const
	{
		if (proxy0 == NULL)
			return false;

		if ( !btCollisionWorld::ClosestRayResultCallback::needsCollision(proxy0) )
			return false;

		btCollisionObject* pbtCollObj = (btCollisionObject*)proxy0->m_clientObject;
		
		SceneNode* pBulletPhysObj = (SceneNode*)pbtCollObj->getUserPointer();
		CollisionMaterial* pMaterial = pBulletPhysObj->GetTypeComponent<CollisionMaterial>();
		if (pBulletPhysObj && pMaterial && pMaterial->GetCollLayer() == m_nTestLayer)
		{
			return true;
		}

		return false;
	}

}

