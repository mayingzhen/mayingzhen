#ifndef  _SKELMESHCOMPONENT__H__
#define  _SKELMESHCOMPONENT__H__

//#include "Animation/NodePose.h"

namespace ma
{
	class SkeletonRes;
	class AnimationRes;
	class MeshComponent;
	class NodePose;

	class FRAMEWORK_API SkelMeshComponent : public Component
	{
	public:
		SkelMeshComponent();

		~SkelMeshComponent();

		void Update();
			 
		void Render();

		void AddMeshComp(MeshComponent* pMeshComp);

		void LoadSkeleton(const char* pSkelPath);

		void LoadAnimation(const char* pAniPath);
	
	private:
		SkeletonRes* m_pSkelRes;

		AnimationRes* m_pAniRes;

		NodePose*	m_pose;

		std::vector<MeshComponent*> m_vMeshComp;	
		
	};
}

#endif
