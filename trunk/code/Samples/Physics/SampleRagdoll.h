#ifndef _Sample_Ragdoll_H_
#define _Sample_Ragdoll_H_

namespace ma
{
	class Scene;

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


	class SampleRagdoll : public Sample
	{
	public:
		SampleRagdoll();

		virtual void Load();

		virtual	void UnLoad();

	private:
		GameObjectPtr CreateCapsule(float fRadius,float fHeight,NodeTransform tsf,const char* pName);

	private:

		GameObjectPtr	m_arrObject[BODYPART_COUNT];
	};
}


#endif

