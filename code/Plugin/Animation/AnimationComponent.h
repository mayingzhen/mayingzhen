#ifndef  _AnimationComponent__H__
#define  _AnimationComponent__H__

namespace ma
{
	class Skeleton;
	class SkeletonPose;
	class SkelAnimtion;
	class AnimationSet;

	class AnimationComponent : public Component
	{
		
	public:
		AnimationComponent();

		~AnimationComponent();

		DECL_OBJECT(AnimationComponent)

		static void				RegisterAttribute();	

		void					Update();
		void					ParallelUpdate();
		void					EndParallelUpdate();

		void					Load(const char* pszAniSetPath, const char* pszSkeletonPath);

		void					Stop();

		void					PlayAnimation(const char* pszAnimName); 
		void					PlayAnimation(uint32 actionID);
		void					PlayAnimation(AnimTreeNode* pSkelAnim);

		void					DebugRender(bool bDrawBoneName = false);

		void					UpdateSkinMatrix();

		SkeletonPose*			GetAnimationPose() {return m_pose;}
		Skeleton*				GetSkeleton() {return m_pSkeleton.get();}
		const char*				GetSkeletonPath() const;
		void					SetSkeletonPath(const char* pSkePath);

		AnimationSet*			GetAnimationSet() {return m_pAnimSet.get();}
		void					SetAnimationSet(AnimationSet* pAnimSet);
		const char*				GetAnimSetPath() const;
		void					SetAnimSetPath(const char* pAniSetPath);
		
		void					SetGoalWorldSpace(Vector3 vGoalWS);

		bool					IsReady();

	protected:
		void					AdvanceTime(float fTimeElepse);
		void					EvaluateAnimation();

		void					ChangeAnimation(AnimTreeNode* pAnim);
	
	private:
		RefPtr<Skeleton>			m_pSkeleton;		
		SkeletonPose*				m_pose;

		RefPtr<AnimationSet>		m_pAnimSet;
	
		uint32						m_nCurAction;
		RefPtr<AnimTreeNode>		m_pAnimation;
		RefPtr<AnimTreeNode>		m_pPreAnimation;
		float						m_fCurFadeTime;

		Matrix3x4*					m_arrSkinMatrix;

		bool						m_bLoadOver;	

		CriticalSection				m_csParallelUpdate;	
	};

	RefPtr<AnimationComponent> CreateAnimationComponent();

}

#endif

