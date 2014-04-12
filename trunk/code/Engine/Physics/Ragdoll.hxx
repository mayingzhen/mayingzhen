#include "Ragdoll.h"

namespace ma
{
	BodyPartMap::BodyPartMap()
	{
	}

	void BodyPartMap::Build(Skeleton* pSkeleton)
	{
		ASSERT(pSkeleton);
		if (pSkeleton == NULL)
			return;

		m_pSkeleton = pSkeleton;

		const char* arrBodyName[BODYPART_COUNT];
		arrBodyName[BODYPART_PELVIS] = "pelvis";
		arrBodyName[BODYPART_SPINE] = "spine";
		arrBodyName[BODYPART_HEAD] = "head";
		arrBodyName[BODYPART_LEFT_UPPER_LEG] = "left upper leg";
		arrBodyName[BODYPART_LEFT_LOWER_LEG] = "left lower leg";
		arrBodyName[BODYPART_RIGHT_UPPER_LEG] = "right upper leg";
		arrBodyName[BODYPART_RIGHT_LOWER_LEG] = "right lower leg";
		arrBodyName[BODYPART_LEFT_UPPER_ARM] = "left upper arm";
		arrBodyName[BODYPART_LEFT_LOWER_ARM] = "left lower arm";
		arrBodyName[BODYPART_RIGHT_UPPER_ARM] = "right lower arm";
		arrBodyName[BODYPART_RIGHT_LOWER_ARM] = "right lower arm";
		
		float arrHight[BODYPART_COUNT];
		arrHight[BODYPART_PELVIS] = 0.02f;
		arrHight[BODYPART_SPINE] = 0.28f;
		arrHight[BODYPART_HEAD] = 0.05f;
		arrHight[BODYPART_LEFT_UPPER_LEG] = 0.45f;
		arrHight[BODYPART_LEFT_LOWER_LEG] = 0.37f;
		arrHight[BODYPART_RIGHT_UPPER_LEG] = 0.45f;
		arrHight[BODYPART_RIGHT_LOWER_LEG] = 0.37f;
		arrHight[BODYPART_LEFT_UPPER_ARM] = 0.33f;
		arrHight[BODYPART_LEFT_LOWER_ARM] = 0.25f;
		arrHight[BODYPART_RIGHT_UPPER_ARM] = 0.33f;
		arrHight[BODYPART_RIGHT_LOWER_ARM] = 0.25f;

		float arrRadius[BODYPART_COUNT];
		arrRadius[BODYPART_PELVIS] = 0.15f;
		arrRadius[BODYPART_SPINE] = 0.15f;
		arrRadius[BODYPART_HEAD] = 0.10f;
		arrRadius[BODYPART_LEFT_UPPER_LEG] = 0.07f;
		arrRadius[BODYPART_LEFT_LOWER_LEG] = 0.05f;
		arrRadius[BODYPART_RIGHT_UPPER_LEG] = 0.07f;
		arrRadius[BODYPART_RIGHT_LOWER_LEG] = 0.05f;
		arrRadius[BODYPART_LEFT_UPPER_ARM] = 0.05f;
		arrRadius[BODYPART_LEFT_LOWER_ARM] = 0.04f;
		arrRadius[BODYPART_RIGHT_UPPER_ARM] = 0.05f;
		arrRadius[BODYPART_RIGHT_LOWER_ARM] = 0.04f;

		for (UINT iBody = 0; iBody < BODYPART_COUNT; ++iBody)
		{
			BoneIndex boneID = pSkeleton->GetBoneIdByName(arrBodyName[iBody]);
			ASSERT(IsValidID<BoneIndex>(boneID));
			if (!IsValidID<BoneIndex>(boneID))
				continue;

			m_arrBodyPart[iBody].m_sPartName = arrBodyName[iBody];

			for (UINT iBone = 0; iBone < pSkeleton->GetBoneNumer(); ++iBone)
			{
				if (iBone == boneID)
				{
					m_arrBodyPart[iBody].m_arrBoneID.push_back(boneID);
				}
				else if ( pSkeleton->IsAncestorOf(boneID,iBone) )
				{
					m_arrBodyPart[iBody].m_arrBoneID.push_back(boneID);
				}
			}	
		}
	}

	BoneIndex	BodyPartMap::GetMapBoneFirst(BODYPART eBodyPart)
	{
		BoneIndex boneID = m_pSkeleton->GetBoneIdByName(m_arrBodyName[eBodyPart].c_str());
		ASSERT(IsValidID<BoneIndex>(boneID));
		return boneID;
	}
// 
// 	BoneIndex	BodyPartMap::GetMapBoneEnd(BODYPART eBodyPart)
// 	{
// 
// 	}

// 	BODYPART	BodyPartMap::GetMapBody(BoneIndex boneID)
// 	{
// 		ASSERT(boneID >=0 && boneID < m_arrMapBody.size());
// 		if (boneID < 0 || boneID >= m_arrMapBody.size())
// 			return BODYPART_PELVIS;
// 
// 		return m_arrMapBody[boneID];
// 	}

	Ragdoll::Ragdoll()
	{

	}

	Ragdoll::~Ragdoll()
	{

	}

	void Ragdoll::Init(SkeletonPose* pSkeletonPose, Skeleton* pSkeleton, float fRadius, BodyPartMap* pBoneMap)
	{
		m_pSkeleton = pSkeleton;
		m_pSkelePose = pSkeletonPose;
		m_pBodyMap = pBoneMap;

		//CreateBody();
		for (UINT iBody = 0; iBody < BODYPART_COUNT; ++iBody)
		{
			m_arrBody[iBody] = GetPhysicsSystem()->CreatePhysicsObject(NULL);
			
			BoneIndex boneFirst = m_pBodyMap->GetMapBoneFirst(iBody);
			BoneIndex boneEnd = m_pBodyMap->GetMapBoneEnd(iBody);

			const NodeTransform& tsfOSFirst = m_pSkelePose->GetTransformOS(boneFirst);
			const NodeTransform& tsfOSEnd = m_pSkelePose->GetTransformOS(boneEnd);

			Vector3 vTemp = tsfOSFirst.m_vPos - tsfOSEnd.m_vPos;
			float fLength = Vec3Length(&vTemp);

			ICapsuleCollisionShape* pCapSule = m_arrBody[iBody]->CreateCapsuleCollisionShape();
			pCapSule->SetHeight(fLength);
			pCapSule->GetRadius(fRadius);

			IRigidBody*	pRigid = m_arrBody[iBody]->CreateRigidBody();
		}

		// JointInfo
		m_arrJointInfo[JOINT_SPINE_HEAD].m_eBodyA = m_arrBody[BODYPART_SPINE];
		m_arrJointInfo[JOINT_SPINE_HEAD].m_eBodyA = m_arrBody[BODYPART_HEAD];
		m_arrJointInfo[JOINT_SPINE_HEAD].m_tsfA = ;
		m_arrJointInfo[JOINT_SPINE_HEAD].m_tsfA = ;
		m_arrJointInfo[JOINT_SPINE_HEAD].m_vAngLowerLimit;
		m_arrJointInfo[JOINT_SPINE_HEAD].m_vAngUpperLimit = ;

		m_arrJointInfo[JOINT_PELVIS_SPINE].m_eBodyA = m_arrBody[BODYPART_PELVIS];
		m_arrJointInfo[JOINT_PELVIS_SPINE].m_eBodyA = m_arrBody[BODYPART_SPINE];
		m_arrJointInfo[JOINT_PELVIS_SPINE].m_tsfA = ;
		m_arrJointInfo[JOINT_PELVIS_SPINE].m_tsfA = ;
		m_arrJointInfo[JOINT_PELVIS_SPINE].m_vAngLowerLimit;
		m_arrJointInfo[JOINT_PELVIS_SPINE].m_vAngUpperLimit = ;


		//CreateJoint();
		for (UINT iJoint = 0; iJoint < JOINT_COUNT; ++iJoint)
		{
			BodyJointInfo& jointInfo = m_arrJointInfo[iJoint];
			IPhysicsObject* pObjA = m_arrBody[jointInfo.m_eBodyA];
			IPhysicsObject* pObjB = m_arrBody[jointInfo.m_eBodyB];
			m_arrPhysicsJoint[iJoint] = GetPhysicsSystem()->CreatePhysicsGenericJoint(pObjA,pObjB);
			m_arrPhysicsJoint[iJoint]->SetATransformLS(jointInfo.m_tsfA);
			m_arrPhysicsJoint[iJoint]->SetATransformLS(jointInfo.m_tsfB);
			m_arrPhysicsJoint[iJoint]->SetAngularLowerLimit(jointInfo.m_vAngLowerLimit);
			m_arrPhysicsJoint[iJoint]->SetAngularUpperLimit(jointInfo.m_vAngUpperLimit);
		}


		SyncToPhysics();

	}

	void Ragdoll::SyncToPhysics()
	{
		for (UINT iBody = 0; iBody < BODYPART_COUNT; ++iBody)
		{
			BoneIndex boneID = m_pBodyMap->GetMapBoneFirst(iBody);
			ASSERT(IsValidID<BoneIndex>(boneID));
			if (!IsValidID<BoneIndex>(boneID))
				continue;
			
			const NodeTransform& tsfOS = m_pSkelePose->GetTransformOS(boneID);

			NodeTransform tsfWS;
			TransformMul(&tsfWS,tsfOS,m_tsfWS);

			m_arrBody[iBody]->SetTransformWS(tsfWS);
		}
	}

	void Ragdoll::SyncFromPhysics()
	{
		NodeTransform tsfOS[BODYPART_COUNT];

		for (UINT iBody = 0; iBody < BODYPART_COUNT; ++iBody)
		{
			NodeTransform tsfWS = m_arrBody[iBody]->GetTransformWS();
			
			NodeTransform tsfOS[iBody];
			TransformInvMul(&tsfOS[iBody],&m_tsfWS,&tsfWS);	
		}

		std::vector<NodeTransform> tsfOSALL;

		for (UINT iBone = 0 ; iBone < m_pSkelePose->GetNodeNumber(); ++iBone)
		{
			BODYPART eBodye = m_pBodyMap->GetMapBody(iBone);
			tsfOSALL.push_back(tsfOS[eBodye]);
		}

		m_pSkelePose->SetTransformOSAll(tsfOSALL);
	}
		
}


