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
		AnimationComponent();

		~AnimationComponent();

		static void				RegisterObject(Context* context);	

		void					Update();

		void					Load(const char* pszAniSetPath, const char* pszSkeletonPath);

 		AnimationSet*			GetAnimationSet() {return m_pAnimSet.get();}

		void					Stop();

		void					PlayAnimation(const char* pszAnimName); 
		void					PlayAnimation(ActionID actionID);

		void					SetFrame(float fFrame);

		void					DebugRender(bool bDrawBoneName = false);

		SkeletonPose*			GetAnimationPose() {return m_pose;}
		Skeleton*				GetSkeleton() {return m_pSkeleton.get();}

		void					UpdateSkinMatrix();

		bool					OnLoadOver();

		const char*				GetSkeletonPath() const;
		void					SetSkeletonPath(const char* pSkePath);

		const char*				GetAnimSetPath() const;
		void					SetAnimSetPath(const char* pAniSetPath);

	protected:
		void					PlayAnimation(SkelAnimtion* pSkelAnim,float fFadeTime = 0.0f);

		void					CreateSkeletonPose();

		void					AdvanceTime(float fTimeElepse);
		void					EvaluateAnimation();
	
	private:
		RefPtr<Skeleton>			m_pSkeleton;		
		SkeletonPose*				m_pose;

		RefPtr<AnimationSet>		m_pAnimSet;
		std::string					m_strAnimSetPath;
	
		std::string					m_strCurAction;
		ActionID					m_nCurAction;
		SkelAnimtion*				m_pCurAction;
		SkelAnimtion*				m_pPreAction;
		float						m_fCurFadeTime;
		float						m_fFadeTime;

		Matrix4*					m_arrSkinMatrix;

		bool						m_bLoadOver;	
	};

	DeclareRefPtr(AnimationComponent);
}

#endif

