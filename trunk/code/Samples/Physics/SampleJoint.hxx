#include "SampleJoint.h"

namespace ma
{
	SampleJoint::SampleJoint()
	{
	}

	void SampleJoint::Load()
	{
		Vector3 vPosA(20,0,0);
		Vector3 vPosB(0,0,0);

		SceneNodePtr pGameObjA = NULL;
		{
			pGameObjA = m_pScene->CreateNode("physicsA");

			RefPtr<BoxCollisionShape> pBoxCollisionComp = pGameObjA->CreateComponent<BoxCollisionShape>();
			pBoxCollisionComp->SetSize(Vector3(5,5,5));

			RefPtr<RigidBody> pRigidBodyComp = pGameObjA->CreateComponent<RigidBody>();
			pRigidBodyComp->SetKinematic(true);

			pGameObjA->Translate(vPosA);

		}

		SceneNodePtr pGameObjB = NULL;
		{
			pGameObjB = m_pScene->CreateNode("physicsB");

			RefPtr<BoxCollisionShape> pBoxCollisionShape = pGameObjB->CreateComponent<BoxCollisionShape>();
			pBoxCollisionShape->SetSize(Vector3(5,5,5));
		
			RefPtr<RigidBody> pRigidBodyComp = pGameObjB->CreateComponent<RigidBody>();

			pGameObjB->Translate(vPosB);
		}

		RefPtr<PhysicsGenericJoint> pJointComp = pGameObjA->CreateComponent<PhysicsGenericJoint>();
		pJointComp->SetPysicsObjectB(pGameObjB.get());
		Transform tsfA,tsfB;
		tsfB.m_vPos = vPosA - vPosB;
		pJointComp->SetATransformLS(tsfA);
		pJointComp->SetBTransformLS(tsfB);
		pJointComp->SetLinearLowerLimit(Vector3(0,0,0));
		pJointComp->SetLinearUpperLimit(Vector3(0,0,0));
		pJointComp->SetAngularLowerLimit(Vector3(0,0,0));
		pJointComp->SetAngularUpperLimit(Vector3(0,0,0));	

// 		GenericJointComponent* pJointComp = pGameObjB->CreateComponent<GenericJointComponent>();
// 		Transform tsfA;
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


