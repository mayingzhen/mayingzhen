#ifndef  _AnimationComponent__H__
#define  _AnimationComponent__H__

namespace ma
{
	class Skeleton;
	class SkeletonPose;
	class SkelAnimtion;
	class AnimationSet;
	class BoneSet;

	enum TriggerType
	{
		NONE = 0,
		EXECUTE = 1,
		STOP = 2,
		CHANGE = 3,
	};

	struct AnimTriggerInfo
	{
		AnimTriggerInfo()
		{
			anim_idx = 0;
			sk_idx = 0;
			type = TriggerType::NONE;
		}
		UINT anim_idx;
		UINT sk_idx;
		Vector2 key;
		TriggerType type;
	};


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

		bool					SetAnimation(const char* pszName,uint32 nLayerID = 0);
		bool					SetAnimation(uint32 nAnimID,uint32 nLayerID = 0,const char* pszName = NULL);
		bool					SetAnimation(AnimTreeNode* pAnim,uint32 nLayerID = 0);

		void					DebugRender(bool bDrawBoneName = false);

		void					UpdateSkinMatrix();

		SkeletonPose*			GetAnimationPose() {return m_pose.get();}
		Skeleton*				GetSkeleton() {return m_pSkeleton.get();}
		const char*				GetSkeletonPath() const;
		void					SetSkeletonPath(const char* pSkePath);

		AnimationSet*			GetAnimationSet() {return m_pAnimSet.get();}
		void					SetAnimationSet(AnimationSet* pAnimSet);
		const char*				GetAnimSetPath() const;
		void					SetAnimSetPath(const char* pAniSetPath);

		void					SetLayerName(uint32 nLayer,const char* pszName);
		
		void					SetGoalWorldSpace(Vector3 vGoalWS);

		bool					IsReady();

	protected:
		void					EvaluateAnimation();

		void					PlayAnimation(AnimTreeNode* pAnim,uint32 nLayerID);
	
	private:
		RefPtr<Skeleton>			m_pSkeleton;		
		RefPtr<SkeletonPose>		m_pose;

		RefPtr<AnimationSet>		m_pAnimSet;
   
		struct LayerInfo
		{
			RefPtr<AnimTreeNode> m_pAnimation;
			uint32 m_nAnimID;// 异步加载结束解析成m_pAnimation
			string m_strAnimName; // Debug

			RefPtr<AnimTreeNode> m_pPreAnimation;
			float m_fCurFadeTime;

			float m_fFadeFactor;

			string m_strLayerName;
			RefPtr<BoneSet> m_pBoneSet;

			LayerInfo()
			{
				m_nAnimID = -1;
				m_fCurFadeTime = 0;
				m_fFadeFactor = 0;
			}
		};

		vector<LayerInfo> m_arrLayerInfo;

		Matrix3x4*					m_arrSkinMatrix;

		bool						m_bLoadOver;	


// 		std::vector<AnimTriggerInfo> m_trigger_key;
// 		std::vector<AnimTrigger*> m_trigger_vec;       // 模型动作事件触发列表
// 		std::vector<AnimatorCallback *> m_callback_vec;
// 		std::vector<ActiveCallback *> m_active_allback_vec;


		CriticalSection				m_csParallelUpdate;	
	};

	RefPtr<AnimationComponent> CreateAnimationComponent();

}

#endif

