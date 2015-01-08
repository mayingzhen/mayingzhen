#include "Ragdoll.h"

namespace ma
{

	Ragdoll::Ragdoll()
	{

	}

	Ragdoll::~Ragdoll() 
	{

	}

	void Ragdoll::Init(AnimationComponent* pAnimComp,std::vector<std::string> arrBodyName,std::vector<Vector2> arrSize)
	{
		ASSERT(arrBodyName.size() == arrSize.size() && arrSize.size() == BODYPART_COUNT);

		m_pAnimaComp = pAnimComp;

		for (UINT i = 0; i < BODYPART_COUNT; ++i)
		{
			m_arrObject[i] = CreateCapsule(arrSize[i].x,arrSize[i].y,arrBodyName[i].c_str());
		}

		CreateJoint(BODYPART_SPINE,BODYPART_HEAD,Vector3(-Math::PI*0.3f,-Math::PI*0.3f,0),Vector3(Math::PI*0.5f,Math::PI*0.3f,0));	

		CreateJoint(BODYPART_SPINE,BODYPART_LEFT_UPPER_ARM,Vector3(-Math::PI*0.8f,-Math::PI*0.5f,0),Vector3(Math::PI*0.8f,Math::PI*0.5f,0));

		CreateJoint(BODYPART_SPINE,BODYPART_RIGHT_UPPER_ARM,Vector3(-Math::PI*0.8f,-Math::PI*0.5f,0),Vector3(Math::PI*0.8f,Math::PI*0.5f,0));

		CreateJoint(BODYPART_LEFT_UPPER_ARM,BODYPART_LEFT_LOWER_ARM,Vector3(0,0,0),Vector3(Math::PI*0.7f,0,0));

		CreateJoint(BODYPART_RIGHT_UPPER_ARM,BODYPART_RIGHT_LOWER_ARM,Vector3(0,0,0),Vector3(Math::PI*0.7f,0,0));

		CreateJoint(BODYPART_PELVIS,BODYPART_SPINE,Vector3(-Math::PI*0.2f,-Math::PI*0.3f,0),Vector3(Math::PI*0.2f,Math::PI*0.3f,0));

		CreateJoint(BODYPART_PELVIS,BODYPART_LEFT_UPPER_LEG,Vector3(-Math::HALF_PI*0.5f,0,0),Vector3(Math::HALF_PI*0.8,Math::HALF_PI*0.6f,0));

		CreateJoint(BODYPART_PELVIS,BODYPART_RIGHT_UPPER_LEG,Vector3(-Math::HALF_PI*0.5f,0,0),Vector3(Math::HALF_PI*0.8,Math::HALF_PI*0.6f,0));

		CreateJoint(BODYPART_LEFT_UPPER_LEG,BODYPART_LEFT_LOWER_LEG,Vector3(-Math::PI*0.7f,0,0),Vector3(0,0,0));

		CreateJoint(BODYPART_RIGHT_UPPER_LEG,BODYPART_RIGHT_LOWER_LEG,Vector3(-Math::PI*0.7f,0,0),Vector3(0,0,0));
	}

	void Ragdoll::Start()
	{	
		m_pAnimaComp->Stop();

		for (int i = 0; i < BODYPART_COUNT; ++i)
		{
			SceneNode* pBoneObj = m_arrObject[i];

			IRigidBody* pRigidBody = pBoneObj->GetTypeComponent<IRigidBody>();
			pRigidBody->SetKinematic(false);
		}
	}

	void Ragdoll::SyncToPhysics()
	{
		SkeletonPose* pAnimPose = m_pAnimaComp->GetAnimationPose();
		Skeleton* pSkeleton = m_pAnimaComp->GetSkeleton();

		for (int i = 0; i < BODYPART_COUNT; ++i)
		{
			BoneIndex boneID = pSkeleton->GetBoneIdByName( m_arrObject[i]->GetName() );

			const Transform& tsfOS = pAnimPose->GetTransformOS(boneID);
			m_arrObject[i]->SetTransformWS(tsfOS);
		}
	}

	void Ragdoll::SyncFromPhysics()
	{
		SkeletonPose* pAnimPose = m_pAnimaComp->GetAnimationPose();
		Skeleton* pSkeleton = m_pAnimaComp->GetSkeleton();

		for (int i = 0; i < BODYPART_COUNT; ++i)
		{
			BoneIndex boneID = pSkeleton->GetBoneIdByName( m_arrObject[i]->GetName() );

			const Transform tsfOS = m_arrObject[i]->GetTransformWS();
			pAnimPose->SetTransformOS(&tsfOS,boneID);
		}

		m_pAnimaComp->UpdateSkinMatrix();
	}

	SceneNode* Ragdoll::CreateCapsule(float fRadius,float fHeight,const char* pBoneName)
	{
		SkeletonPose* pAnimPose = m_pAnimaComp->GetAnimationPose();
		Skeleton* pSkeleton = m_pAnimaComp->GetSkeleton();

		Transform tsfOffset;

		Matrix3 euler;	
		euler.FromEulerAnglesXYZ(Radian(0),Radian(0),Radian(Math::HALF_PI));
		tsfOffset.m_qRot.FromRotationMatrix(euler);	
		tsfOffset.m_vPos = Vector3(fRadius  + fHeight * 0.5f, 0, 0);

		BoneIndex boneID = pSkeleton->GetBoneIdByName(pBoneName);
		Transform tsfBone = pAnimPose->GetTransformOS(boneID);

		ASSERT(false);
		SceneNode* pBoneNode = NULL;//CreateGameObject(pBoneName).get();
		pBoneNode->SetTransformWS(tsfBone);

		ICapsuleCollisionShape* pCapuleComp = pBoneNode->CreateComponent<ICapsuleCollisionShape>();
		pCapuleComp->SetRadius(fRadius);
		pCapuleComp->SetHeight(fHeight);
		pCapuleComp->SetTransformLS(tsfOffset);

		IRigidBody* pRigidComp = pBoneNode->CreateComponent<IRigidBody>();
		pRigidComp->SetMass(1.0f);
		pRigidComp->SetLinearDamping(0.05f);
		pRigidComp->SetAngularDamping(0.85f);
		pRigidComp->SetDeactivationTime(0.8f);
		pRigidComp->SetSleepingThresholds(1.6f, 2.5f);
		pRigidComp->SetKinematic(true);

		return pBoneNode;
	}

	void Ragdoll::CreateJoint(int eBoneA,int eBoneB,Vector3 vLowerLimit,Vector3 vUpLimit)
	{
		SceneNode* pBoneObjA = m_arrObject[eBoneA];
		SceneNode* pBoneObjB = m_arrObject[eBoneB];

		Vector3 vPosWS = pBoneObjB->GetPos();

		Transform tsfALS,tsfBLS;
		tsfALS.m_vPos = pBoneObjA->GetMatrixWS().inverse() * vPosWS;
		tsfALS.m_qRot = pBoneObjA->GetRotationWS().Inverse();

		tsfBLS.m_vPos = pBoneObjB->GetMatrixWS().inverse() * vPosWS;
		tsfBLS.m_qRot = pBoneObjB->GetRotationWS().Inverse();

		IPhysicsGenericJoint* pJointComp = pBoneObjA->CreateComponent<IPhysicsGenericJoint>();
		pJointComp->SetPysicsObjectB(pBoneObjB);
		pJointComp->SetATransformLS(tsfALS);
		pJointComp->SetBTransformLS(tsfBLS);
		pJointComp->SetAngularLowerLimit(vLowerLimit);
		pJointComp->SetAngularUpperLimit(vUpLimit);
	}


}


