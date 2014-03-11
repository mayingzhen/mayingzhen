#include "SampleJoint.h"

namespace ma
{
	SampleJoint::SampleJoint()
	{
	}

	void SampleJoint::Load()
	{

		Vector3 vPosA(20,40,0);
		Vector3 vPosB(0,40,0);

		GameObjectPtr pGameObjA = NULL;
		{
			pGameObjA = GetEntitySystem()->CreateGameObject("physicsA");

			IBoxCollisionShape* pBoxCollisionComp = pGameObjA->CreateComponent<IBoxCollisionShape>();
			pBoxCollisionComp->SetSize(Vector3(5,5,5));

			IRigidBody* pRigidBodyComp = pGameObjA->CreateComponent<IRigidBody>();
			pRigidBodyComp->SetKinematic(true);

			pGameObjA->GetSceneNode()->Translate(vPosA);

		}

		GameObjectPtr pGameObjB = NULL;
		{
			pGameObjB = GetEntitySystem()->CreateGameObject("physicsB");

			IBoxCollisionShape* pBoxCollisionShape = pGameObjB->CreateComponent<IBoxCollisionShape>();
			pBoxCollisionShape->SetSize(Vector3(5,5,5));
		
			IRigidBody* pRigidBodyComp = pGameObjB->CreateComponent<IRigidBody>();

			pGameObjB->GetSceneNode()->Translate(vPosB);
		}

		IPhysicsGenericJoint* pJointComp = pGameObjA->CreateComponent<IPhysicsGenericJoint>();
		pJointComp->SetPysicsObjectB(pGameObjB->GetPhyscisObject());
		NodeTransform tsfA,tsfB;
		TransformSetIdentity(&tsfA);
		TransformSetIdentity(&tsfB);
		tsfB.m_vPos = vPosA - vPosB;
		pJointComp->SetATransformLS(tsfA);
		pJointComp->SetBTransformLS(tsfB);
		//pJointComp->SetLinearLowerLimit(Vector3(0,0,0));
		//pJointComp->SetLinearUpperLimit(Vector3(0,0,0));
		//pJointComp->SetAngularLowerLimit(Vector3(0,0,0));
		//pJointComp->SetAngularUpperLimit(Vector3(0,0,0));	

// 		GenericJointComponent* pJointComp = pGameObjB->CreateComponent<GenericJointComponent>();
// 		NodeTransform tsfA;
// 		TransformSetIdentity(&tsfA);
// 		tsfA.m_vPos = Vector3(5,0,0);
// 		pJointComp->SetATransformLS(tsfA);

	}

	void SampleJoint::UnLoad()
	{

	}

	void SampleJoint::Update()
	{
			
	}


}


