#ifndef  _BULLETPHYSICS_Moudule__H__
#define  _BULLETPHYSICS_Moudule__H__

#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/BulletDynamics/Character/btKinematicCharacterController.h"
#include "Bullet/BulletCollision/CollisionDispatch/btGhostObject.h"

#include "BulletPhysics/Export.h"
#include "BulletPhysics/BtPhysicsDevive.h"
#include "BulletPhysics/BtPhysicsObject.h"
#include "BulletPhysics/BtPhysicsScene.h"
#include "BulletPhysics/BtCollisionShape.h"
#include "BulletPhysics/BtRigidBody.h"
#include "BulletPhysics/BtCharacterController.h"
#include "BulletPhysics/BtContactReport.h"

#include "Framework/Module.h"


BULLETPHYSICS_API void BtPhysicsModuleInit();

BULLETPHYSICS_API void BtPhysicsModuleShutdown();


#endif
