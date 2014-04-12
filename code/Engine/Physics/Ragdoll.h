#ifndef _Ragdoll_H_
#define _Ragdoll_H_

namespace ma
{
	enum BODYPART
	{
		BODYPART_PELVIS = 0,
		BODYPART_SPINE,
		BODYPART_HEAD,

		BODYPART_LEFT_UPPER_LEG,
		BODYPART_LEFT_LOWER_LEG,

		BODYPART_RIGHT_UPPER_LEG,
		BODYPART_RIGHT_LOWER_LEG,

		BODYPART_LEFT_UPPER_ARM,
		BODYPART_LEFT_LOWER_ARM,

		BODYPART_RIGHT_UPPER_ARM,
		BODYPART_RIGHT_LOWER_ARM,

		BODYPART_COUNT
	};

	enum
	{
		JOINT_PELVIS_SPINE = 0,
		JOINT_SPINE_HEAD,

		JOINT_LEFT_HIP,
		JOINT_LEFT_KNEE,

		JOINT_RIGHT_HIP,
		JOINT_RIGHT_KNEE,

		JOINT_LEFT_SHOULDER,
		JOINT_LEFT_ELBOW,

		JOINT_RIGHT_SHOULDER,
		JOINT_RIGHT_ELBOW,

		JOINT_COUNT
	};

	struct BodyPart
	{
		std::string				m_sPartName;

		std::vector<BoneIndex>	m_arrBoneID;

		float					m_fHight;

		float					m_fRadius;
	};

	class BodyPartMap
	{
	public:
		BodyPartMap();

		void		Build(Skeleton* pSkeleton);	

	private:
		Skeleton*				m_pSkeleton;

		BodyPart				m_arrBodyPart[BODYPART_COUNT];	
	};

	class Ragdoll
	{
	public:
		Ragdoll();

		~Ragdoll();

		void Init(SkeletonPose* pSkeletonPose,Skeleton* pSkeleton,float fRadius, BodyPartMap* pBodyMap);

		void SyncToPhysics();

		void SyncFromPhysics();

	private:
		SkeletonPose*			m_pSkelePose;

		Skeleton*				m_pSkeleton;
			
		BodyPartMap*			m_pBodyMap;

		IPhysicsObject*			m_arrBody[BODYPART_COUNT];

		IPhysicsGenericJoint*	m_arrJoint[JOINT_COUNT];

		NodeTransform			m_tsfWS;


		struct	BodyJointInfo
		{
			BODYPART		m_eBodyA;
			BODYPART		m_eBodyB;
			NodeTransform	m_tsfA;
			NodeTransform	m_tsfB;
			Vector3			m_vAngLowerLimit;
			Vector3			m_vAngUpperLimit;
		};

		BodyJointInfo			m_arrJointInfo[JOINT_COUNT];

		IPhysicsGenericJoint*	m_arrPhysicsJoint[JOINT_COUNT];
		
	};
}


#endif // _Ragdoll_H_

