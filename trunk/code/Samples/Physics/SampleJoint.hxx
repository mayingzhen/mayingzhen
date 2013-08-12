#include "SampleJoint.h"

namespace ma
{
	SampleJoint::SampleJoint()
	{
	}

	void SampleJoint::Load()
	{
	
		GetPhysicsSystem()->SetGravity(Vector3(0,-0.98f * 10,0));

		Vector3 vPosA(20,40,0);
		Vector3 vPosB(0,40,0);

		GameObject* pGameObjA = NULL;
		{
			pGameObjA = GetEntitySystem()->CreateGameObject("physicsA");

			BoxCollisionComponent* pBoxCollisionComp = pGameObjA->CreateComponent<BoxCollisionComponent>();
			pBoxCollisionComp->GetBoxCollisionShape()->SetSize(Vector3(5,5,5));

			RigidBodyComponent* pRigidBodyComp = pGameObjA->CreateComponent<RigidBodyComponent>();
			//pRigidBodyComp->GetRigidBody()->SetUseGravity(false);
			pRigidBodyComp->GetRigidBody()->SetKinematic(true);

			pGameObjA->GetSceneNode()->Translate(vPosA);

		}

		GameObject* pGameObjB = NULL;
		{
			pGameObjB = GetEntitySystem()->CreateGameObject("physicsB");

			BoxCollisionComponent* pBoxCollisionShape = pGameObjB->CreateComponent<BoxCollisionComponent>();
			pBoxCollisionShape->GetBoxCollisionShape()->SetSize(Vector3(5,5,5));
		
			RigidBodyComponent* pRigidBodyComp = pGameObjB->CreateComponent<RigidBodyComponent>();

			pGameObjB->GetSceneNode()->Translate(vPosB);
		}

		GenericJointComponent* pJointComp = pGameObjA->CreateComponent<GenericJointComponent>();
		pJointComp->GetPhysicsGenericJoint()->SetPysicsObjectB(pGameObjB->GetPhyscisObject());
		NodeTransform tsfA,tsfB;
		TransformSetIdentity(&tsfA);
		TransformSetIdentity(&tsfB);
		tsfB.m_vPos = vPosA - vPosB;
		pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
		pJointComp->GetPhysicsGenericJoint()->SetBTransformLS(tsfB);
		//pJointComp->GetPhysicsGenericJoint()->SetLinearLowerLimit(Vector3(0,0,0));
		//pJointComp->GetPhysicsGenericJoint()->SetLinearUpperLimit(Vector3(0,0,0));
		//pJointComp->GetPhysicsGenericJoint()->SetAngularLowerLimit(Vector3(0,0,0));
		//pJointComp->GetPhysicsGenericJoint()->SetAngularUpperLimit(Vector3(0,0,0));	

// 		GenericJointComponent* pJointComp = pGameObjB->CreateComponent<GenericJointComponent>();
// 		NodeTransform tsfA;
// 		TransformSetIdentity(&tsfA);
// 		tsfA.m_vPos = Vector3(5,0,0);
// 		pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
// 
 		//GetPhysicsSystem()->Start();
	}

	void SampleJoint::UnLoad()
	{
		//GetPhysicsSystem()->Stop();
	}

	void SampleJoint::Update()
	{
			
	}


}


