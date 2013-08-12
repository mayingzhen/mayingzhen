#include "SampleRagdoll.h"

namespace ma
{


	SampleRagdoll::SampleRagdoll()
	{
	}

	GameObject* SampleRagdoll::CreateCapsule(float fRadius,float fHeight,NodeTransform tsf,const char* pName)
	{
		GameObject* pObj = GetEntitySystem()->CreateGameObject(pName);
		CapsuleCollisionComponent* pCapuleComp = pObj->CreateComponent<CapsuleCollisionComponent>();
		pCapuleComp->GetCapsuleCollisionShape()->SetRadius(fRadius);
		pCapuleComp->GetCapsuleCollisionShape()->SetHeight(fHeight);

		RigidBodyComponent* pRigidComp = pObj->CreateComponent<RigidBodyComponent>();
		//pRigidComp->GetRigidBody()->SetKinematic(true);

		pObj->GetSceneNode()->SetTransform(tsf);
		
		return pObj;
	}

	void SampleRagdoll::Load()
	{
		Vector3 vEyePos = Vector3(0, 300, 400);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->LookAt(vEyePos,VAtPos,vUp);


		GetPhysicsSystem()->SetGravity(Vector3(0,-0.98f * 30,0));

		{
			GameObject* pGameObj = GetEntitySystem()->CreateGameObject("Terrain");
		
			BoxCollisionComponent* pBoxCollisionShape = pGameObj->CreateComponent<BoxCollisionComponent>();
			pBoxCollisionShape->GetBoxCollisionShape()->SetSize(Vector3(400,5,400));

			pGameObj->GetSceneNode()->Translate(Vector3(0,-50,0));
		}

		NodeTransform arrTsf[BODYPART_COUNT];
		for (int i = 0; i < BODYPART_COUNT; ++i)
		{
			TransformSetIdentity(&arrTsf[i]);
		}

		arrTsf[BODYPART_PELVIS].m_vPos = Vector3(0,100,0);

		arrTsf[BODYPART_SPINE].m_vPos = Vector3(0,120,0); 

		arrTsf[BODYPART_HEAD].m_vPos = Vector3(0,160,0);

		arrTsf[BODYPART_LEFT_UPPER_LEG].m_vPos = Vector3(-18,65,0); 

		arrTsf[BODYPART_LEFT_LOWER_LEG].m_vPos = Vector3(-18,20,0); 

		arrTsf[BODYPART_RIGHT_UPPER_LEG].m_vPos = Vector3(18,65,0);

		arrTsf[BODYPART_RIGHT_LOWER_LEG].m_vPos = Vector3(18,20,0);
	
		Vector3 vAxiZ(0,0,1);
		arrTsf[BODYPART_LEFT_UPPER_ARM].m_vPos = Vector3(-35,145,0);
		QuaternionRotationAxis(&arrTsf[BODYPART_LEFT_UPPER_ARM].m_qRot,&vAxiZ,PI * 0.5f);
		
		arrTsf[BODYPART_LEFT_LOWER_ARM].m_vPos = Vector3(-70,145,0);
		QuaternionRotationAxis(&arrTsf[BODYPART_LEFT_LOWER_ARM].m_qRot,&vAxiZ,PI * 0.5f);
		
		arrTsf[BODYPART_RIGHT_UPPER_ARM].m_vPos = Vector3(35,145,0);
		QuaternionRotationAxis(&arrTsf[BODYPART_RIGHT_UPPER_ARM].m_qRot,&vAxiZ,-PI  * 0.5f);
		
		arrTsf[BODYPART_RIGHT_LOWER_ARM].m_vPos = Vector3(70,145,0);
		QuaternionRotationAxis(&arrTsf[BODYPART_RIGHT_LOWER_ARM].m_qRot,&vAxiZ,-PI * 0.5f);
			

		m_arrObject[BODYPART_PELVIS] = CreateCapsule(15,20,arrTsf[BODYPART_PELVIS],"BODYPART_PELVIS");
		m_arrObject[BODYPART_SPINE] = CreateCapsule(15,28,arrTsf[BODYPART_SPINE],"BODYPART_SPINE");
		m_arrObject[BODYPART_HEAD] = CreateCapsule(10,5,arrTsf[BODYPART_HEAD],"BODYPART_HEAD");
		m_arrObject[BODYPART_LEFT_UPPER_LEG] = CreateCapsule(7,45,arrTsf[BODYPART_LEFT_UPPER_LEG],"BODYPART_LEFT_UPPER_LEG");
		m_arrObject[BODYPART_LEFT_LOWER_LEG] = CreateCapsule(5,37,arrTsf[BODYPART_LEFT_LOWER_LEG],"BODYPART_LEFT_LOWER_LEG");
		m_arrObject[BODYPART_RIGHT_UPPER_LEG] = CreateCapsule(7,45,arrTsf[BODYPART_RIGHT_UPPER_LEG],"BODYPART_RIGHT_UPPER_LEG");
		m_arrObject[BODYPART_RIGHT_LOWER_LEG] = CreateCapsule(5,37,arrTsf[BODYPART_RIGHT_LOWER_LEG],"BODYPART_RIGHT_LOWER_LEG");
		m_arrObject[BODYPART_LEFT_UPPER_ARM] = CreateCapsule(5,33,arrTsf[BODYPART_LEFT_UPPER_ARM],"BODYPART_LEFT_UPPER_ARM");
		m_arrObject[BODYPART_LEFT_LOWER_ARM] = CreateCapsule(4,25,arrTsf[BODYPART_LEFT_LOWER_ARM],"BODYPART_LEFT_LOWER_ARM");
		m_arrObject[BODYPART_RIGHT_UPPER_ARM] = CreateCapsule(5,33,arrTsf[BODYPART_RIGHT_UPPER_ARM],"BODYPART_RIGHT_UPPER_ARM");
		m_arrObject[BODYPART_RIGHT_LOWER_ARM] = CreateCapsule(4,25,arrTsf[BODYPART_RIGHT_LOWER_ARM],"BODYPART_RIGHT_LOWER_ARM");

		NodeTransform tsfA,tsfB;
		TransformSetIdentity(&tsfA);
		TransformSetIdentity(&tsfB);

		// ******* SPINE HEAD ******** ///
				tsfA.m_vPos = Vector3(0,30,0);
				tsfB.m_vPos = Vector3(0,-14,0);
				GenericJointComponent* pJointComp = m_arrObject[BODYPART_SPINE]->CreateComponent<GenericJointComponent>();
				pJointComp->GetPhysicsGenericJoint()->SetPysicsObjectB(m_arrObject[BODYPART_HEAD]->GetPhyscisObject());
				pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
				pJointComp->GetPhysicsGenericJoint()->SetBTransformLS(tsfB);
				pJointComp->GetPhysicsGenericJoint()->SetAngularLowerLimit(Vector3(-PI*0.3f,-FEPS,-PI*0.3f));
				pJointComp->GetPhysicsGenericJoint()->SetAngularUpperLimit(Vector3(PI*0.5f,FEPS,PI*0.3f));
				
		// ******* LEFT SHOULDER ******** ///
				tsfA.m_vPos = Vector3(-20,15,0);

				tsfB.m_vPos = Vector3(0,18,0);
				EulerAngleXYZ euler(-HALF_PI,0,0/*HALF_PI*/);
				QuaternionFromEulerAngle(&tsfB.m_qRot,(float*)&euler);
				
				pJointComp = m_arrObject[BODYPART_SPINE]->CreateComponent<GenericJointComponent>();
				pJointComp->GetPhysicsGenericJoint()->SetPysicsObjectB(m_arrObject[BODYPART_LEFT_UPPER_ARM]->GetPhyscisObject());
				pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
				pJointComp->GetPhysicsGenericJoint()->SetBTransformLS(tsfB);
				pJointComp->GetPhysicsGenericJoint()->SetAngularLowerLimit(Vector3(-PI*0.8f,-FEPS,-PI*0.5f));
				pJointComp->GetPhysicsGenericJoint()->SetAngularUpperLimit(Vector3(PI*0.8f,FEPS,PI*0.5f));
		
		// ******* RIGHT SHOULDER ******** ///
				tsfA.m_vPos = Vector3(20,15,0);

				tsfB.m_vPos = Vector3(0,-18,0);
				euler = EulerAngleXYZ (HALF_PI,0,0);
				QuaternionFromEulerAngle(&tsfB.m_qRot,(float*)&euler);
				pJointComp = m_arrObject[BODYPART_SPINE]->CreateComponent<GenericJointComponent>();
				pJointComp->GetPhysicsGenericJoint()->SetPysicsObjectB(m_arrObject[BODYPART_RIGHT_UPPER_ARM]->GetPhyscisObject());
				pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
				pJointComp->GetPhysicsGenericJoint()->SetBTransformLS(tsfB);
				pJointComp->GetPhysicsGenericJoint()->SetAngularLowerLimit(Vector3(-PI*0.8f,-FEPS,-PI*0.5f));
				pJointComp->GetPhysicsGenericJoint()->SetAngularUpperLimit(Vector3(PI*0.8f,FEPS,PI*0.5f));
		
		// ******* LEFT ELBOW ******** ///
				tsfA.m_vPos = Vector3(0,18,0);
				tsfB.m_vPos = Vector3(0,-14,0);
				pJointComp = m_arrObject[BODYPART_LEFT_UPPER_ARM]->CreateComponent<GenericJointComponent>();
				pJointComp->GetPhysicsGenericJoint()->SetPysicsObjectB(m_arrObject[BODYPART_LEFT_LOWER_ARM]->GetPhyscisObject());
				pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
				pJointComp->GetPhysicsGenericJoint()->SetBTransformLS(tsfB);
				pJointComp->GetPhysicsGenericJoint()->SetAngularLowerLimit(Vector3(-FEPS,-FEPS,-FEPS));
				pJointComp->GetPhysicsGenericJoint()->SetAngularUpperLimit(Vector3(PI*0.7f,FEPS,FEPS));	
			
		// ******* RIGHT ELBOW ******** ///
				tsfA.m_vPos = Vector3(0,18,0);
				tsfB.m_vPos = Vector3(0,-14,0);
				pJointComp = m_arrObject[BODYPART_RIGHT_UPPER_ARM]->CreateComponent<GenericJointComponent>();
				pJointComp->GetPhysicsGenericJoint()->SetPysicsObjectB(m_arrObject[BODYPART_RIGHT_LOWER_ARM]->GetPhyscisObject());
				pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
				pJointComp->GetPhysicsGenericJoint()->SetBTransformLS(tsfB);
				pJointComp->GetPhysicsGenericJoint()->SetAngularLowerLimit(Vector3(-FEPS,-FEPS,-FEPS));
				pJointComp->GetPhysicsGenericJoint()->SetAngularUpperLimit(Vector3(PI*0.7f,FEPS,FEPS));	
		
		// ******* PELVIS ******** ///
				tsfA.m_vPos = Vector3(0,15,0);
				euler = EulerAngleXYZ(0,HALF_PI,0);
				QuaternionFromEulerAngle(&tsfA.m_qRot,(float*)&euler);

				tsfB.m_vPos = Vector3(0,-15,0);
				euler = EulerAngleXYZ(0,HALF_PI,0);
				QuaternionFromEulerAngle(&tsfB.m_qRot,(float*)&euler);

				pJointComp = m_arrObject[BODYPART_PELVIS]->CreateComponent<GenericJointComponent>();
				pJointComp->GetPhysicsGenericJoint()->SetPysicsObjectB(m_arrObject[BODYPART_SPINE]->GetPhyscisObject());
				pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
				pJointComp->GetPhysicsGenericJoint()->SetBTransformLS(tsfB);
				pJointComp->GetPhysicsGenericJoint()->SetAngularLowerLimit(Vector3(-PI*0.2f,-FEPS,-PI*0.3f));
				pJointComp->GetPhysicsGenericJoint()->SetAngularUpperLimit(Vector3(PI*0.2f,FEPS,PI*0.6f));	
		
		// ******* LEFT HIP ******** ///
				tsfA.m_vPos = Vector3(-18,-10,0);
				tsfB.m_vPos = Vector3(0,22.5,0);
				pJointComp = m_arrObject[BODYPART_PELVIS]->CreateComponent<GenericJointComponent>();
				pJointComp->GetPhysicsGenericJoint()->SetPysicsObjectB(m_arrObject[BODYPART_LEFT_UPPER_LEG]->GetPhyscisObject());
				pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
				pJointComp->GetPhysicsGenericJoint()->SetBTransformLS(tsfB);
				pJointComp->GetPhysicsGenericJoint()->SetAngularLowerLimit(Vector3(-HALF_PI * 0.5,-FEPS,-FEPS));
				pJointComp->GetPhysicsGenericJoint()->SetAngularUpperLimit(Vector3(HALF_PI*0.8,FEPS,HALF_PI*0.6));	
		
		
		// ******* RIGHT HIP ******** ///
				tsfA.m_vPos = Vector3(18,-10,0);
				tsfB.m_vPos = Vector3(0,22.5,0);
				pJointComp = m_arrObject[BODYPART_PELVIS]->CreateComponent<GenericJointComponent>();
				pJointComp->GetPhysicsGenericJoint()->SetPysicsObjectB(m_arrObject[BODYPART_RIGHT_UPPER_LEG]->GetPhyscisObject());
				pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
				pJointComp->GetPhysicsGenericJoint()->SetBTransformLS(tsfB);
				pJointComp->GetPhysicsGenericJoint()->SetAngularLowerLimit(Vector3(-HALF_PI * 0.5,-FEPS,-FEPS));
				pJointComp->GetPhysicsGenericJoint()->SetAngularUpperLimit(Vector3(HALF_PI*0.8,FEPS,HALF_PI*0.6));	
		
		// ******* LEFT KNEE ******** ///
				tsfA.m_vPos = Vector3(0,-22.5,0);
				tsfB.m_vPos = Vector3(0,18.5,0);
				pJointComp = m_arrObject[BODYPART_LEFT_UPPER_LEG]->CreateComponent<GenericJointComponent>();
				pJointComp->GetPhysicsGenericJoint()->SetPysicsObjectB(m_arrObject[BODYPART_LEFT_LOWER_LEG]->GetPhyscisObject());
				pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
				pJointComp->GetPhysicsGenericJoint()->SetBTransformLS(tsfB);
				pJointComp->GetPhysicsGenericJoint()->SetAngularLowerLimit(Vector3(-FEPS,-FEPS,-FEPS));
				pJointComp->GetPhysicsGenericJoint()->SetAngularUpperLimit(Vector3(PI*0.7,FEPS,FEPS));
		
		// ******* RIGHT KNEE ******** ///
				tsfA.m_vPos = Vector3(0,-22.5,0);
				tsfB.m_vPos = Vector3(0,18.5,0);
				pJointComp = m_arrObject[BODYPART_RIGHT_UPPER_LEG]->CreateComponent<GenericJointComponent>();
				pJointComp->GetPhysicsGenericJoint()->SetPysicsObjectB(m_arrObject[BODYPART_RIGHT_LOWER_LEG]->GetPhyscisObject());
				pJointComp->GetPhysicsGenericJoint()->SetATransformLS(tsfA);
				pJointComp->GetPhysicsGenericJoint()->SetBTransformLS(tsfB);
				pJointComp->GetPhysicsGenericJoint()->SetAngularLowerLimit(Vector3(-FEPS,-FEPS,-FEPS));
				pJointComp->GetPhysicsGenericJoint()->SetAngularUpperLimit(Vector3(PI*0.7,FEPS,FEPS));

	}

	void SampleRagdoll::UnLoad()
	{
	}


	void SampleRagdoll::Update()
	{
	}

	void SampleRagdoll::Render()
	{
	}

}


