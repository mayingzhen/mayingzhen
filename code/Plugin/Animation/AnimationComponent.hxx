#include "AnimationComponent.h"
#include "AnimationSet.h"
#include "AnimationTree/AnimationNodeOutput.h"

namespace ma
{

	AnimationComponent::AnimationComponent()
	{
		m_pAnimSet = NULL;
		m_pSkeleton = NULL;
		m_pose = NULL;
		m_arrSkinMatrix = NULL;
		
		m_bLoadOver = false;
	
		m_prameNode = new ParameterNode();
	}

	AnimationComponent::~AnimationComponent()
	{
		SAFE_DELETE(m_prameNode);

		SAFE_DELETE_ARRAY(m_arrSkinMatrix);
	}

	void AnimationComponent::RegisterAttribute()
	{
		REGISTER_OBJECT(AnimationComponent,CreateAnimationComponent)

		ACCESSOR_ATTRIBUTE(AnimationComponent, "AnimSetPath", GetAnimSetPath, SetAnimSetPath, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimationComponent, "SkeletonPath", GetSkeletonPath, SetSkeletonPath, const char*, NULL, AM_DEFAULT);
	}

	const char* AnimationComponent::GetSkeletonPath() const
	{
		return m_pSkeleton->GetResPath();
	}

	void AnimationComponent::SetSkeletonPath(const char* pSkePath)
	{
		m_pSkeleton = CreateSkeleton(pSkePath);

		m_bLoadOver = false;
		IsReady();
	}

	const char* AnimationComponent::GetAnimSetPath() const
	{
		if (m_pAnimSet == NULL || m_pAnimSet->GetXMLFile() == NULL)
			return NULL;

		return m_pAnimSet->GetXMLFile()->GetResPath();
	}

	void AnimationComponent::SetAnimationSet(AnimationSet* pAnimSet)
	{
		m_pAnimSet = pAnimSet;

		m_bLoadOver = false;
		IsReady();
	}

	void AnimationComponent::SetAnimSetPath(const char* pAniSetPath)
	{
		SetAnimationSet( CreateAnimationSet(pAniSetPath).get() );
	}

	void AnimationComponent::Load(const char* pszAniSetPath, const char* pszSkeletonPath)
	{
		SetSkeletonPath(pszSkeletonPath);

		SetAnimSetPath(pszAniSetPath);
	}

	bool AnimationComponent::IsReady()
	{
		if (m_bLoadOver)
			return true;

		if ( m_pSkeleton == NULL || !m_pSkeleton->IsReady() )
			return false;

		if ( m_pAnimSet == NULL || !m_pAnimSet->IsReady() )
			return false;

		if (m_pose == NULL)
		{
			const SkeletonPose* pRefPose = m_pSkeleton ? m_pSkeleton->GetResPose() : NULL;
			m_pose = pRefPose ? pRefPose->Clone() : NULL;
			uint32_t nBone = m_pSkeleton->GetBoneNumer();
			m_arrSkinMatrix = new Matrix3x4[nBone];
			for (uint32_t i = 0; i < nBone; ++i)
			{
				m_arrSkinMatrix[i] = Matrix4::IDENTITY;
			}
		}

		// allocate memory on demand
		if (m_treeOutput == NULL)
		{
			m_treeOutput = new AnimationNodeOutput();
			m_treeOutput->Init(m_pSkeleton->GetBoneNumer());
		}

		m_bLoadOver = true;

		return true;
	}

	void AnimationComponent::Update()
	{
		float fTimeElepse = GetTimer()->GetFrameDeltaTime();

		if (m_rootNode)
		{

			ASSERT(m_rootNode);
			if (m_rootNode == NULL)
				return;

			// Clear collected events in the previous frame.
			// Events for the current frame will be collected in the UpdateFrame method of source nodes.
			//m_event_queue.clear();
			//m_trigger_key.clear();

			if (m_rootNode->IsActive())
			{
				m_rootNode->AdvanceTime(fTimeElepse);
			}
		}

		//ClearManualBoneFlags();

		// 动作回调完以后，动作有可能被改变了
		IsReady();

		if ( GetTimer()->GetFrameCount() - m_pSceneNode->GetLastVisibleFrame() > 1 )
			return;

		if (/*GetTaskScheduler()->GetWorkersCount() > 0*/0)
		{
			GetSceneNode()->GetScene()->AddParallelUpdate(this);	
		}
		else
		{
			ParallelUpdate();
			EndParallelUpdate();
		}
	}

	void AnimationComponent::ParallelUpdate()
	{
		EvaluateAnimation();
	}

	void AnimationComponent::EndParallelUpdate()
	{

	}

	bool AnimationComponent::SetAnimationTree(AnimTreeNode* rootNode, bool activateTree)
	{
		if (m_rootNode)
		{
			m_rootNode->Deactivate();
			m_rootNode = NULL;
		}

		m_rootNode = rootNode;
		if (m_rootNode)
		{
			m_rootNode->Init(this);

			if (activateTree)
			{
				m_rootNode->Activate();
			}
		}

		return true;
	}

	thread_local AnimationNodeOutputPool AnimationNodeOutputPool::s_animation_node_output_pool;

	void AnimationComponent::EvaluateAnimation()
	{
		const SkeletonPose* pRefPose = m_pSkeleton->GetResPose();
		if (pRefPose == NULL)
			return;

		if (m_rootNode && m_rootNode->IsActive())
		{
			m_animation_node_out_pool = &AnimationNodeOutputPool::s_animation_node_output_pool;

			m_rootNode->EvaluateAnimation(*m_treeOutput,1);
		}

		for (uint32_t i = 0; i < m_pSkeleton->GetBoneNumer(); i++)
		{
			if (!m_treeOutput->boneMask[i])
			{
				m_treeOutput->boneTrans[i] = pRefPose->GetTransformPS(i);
			}
		}

		m_pose->SetTransformPSAll(m_treeOutput->boneTrans);

		m_pose->SyncObjectSpace();

		UpdateSkinMatrix();	
	}

	void AnimationComponent::UpdateSkinMatrix()
	{
		if (m_pSkeleton == NULL)
			return;

		uint32_t nBoneNum = m_pSkeleton->GetBoneNumer();
		for (uint32_t i = 0; i < nBoneNum; ++i)
		{
			Matrix4 matSkin;
			MatrixFromTransform(&matSkin,& m_pose->GetTransformOS(i));
			matSkin = matSkin * m_pSkeleton->GetBoneMatrixOSInv(i);
			m_arrSkinMatrix[i] = matSkin;
		}

		std::vector<SkinMeshComponent*> arrMeshComp;
		m_pSceneNode->GetTypeComponent<SkinMeshComponent>(arrMeshComp);
		for (uint32_t i = 0; i < arrMeshComp.size(); ++i)
		{
			SkinMeshComponent* pMeshComp = arrMeshComp[i];
			ASSERT(pMeshComp);
			if (pMeshComp == NULL)
				continue;

			pMeshComp->SetSkinMatrix(m_arrSkinMatrix,nBoneNum);
		}
	}

	void AnimationComponent::DebugRender(bool bDrawBoneName/* = false*/)
	{
		if (m_pose)
		{
			m_pose->DebugRender(m_pSceneNode->GetMatrixWS(),bDrawBoneName,m_pSkeleton.get());
		}
	}

	RefPtr<AnimationComponent> CreateAnimationComponent()
	{
		return new AnimationComponent;
	}

}

