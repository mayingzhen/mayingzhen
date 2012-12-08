#ifndef  _SKELMESHCOMPONENT__H__
#define  _SKELMESHCOMPONENT__H__

//#include "Animation/NodePose.h"

namespace ma
{
	class SkeletonRes;
	class AnimationRes;
	class MeshComponent;
	class NodePose;
	class AnimationInst;
	class AnimationSet;
	class Animation;

	class FRAMEWORK_API SkelMeshComponent : public Component
	{
	public:
		SkelMeshComponent();

		~SkelMeshComponent();

		void Update();
			 
		void Render();

		void AddMeshComp(MeshComponent* pMeshComp);

		void LoadSkeleton(const char* pSkelPath);

		void PlayAnimation(const char* pszAniName);

		void PlayAnimation(Animation* pAnimation);

		void LoadAnimation(const char* pAniPath);
	
	private:
		SkeletonRes* m_pSkelRes;

		AnimationRes* m_pAniRes;

		AnimationInst* m_pAnimationInst;

		NodePose*	m_pose;

		//AnimationSet* m_pAnimationSet;

//		SkeletonAnimation* m_pSkeletonAnimation;

		/// 
// 		{
// 			Skeleton* m_pSkeleton;
// 			AnimationInst* m_pAnimationInst;
// 			NodePose* m_AniPos;
// 		}

		std::vector<MeshComponent*> m_vMeshComp;	
		
	};
}

#endif
