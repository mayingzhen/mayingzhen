#ifndef  _SKELMESHCOMPONENT__H__
#define  _SKELMESHCOMPONENT__H__

//#include "Animation/NodePose.h"

namespace ma
{
	class SkeletonRes;
	class Skeleton;
	class MeshComponent;
	class AnimationSet;
	class AnimationPlay;

	class FRAMEWORK_API SkelMeshComponent : public Component
	{
	public:
		SkelMeshComponent();

		~SkelMeshComponent();

		void Update();
			 
		void Render();

		void AddMeshComp(MeshComponent* pMeshComp);

		void SetSkeleton(Skeleton* pSkeleton);

		void SetAnimationSet(AnimationSet* pAnimSet);

		void PlayAnimation(const char* pszAniName);

	private:
		Skeleton* m_pSkeleton;

		AnimationSet* m_pAnimSet;

		std::vector<MeshComponent*> m_vMeshComp;	

		AnimationPlay* m_pAnimtionPlay;
	};
}

#endif
