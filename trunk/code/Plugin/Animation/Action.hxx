#include "Action.h"
#include "PoseModifier/PoseModifier.h"
#include "AnimationTree/AnimBlendNode.h"
#include "AnimationTree/AnimClipNode.h"
#include "AnimationTree/AnimLayerNode.h"
#include "ActionData.h"

namespace ma
{
	//IMPL_OBJECT(Action,Object)

	Action::Action(const char* pName)
	{
		m_sAnimName = pName ? pName : "";
		m_pAnimaNode = NULL;
	}

	Action::Action(ActionData* pActionData,Skeleton* pSkeleton)
	{
		m_pSkeleton = pSkeleton;

		m_sAnimName = pActionData->m_sAnimName;

		m_pAnimaNode = CreateAnimNode(pActionData->m_pAnimNodeData);
	}

	Action::~Action()
	{
		SAFE_DELETE(m_pAnimaNode);
	}

	void Action::SetTreeNode(IAnimTreeNode* pAnimNode)
	{
		m_pAnimaNode = pAnimNode;
	}

	void Action::AddPoseModifier(IPoseModifier* pPoseModifier)
	{
		if (pPoseModifier == NULL)
			return;

		m_arrPoseModifier.push_back((PoseModifier*)pPoseModifier);
	}

	void Action::RemovePoseModifier(IPoseModifier* pPoseModifier)
	{
		PoseModifier* pModifier = (PoseModifier*)pPoseModifier;
		if (pModifier == NULL)
			return;

		std::vector<PoseModifier*>::iterator it = std::find(m_arrPoseModifier.begin(),m_arrPoseModifier.end(),pModifier);
		if (it == m_arrPoseModifier.end())
			return;

		m_arrPoseModifier.erase(it);
	}

	void Action::AdvanceTime(float fTimeElepse)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->AdvanceTime(fTimeElepse);
		}
	}

	void Action::EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight)
	{
		profile_code();

		if (m_pAnimaNode)
		{
			m_pAnimaNode->EvaluateAnimation(pAnimContext,fWeight);
		}

		// ApplyLocalSpaceAnimation
		UINT nBoneNumber = pAnimContext->m_pNodePos->GetNodeNumber();
		std::vector<NodeTransform> arrTSfPS;
		arrTSfPS.resize(nBoneNumber);
		for (UINT i = 0; i < nBoneNumber; ++i)
		{
		 	TransformMul(&arrTSfPS[i],&pAnimContext->m_arrTSFLS[i],&pAnimContext->m_refNodePos->GetTransformPS(i));
		}
		pAnimContext->m_pNodePos->SetTransformPSAll(arrTSfPS);
		 

		for (UINT i = 0; i < m_arrPoseModifier.size(); ++i)
		{
			if (m_arrPoseModifier[i] == NULL)
				continue;

			m_arrPoseModifier[i]->UpdatePose(pAnimContext->m_pNodePos);
		}

		pAnimContext->m_pNodePos->SyncObjectSpace();

	}

	void Action::SetFrame(float fFrame)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->SetFrame(fFrame);
		}
	}

	void Action::SetSkeleton(Skeleton* pSkeleton)
	{
		m_pAnimaNode->SetSkeleton(pSkeleton);	
	}

	IAnimTreeNode* Action::CreateAnimNode(AnimNodeData* pAnimNodeData)
	{
		IAnimTreeNode* pAnimNode = NULL;

		if ( pAnimNodeData->IsA( AnimClipNodeData::StaticGetClass() ) )
		{
			AnimClipNodeData* pClipNodeData = SafeCast<AnimClipNodeData>(pAnimNodeData);
			pAnimNode = CreateClipNode(pClipNodeData);
		}
		else if ( pAnimNodeData->IsA( AnimLayerNodeData::StaticGetClass() ) )
		{
			AnimLayerNodeData* pLayerNodeData = SafeCast<AnimLayerNodeData>(pAnimNodeData);
			pAnimNode = CreateLayerNode(pLayerNodeData);
		}
		else if ( pAnimNodeData->IsA( AnimBlendNodData::StaticGetClass() ) )
		{
			AnimBlendNodData* pBlendNodeData = SafeCast<AnimBlendNodData>(pAnimNodeData);
			pAnimNode = CreateBlendNode(pBlendNodeData);
		}

		ASSERT(pAnimNode);
		if (pAnimNode)
			pAnimNode->SetSkeleton(m_pSkeleton);

		return pAnimNode;
	}

	IAnimLayerNode*	Action::CreateLayerNode(AnimLayerNodeData* pLayerData)
	{
		AnimLayerNode* pLayerNode = new AnimLayerNode();

		for (UINT i = 0; i < pLayerData->m_arrAnimNodeData.size(); ++i)
		{
			pLayerNode->AddLayer( CreateAnimNode(pLayerData->m_arrAnimNodeData[i]) );
		}

		return pLayerNode;
	}

	IAnimBlendNode*	Action::CreateBlendNode(AnimBlendNodData* pBlendData)
	{
		AnimBlendNode* pBlendNode = new AnimBlendNode();

		pBlendNode->SetSrcAnimNode( CreateAnimNode(pBlendData->m_pSrcAnimNodeData) );
		pBlendNode->SetDestAnimNode( CreateAnimNode(pBlendData->m_pDestAnimNodeData) );
		pBlendNode->SetWeight(pBlendData->m_fWeight);

		return pBlendNode;
	}

	IAnimClipNode*	Action::CreateClipNode(AnimClipNodeData* pClipData)
	{
		AnimClipNode* pClipNode = new AnimClipNode();

		pClipNode->SetAnimationClip(pClipData->m_sClipPath.c_str());
		pClipNode->SetBoneSet(pClipData->m_sBoneSetName.c_str());

		return pClipNode;
	}
}

