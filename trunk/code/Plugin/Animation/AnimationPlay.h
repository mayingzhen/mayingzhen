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
		AnimationPlay();

		~AnimationPlay();

 		virtual	ISkeleton*		GetSkeleton() {return m_pSkeleton;}

 		virtual IAnimationSet*	GetAnimationSet() {return m_pAnimSet;}

		virtual ISkeleton*		CreateSkeleton(const char* pSkePath);

		virtual	IAnimationSet*	CreateAnimSet(const char* pAnimSetPath);

		virtual void			PlayAnimation(const char* pszAnimName); 

		virtual void			PlayAnimation(ActionID actionID);

		virtual void			AdvanceTime(float fTimeElepse);

		virtual void			EvaluateAnimation(float fWeight);

		virtual	void			SetFrame(float fFrame);

		virtual UINT			GetSkinMatrixNumber() {return m_pSkeleton->GetBoneNumer();}

		virtual Matrix4x4*		GetSkinMatrixArray()  {return m_arrSkinMatrix;}	

		SkeletonPose*			GetAnimationPose() {return m_pose;}

	protected:
		void					PlayAnimation(Action* pSkelAnim);
	
	private:
		Skeleton*				m_pSkeleton;
		
		SkeletonPose*			m_pose;

		AnimationSet*			m_pAnimSet;

		Action*					m_pSkelAnim;

		Matrix4x4				m_arrSkinMatrix[256];
	};
}

#endif

