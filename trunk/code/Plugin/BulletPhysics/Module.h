#ifndef  _BULLETPHYSICS_Moudule__H__
#define  _BULLETPHYSICS_Moudule__H__

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include "BulletPhysics/Export.h"

#include "BulletPhysics/BtCollisionShape.h"
#include "BulletPhysics/BtCharacterController.h"
#include "BulletPhysics/BtPhysicsJoint.h"
#include "BulletPhysics/BtRigidBody.h"
#include "BulletPhysics/BtPhysicsSystem.h"


BULLETPHYSICS_API void BtPhysicsModuleInit();

BULLETPHYSICS_API void BtPhysicsModuleShutdown();


#endif