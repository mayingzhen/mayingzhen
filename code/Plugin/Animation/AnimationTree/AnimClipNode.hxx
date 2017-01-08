#include "AnimClipNode.h"


namespace ma
{

	AnimClipNode::AnimClipNode()
	{
		m_bLoadOver = false;

		m_nStartFrame = 0;
		m_nEndFrame = 0;
		m_fSkaFrame = 0;
	}

	AnimClipNode::~AnimClipNode()
	{
	}

	RefPtr<AnimClipNode> NewClipNode()
	{
		return new AnimClipNode();
	}

	void AnimClipNode::RegisterAttribute()
	{
		REGISTER_OBJECT(AnimClipNode,NewClipNode);

		COPY_BASE_ATTRIBUTES(AnimClipNode,AnimTreeNode);
		ACCESSOR_ATTRIBUTE(AnimClipNode, "AnimationClip", GetAnimationClip, SetAnimationClip, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimClipNode, "StartFrame", GetStartFrame, SetStartFrame, uint32, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimClipNode, "EndFrame", GetEndFrame, SetEndFrame, uint32, 0, AM_DEFAULT);
	}

	void AnimClipNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight, BoneSet* pBoneSet)
	{
		profile_code();

		if (m_pAnimation == NULL || pEvalContext == NULL)
			return;

		UINT uBoneNumber = pBoneSet ? pBoneSet->GetBoneNumber() : pEvalContext->m_arrTSFPS.size();
		for (UINT i = 0; i < uBoneNumber; ++i)
		{
			BoneIndex uBoneId = pBoneSet ? pBoneSet->GetBoneIdByIndex(i) : i;
			BoneIndex nTrackInd = m_NodeLink.MapNode(uBoneId);

			if ( !Math::IsValidID<BoneIndex>(nTrackInd) )
			{
				continue;
			}
			
			Transform source;
			m_pAnimation->SampleSingleTrackByFrame(&source,nTrackInd,m_fSkaFrame);

			Transform& dest = pEvalContext->m_arrTSFPS[uBoneId];

			if (fWeight < 1.0f - 0.00001f)
			{
				if (pEvalContext->m_arrFirst[uBoneId])
				{
					TransformMul(dest, source, fWeight);
				}
				else
				{
					TransformMad(dest, source, fWeight, dest);
				}
			}
			else
			{
				dest = source;
			}

			pEvalContext->m_arrFirst[uBoneId] = false;
		}
	}

	void AnimClipNode::SetFrame(float fFrame)
	{
		AnimTreeNode::SetFrame(fFrame);

		m_fSkaFrame = (float)m_nStartFrame + m_fLocalFrame;
	}

	void AnimClipNode::SetAnimationClip(const char* pszSkaPath)
	{
		m_strSkaName = pszSkaPath ? pszSkaPath : "";
	}

	const char*	AnimClipNode::GetAnimationClip() const
	{
		return m_strSkaName.c_str();
	}

	uint32	AnimClipNode::GetFrameCount()
	{
		UINT nFrameCount = m_nEndFrame - m_nStartFrame + 1;
		ASSERT(nFrameCount != 0);
		if (nFrameCount == 0)
			return 1;

		return nFrameCount;
	}

	bool AnimClipNode::Instantiate(Skeleton* pSkeleton)
	{
		if (m_pAnimation)
			return true;

		ASSERT(!m_strSkaName.empty());
		if (m_strSkaName.empty())
			return false;
	
		ASSERT(pSkeleton && pSkeleton->IsReady());
		m_pSkeleton = pSkeleton;

		m_pAnimation = CreateAnimation( m_strSkaName.c_str(), pSkeleton->GetResPath() ); 

		m_bLoadOver = false;
		return IsReady();
	}

	bool AnimClipNode::IsReady()
	{
		if (m_bLoadOver)
			return true;

		if (m_pAnimation == NULL || m_pSkeleton == NULL)
			return false;

		if (!m_pAnimation->IsReady() )
			return false;

		if (!m_pSkeleton->IsReady() )
			return false;

		m_NodeLink.Build(m_pSkeleton.get(),m_pAnimation.get());

		if (m_nEndFrame == 0)
		{
			m_nEndFrame = m_pAnimation->GetFrameNumber() - 1;
		}
		else
		{
			if (m_nEndFrame >= m_pAnimation->GetFrameNumber())
			{
				LogWarning("m_nEndFrame < m_pAnimData->GetFrameCount()");
			}
			m_nEndFrame = min( m_pAnimation->GetFrameNumber() - 1, m_nEndFrame );
		}

		Animatable::SetFrameCount(this->GetFrameCount());

		m_bLoadOver = true;

		return true;
	}

	RefPtr<AnimClipNode>  CreateClipNode(const char* skaName,const char* pszName)
	{
		AnimClipNode* pClipNode = new AnimClipNode();
		pClipNode->SetName(pszName);
		pClipNode->SetAnimationClip(skaName);
		return pClipNode;
	}

}
