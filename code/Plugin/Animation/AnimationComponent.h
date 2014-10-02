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
		AnimationComponent(SceneNode* pGameObj);

		~AnimationComponent();

		void					Update();

		virtual	void			Serialize(Serializer& sl, const char* pszLable = "AnimComponent");

		void					Load(const char* pszAniSetPath, const char* pszSkeletonPath);

 		AnimationSet*			GetAnimationSet() {return m_pAnimSet;}

		void					Stop();

		void					PlayAnimation(const char* pszAnimName); 
		void					PlayAnimation(ActionID actionID);

		void					SetFrame(float fFrame);

		void					DebugRender(bool bDrawBoneName = false);

		SkeletonPose*			GetAnimationPose() {return m_pose;}
		Skeleton*				GetSkeleton() {return m_pSkeleton.get();}

		void					UpdateSkinMatrix();

	protected:
		void					PlayAnimation(SkelAnimtion* pSkelAnim,float fFadeTime = 0.0f);

		void					CreateAniSet(void* parm1,void* parm2);
		void					CreateSkeletonPose(void* parm1,void* parm2);

		void					AdvanceTime(float fTimeElepse);
		void					EvaluateAnimation();
	
	private:
		RefPtr<Skeleton>			m_pSkeleton;		
		SkeletonPose*				m_pose;

		RefPtr<AnimationSetData>	m_pAnimSetData;
		AnimationSet*				m_pAnimSet;

		SkelAnimtion*				m_pCurAction;
		SkelAnimtion*				m_pPreAction;
		float						m_fCurFadeTime;
		float						m_fFadeTime;

		Matrix4*					m_arrSkinMatrix;

		typedef tEventListener<AnimationComponent> ELAnimationComponent;
		RefPtr< ELAnimationComponent >	m_pElAniSetLoaded;
		RefPtr< ELAnimationComponent >	m_pElSkeletonLoaded;
	};

	DeclareRefPtr(AnimationComponent);
}

#endif

