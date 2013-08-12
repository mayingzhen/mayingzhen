#ifndef _Sample_Ragdoll_H_
#define _Sample_Ragdoll_H_

namespace ma
{
	class Scene;

	enum
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

	class SampleRagdoll : public Sample
	{
	public:
		SampleRagdoll();

		virtual void Load();

		virtual	void UnLoad();

		virtual void Update();

		virtual void Render();

	private:
		GameObject* CreateCapsule(float fRadius,float fHeight,NodeTransform tsf,const char* pName);

	private:

		GameObject*	m_arrObject[BODYPART_COUNT];
	};
}


#endif

