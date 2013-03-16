#ifndef  _AnimationPlay__H__
#define  _AnimationPlay__H__

namespace ma
{
	class Skeleton;
	class SkeletonPose;
	class Action;

	typedef UINT ActionID;

	class ANIMATION_API AnimationPlay : public IAnimationPlay
	{
	public:
		AnimationPlay(ISkeleton* pSkeleton = NULL);

		~AnimationPlay();

		virtual void			SetSkeleton(ISkeleton* pSkeleton);

		virtual	ISkeleton*		GetSkeleton() {return m_pSkeleton;}

		virtual void			SetAnimationSet(IAnimationSet* pAnimationSet);

		virtual IAnimationSet*	GetAnimationSet() {return m_pAnimSet;}

		virtual	void			AddAction(const char* pszSkaPath, const char* actionName);

		virtual void			PlayAnimation(const char* pszAnimName); 

		virtual void			PlayAnimation(ActionID actionID);

		virtual void			AdvanceTime(float fTimeElepse);

		virtual void			EvaluateAnimation(float fWeight);

		virtual UINT			GetSkinMatrixNumber() {return m_pSkeleton->GetBoneNumer();}

		virtual D3DXMATRIX*		GetSkinMatrixArray()  {return m_arrSkinMatrix;}	

		SkeletonPose*			GetAnimationPose() {return m_pose;}

	protected:
		void					PlayAnimation(Action* pSkelAnim);
	
	private:
		Skeleton*				m_pSkeleton;
		
		SkeletonPose*			m_pose;

		AnimationSet*			m_pAnimSet;

		Action*					m_pSkelAnim;

		D3DXMATRIX				m_arrSkinMatrix[256];
	};
}

#endif

