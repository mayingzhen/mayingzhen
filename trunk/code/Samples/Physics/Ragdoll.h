#ifndef _Ragdoll_H_
#define _Ragdoll_H_

namespace ma
{

	class Ragdoll : public Referenced
	{
		enum BODYPART
		{
			BODYPART_PELVIS = 0,
			BODYPART_SPINE,
			BODYPART_HEAD,

			BODYPART_LEFT_UPPER_ARM,
			BODYPART_RIGHT_UPPER_ARM,

			BODYPART_LEFT_LOWER_ARM,
			BODYPART_RIGHT_LOWER_ARM,

			BODYPART_LEFT_UPPER_LEG,
			BODYPART_RIGHT_UPPER_LEG,

			BODYPART_LEFT_LOWER_LEG,
			BODYPART_RIGHT_LOWER_LEG,

			BODYPART_COUNT
		};

	public:

		Ragdoll();

		~Ragdoll();

		void Init(AnimationComponent* pAnimComp,std::vector<std::string> arrBodyName,std::vector<Vector2> arrSize);

		void Start();

		void SyncToPhysics();

		void SyncFromPhysics();

	private:
		SceneNode* CreateCapsule(float fRadius,float fHeight,const char* pBoneName);
	
		void		CreateJoint(int eBoneA,int eBoneB,Vector3 vLowerLimit,Vector3 vUpLimit);

	private:
		AnimationComponent*		m_pAnimaComp;

		SceneNode*				m_arrObject[BODYPART_COUNT];
	};
}

#endif


