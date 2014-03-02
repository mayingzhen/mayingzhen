#ifndef  _AnimationObject__H__
#define  _AnimationObject__H__

namespace ma
{
	class Skeleton;
	class SkeletonPose;
	class Action;

	typedef UINT ActionID;

	class AnimationObject : public IAnimationObject
	{
	public:
		AnimationObject(GameObject* pGameObj);

		~AnimationObject();

		virtual	void			Load(const char* pszAniSetPath, const char* pszSkeletonPath);

		void					Serialize(Serializer& sl, const char* pszLable = "AnimComponent");

 		virtual IAnimationSet*	GetAnimationSet() {return m_pAnimSet;}

		virtual void			PlayAnimation(const char* pszAnimName); 

		virtual void			PlayAnimation(ActionID actionID);

		virtual void			AdvanceTime(float fTimeElepse);

		virtual void			EvaluateAnimation(float fWeight);

		virtual	void			SetFrame(float fFrame);

		SkeletonPose*			GetAnimationPose() {return m_pose;}

		Skeleton*				GetSkeleton() {return m_pSkeleton;}

	protected:
		void					CreateSkeleton(const char* pSkePath);

		void					CreateAniSet(const char* pszAniSetPath);

		void					PlayAnimation(Action* pSkelAnim);
	
	private:
		Skeleton*				m_pSkeleton;
		
		SkeletonPose*			m_pose;

		AnimationSetData*		m_pAnimSetData;

		AnimationSet*			m_pAnimSet;

		Action*					m_pCurAction;

		Matrix4x4*				m_arrSkinMatrix;
	};
}

#endif

