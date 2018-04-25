#pragma once

namespace ma
{
	class Skeleton;
	class SkeletonPose;
	class SkelAnimtion;
	class AnimationSet;
	class BoneSet;
	class BaseParameter;
	class AnimationNodeOutputPool;
	struct AnimationNodeOutput;
	class ParameterNode;


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

		bool					SetAnimationTree(AnimTreeNode* rootNode, bool activateTree);

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

		bool					IsReady();


		AnimationNodeOutputPool* GetAnimationNodeOutputPool() { return m_animation_node_out_pool; }

		ParameterNode*			GetParameterNode() { return m_prameNode; }

	protected:
		void					EvaluateAnimation();

	private:
		RefPtr<Skeleton>			m_pSkeleton;		
		RefPtr<SkeletonPose>		m_pose;

		RefPtr<AnimationSet>		m_pAnimSet;

		RefPtr<AnimTreeNode>		m_rootNode;

		ParameterNode*				m_prameNode;

		Matrix3x4*					m_arrSkinMatrix;

		bool						m_bLoadOver;	

        std::mutex                  m_csParallelUpdate;

		AnimationNodeOutputPool*	m_animation_node_out_pool = NULL;

		AnimationNodeOutput*		m_treeOutput = NULL;
	};

	RefPtr<AnimationComponent> CreateAnimationComponent();

}



