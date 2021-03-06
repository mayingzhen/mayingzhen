#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"


#include "BulletPhysics/BtCollisionShape.h"
#include "BulletPhysics/BtCharacterController.h"
#include "BulletPhysics/BtPhysicsJoint.h"
#include "BulletPhysics/BtRigidBody.h"
#include "BulletPhysics/BtPhysicsSystem.h"



void BtPhysicsModuleInit();

void BtPhysicsModuleShutdown();


