#ifndef __Bullet_CollisionListener_H__
#define __Bullet_CollisionListener_H__

#include "BulletContactReport.h"

namespace xm
{
	class BulletCollisionListener : public CollisionListener
    {
    public:

        virtual void collisionEvent(CollisionListener::EventType type,
                                    const CollisionPair& collisionPair,
                                    const Vector3& contactPointA,
                                    const Vector3& contactPointB);
    };

	
	CollisionListener* GetBulletCollisionListener();

}

#endif //__Bullet_CollisionListener_H__

