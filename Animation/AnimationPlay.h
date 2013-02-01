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

		void SetAnimationSet(AnimationSet* pAnimationSet);

		void PlayAnimation(AnimationAction* pSkelAnim);

		void PlayAnimation(const char* pszAnimName);
		
		void AdvanceTime(float fTimeElepse);

		void EvaluateAnimation(float fWeight);

		NodePose* GetAnimationPose() {return m_pose;}

		UINT GetSkinMatrixNumber() {return m_pSkeleton->GetBoneNumer();}

		D3DXMATRIX GetSkinMatrixByIndex(UINT index) {return m_arrSkinMatrix[index];}

		D3DXMATRIX* GetSkinMatrixArray() {return m_arrSkinMatrix;}

	private:
		Skeleton*			m_pSkeleton;
		
		NodePose*			m_pose;

		AnimationSet*		m_pAnimSet;

		AnimationAction*	m_pSkelAnim;

		D3DXMATRIX			m_arrSkinMatrix[256];
	};
}

#endif