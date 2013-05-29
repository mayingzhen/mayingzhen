#ifndef __BulletContactReport_H__
#define __BulletContactReport_H__

class btDynamicsWorld;

namespace ma
{
	void BulletContactReport(btDynamicsWorld* sDynamicsWorld);


	struct MyClosestRayResultCallbackCallback : public btCollisionWorld::ClosestRayResultCallback
	{
		MyClosestRayResultCallbackCallback(const btVector3&	rayFromWorld,const btVector3&	rayToWorld);

		virtual bool needsCollision(btBroadphaseProxy* proxy0) const;
	public:
		int m_nTestLayer;
	};
}

#endif


