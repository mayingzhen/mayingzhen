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
		ACCESSOR_ATTRIBUTE(AnimClipNode, "StartFrame", GetStartFrame, SetStartFrame, uint32_t, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimClipNode, "EndFrame", GetEndFrame, SetEndFrame, uint32_t, 0, AM_DEFAULT);
	}

	void AnimClipNode::EvaluateAnimation(AnimationNodeOutput &output, float fWeight)
	{
		if (m_pAnimation == NULL)
			return;

		uint32_t uBoneNumber = m_pBoneSet ? m_pBoneSet->GetBoneNumber() : GetBoneCount();
		for (uint32_t i = 0; i < uBoneNumber; ++i)
		{
			BoneIndex uBoneId = m_pBoneSet ? m_pBoneSet->GetBoneIdByIndex(i) : i;
			BoneIndex nTrackInd = m_NodeLink.MapNode(uBoneId);

			if ( !Math::IsValidID<BoneIndex>(nTrackInd) )
			{
				continue;
			}
			
			Transform source;
			m_pAnimation->SampleSingleTrackByFrame(&source,nTrackInd,m_fSkaFrame);

			output.boneTrans[uBoneId] = source;
			output.boneMask[uBoneId] = true;
		}
	}

	void AnimClipNode::SetFrame(float fFrame)
	{
		m_player.SetFrame(fFrame);

		m_fSkaFrame = (float)m_nStartFrame + m_player.GetFrame();
	}

	void AnimClipNode::SetAnimationClip(const char* pszSkaPath)
	{
		m_strSkaName = pszSkaPath ? pszSkaPath : "";
	}

	const char*	AnimClipNode::GetAnimationClip() const
	{
		return m_strSkaName.c_str();
	}

	uint32_t	AnimClipNode::GetFrameCount()
	{
		uint32_t nFrameCount = m_nEndFrame - m_nStartFrame + 1;
		ASSERT(nFrameCount != 0);
		if (nFrameCount == 0)
			return 1;

		return nFrameCount;
	}

	void AnimClipNode::Activate()
	{
		ASSERT(m_animator);

		AnimTreeNode::Activate();

		m_player.Play();

		if (m_pAnimation)
			return;

		ASSERT(!m_strSkaName.empty());
		if (m_strSkaName.empty())
			return;

		Skeleton* pSkeleton = m_animator->GetSkeleton();

		ASSERT(pSkeleton && pSkeleton->IsReady());
		m_pSkeleton = pSkeleton;

		m_pAnimation = CreateAnimation(m_strSkaName.c_str(), pSkeleton->GetResPath());

		m_bLoadOver = false;
		IsReady();
	}

	void AnimClipNode::AdvanceTime(float fTimeElepse)
	{
		if (!this->IsReady())
			return;

		if (!m_player.IsPlaying())
			return;

		float fLocalFrame = m_player.Process();
		m_fSkaFrame = (float)m_nStartFrame + fLocalFrame;

		if (m_player.IsStopped() && m_pCallBack)
		{
			m_pCallBack->OnStop(this);
		}

		//ProcessFrameEvent(fLastFrame, m_fLocalFrame);
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

		m_player.SetFrameCount(this->GetFrameCount());

		m_bLoadOver = true;

		return true;
	}

	void AnimClipNode::AddFrameEvent(FrameEvent* pFrameEvent)
	{
		m_vecFrameEvent.push_back(pFrameEvent);
	}

	RefPtr<AnimClipNode>  CreateClipNode(const char* skaName,const char* pszName)
	{
		AnimClipNode* pClipNode = new AnimClipNode();
		pClipNode->SetName(pszName);
		pClipNode->SetAnimationClip(skaName);
		return pClipNode;
	}

}
