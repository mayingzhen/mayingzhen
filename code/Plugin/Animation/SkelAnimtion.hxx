#include "SkelAnimtion.h"
#include "PoseModifier/PoseModifier.h"
#include "AnimationTree/AnimBlendNode.h"
#include "AnimationTree/AnimClipNode.h"
#include "AnimationTree/AnimLayerNode.h"
#include "SkelAnimtionData.h"

namespace ma
{
	SkelAnimtion::SkelAnimtion(const char* pName,Skeleton* pSkeleton)
	{
		m_sAnimName = pName ? pName : "";
		m_pAnimaNode = NULL;
		m_pSkeleton = pSkeleton;
		m_pSkelAnimData = NULL;
	}

	SkelAnimtion::SkelAnimtion(SkelAnimData* pSkelAnimData,Skeleton* pSkeleton)
	{
		m_pSkeleton = pSkeleton;

		m_sAnimName = pSkelAnimData->m_sAnimName;

		m_pAnimaNode = CreateAnimNode(pSkelAnimData->m_pAnimNodeData);
	}

	SkelAnimtion::~SkelAnimtion()
	{
		SAFE_DELETE(m_pAnimaNode);
	}

	void SkelAnimtion::SetTreeNode(AnimTreeNode* pAnimNode)
	{
		m_pAnimaNode = pAnimNode;
	}

	void SkelAnimtion::AddPoseModifier(PoseModifier* pPoseModifier)
	{
		if (pPoseModifier == NULL)
			return;

		m_arrPoseModifier.push_back((PoseModifier*)pPoseModifier);
	}

	void SkelAnimtion::RemovePoseModifier(PoseModifier* pPoseModifier)
	{
		PoseModifier* pModifier = (PoseModifier*)pPoseModifier;
		if (pModifier == NULL)
			return;

		std::vector<PoseModifier*>::iterator it = std::find(m_arrPoseModifier.begin(),m_arrPoseModifier.end(),pModifier);
		if (it == m_arrPoseModifier.end())
			return;

		m_arrPoseModifier.erase(it);
	}

	void SkelAnimtion::AdvanceTime(float fTimeElepse)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->AdvanceTime(fTimeElepse);
		}
	}

	void SkelAnimtion::EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight)
	{
		profile_code();

		if (m_pAnimaNode)
		{
			m_pAnimaNode->EvaluateAnimation(pAnimContext,fWeight,BLENDMODE_OVERWRITE);
		}

		// ApplyLocalSpaceAnimation
		pAnimContext->m_pNodePos->InitLocalSpace(pAnimContext->m_arrTSFLS,pAnimContext->m_refNodePos);
		 

		for (UINT i = 0; i < m_arrPoseModifier.size(); ++i)
		{
			if (m_arrPoseModifier[i] == NULL)
				continue;

			m_arrPoseModifier[i]->UpdatePose(pAnimContext->m_pNodePos);
		}

		pAnimContext->m_pNodePos->SyncObjectSpace();

	}

	void SkelAnimtion::SetFrame(float fFrame)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->SetFrame(fFrame);
		}
	}

	AnimTreeNode* SkelAnimtion::CreateAnimNode(AnimNodeData* pAnimNodeData)
	{
		AnimTreeNode* pAnimNode = NULL;

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

		return pAnimNode;
	}

	AnimLayerNode*	SkelAnimtion::CreateLayerNode(AnimLayerNodeData* pLayerData)
	{
		AnimLayerNode* pLayerNode = new AnimLayerNode();

		if (pLayerData)
		{
			for (UINT i = 0; i < pLayerData->m_arrAnimNodeData.size(); ++i)
			{
				pLayerNode->AddLayer( CreateAnimNode(pLayerData->m_arrAnimNodeData[i]) );
			}
		}

		return pLayerNode;
	}

	AnimBlendNode*	SkelAnimtion::CreateBlendNode(AnimBlendNodData* pBlendData)
	{
		AnimBlendNode* pBlendNode = new AnimBlendNode();

		if (pBlendData)
		{
			pBlendNode->SetSrcAnimNode( CreateAnimNode(pBlendData->m_pSrcAnimNodeData) );
			pBlendNode->SetDestAnimNode( CreateAnimNode(pBlendData->m_pDestAnimNodeData) );
			pBlendNode->SetWeight(pBlendData->m_fWeight);
		}

		return pBlendNode;
	}

	AnimClipNode*	SkelAnimtion::CreateClipNode(AnimClipNodeData* pClipData)
	{
		AnimClipNode* pClipNode = new AnimClipNode(m_pSkeleton);

		if (pClipData)
		{
			pClipNode->SetAnimationClip(pClipData->m_sClipPath.c_str());
			pClipNode->SetBoneSet(pClipData->m_sBoneSetName.c_str());
		}

		return pClipNode;
	}

	AnimClipNode*	SkelAnimtion::CreateClipNode(const char* pSkaPath,const char* pBonsetName)
	{
		AnimClipNode* pClipNode = new AnimClipNode(m_pSkeleton);

		pClipNode->SetAnimationClip(pSkaPath);
		pClipNode->SetBoneSet(pBonsetName);
	
		return pClipNode;
	}
}

