#ifndef __BulletContactReport_H__
#define __BulletContactReport_H__

#include "BtCharacterController.h"
#include "BtPhysicsObject.h"

namespace ma
{
	
	class BulletContactReport
	{
	public:
		static void AddCollisionListener(BulletPhysicsObject* objectA, BulletPhysicsObject* objectB,CollisionListener* listener = NULL);

		static void RemoveCollisionListener(BulletPhysicsObject* objectA, BulletPhysicsObject* objectB,CollisionListener* listener = NULL);
		
		static void ClearCollisionListener();

		static void Update();
	};


	class CollisionCallback : public btCollisionWorld::ContactResultCallback
    {
    public:
        /**
         * Constructor.
         * 
         * @param pc The physics controller that owns the callback.
         */
        CollisionCallback(/*PhysicsController* pc*/) /*: _pc(pc)*/ {}

    protected:
        /**
            * Internal function used for Bullet integration (do not use or override).
            */
        btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* a, int partIdA, int indexA, const btCollisionObjectWrapper* b, int partIdB, int indexB);    

    private:
        //PhysicsController* _pc;
    };


	struct MyClosestRayResultCallbackCallback : public btCollisionWorld::ClosestRayResultCallback
	{
		MyClosestRayResultCallbackCallback(const btVector3&	rayFromWorld,const btVector3&	rayToWorld):
		btCollisionWorld::ClosestRayResultCallback(rayFromWorld,rayToWorld)	
		{

		}

		virtual bool needsCollision(btBroadphaseProxy* proxy0) const;

	public:
		int m_nTestLayer;
	};
}

#endif

