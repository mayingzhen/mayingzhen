#ifndef  _AnimationPlay__H__
#define  _AnimationPlay__H__

namespace ma
{
	class Skeleton;
	class NodePose;
	class AnimationAction;

	class ANIMATION_API AnimationPlay
	{
	public:
		AnimationPlay();

		~AnimationPlay();

		void SetSkeleton(Skeleton* pSkeleton);

		void PlayAnimation(AnimationAction* pSkelAnim);

		void AdvanceTime(float fTimeElepse);

		void EvaluateAnimation(float fWeight);

		NodePose* GetAnimationPose() {return m_pose;}

	private:
		Skeleton*		m_pSkeleton;
		
		NodePose*		m_pose;

		AnimationAction*  m_pSkelAnim;
	};
}

#endif