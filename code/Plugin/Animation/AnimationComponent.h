#ifndef  _AnimationComponent__H__
#define  _AnimationComponent__H__

namespace ma
{
	class Skeleton;
	class SkeletonPose;
	class SkelAnimtion;
	class AnimationSet;

	typedef UINT ActionID;

	class ANIMATION_API AnimationComponent : public Component
	{
		DECL_OBJECT(AnimationComponent)

	public:
		AnimationComponent(GameObject* pGameObj);

		~AnimationComponent();

		virtual	void			Update();

		virtual	void			Load(const char* pszAniSetPath, const char* pszSkeletonPath);

		virtual	void			Serialize(Serializer& sl, const char* pszLable = "AnimComponent");

 		virtual AnimationSet*	GetAnimationSet() {return m_pAnimSet;}

		virtual void			PlayAnimation(const char* pszAnimName); 

		virtual void			PlayAnimation(ActionID actionID);

		virtual void			AdvanceTime(float fTimeElepse);

		virtual void			EvaluateAnimation(float fWeight);

		virtual	void			SetFrame(float fFrame);

		SkeletonPose*			GetAnimationPose() {return m_pose;}

		virtual Skeleton*		GetSkeleton() {return m_pSkeleton.get();}

	protected:
		void					PlayAnimation(SkelAnimtion* pSkelAnim);
	
	private:
		ref_ptr<Skeleton>			m_pSkeleton;
		
		SkeletonPose*				m_pose;

		ref_ptr<AnimationSetData>	m_pAnimSetData;

		AnimationSet*				m_pAnimSet;

		SkelAnimtion*				m_pCurAction;

		Matrix4x4*					m_arrSkinMatrix;
	};

	DeclareRefPtr(AnimationComponent);
}

#endif

