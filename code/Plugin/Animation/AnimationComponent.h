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

 		AnimationSet*			GetAnimationSet() {return m_pAnimSet.get();}

		void					Stop();

		void					PlayAnimation(const char* pszAnimName,float fFadeTime = 0.0f); 
		void					PlayAnimation(uint32 actionID,float fFadeTime = 0.0f);
		void					PlayAnimation(AnimTreeNode* pSkelAnim,float fFadeTime = 0.0f);

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
		void					AdvanceTime(float fTimeElepse);
		void					EvaluateAnimation();

		void					ChangeAnimation(AnimTreeNode* pAnim,float fFadeTime);
	
	private:
		RefPtr<Skeleton>			m_pSkeleton;		
		SkeletonPose*				m_pose;

		RefPtr<AnimationSet>		m_pAnimSet;
		std::string					m_strAnimSetPath;
	
		std::string					m_strCurAction;
		uint32						m_nCurAction;
		RefPtr<AnimTreeNode>		m_pCurAction;
		RefPtr<AnimTreeNode>		m_pPreAction;
		float						m_fCurFadeTime;
		float						m_fFadeTime;

		Matrix3x4*					m_arrSkinMatrix;

		bool						m_bLoadOver;	
	};

}

#endif

