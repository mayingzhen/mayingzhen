#include "AnimClipNode.h"


namespace ma
{

	AnimClipNode::AnimClipNode()
	{
		m_bLoadOver = false;

		m_fLocalFrame = 0;
		m_fPlaySpeed = 1.0f;
		m_playbackMode = PLAYBACK_LOOP;
		m_playerStatus = PLAYER_PLAYING;

		m_nStartFrame = 0;
		m_nTotalFrame = 0;
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
		ACCESSOR_ATTRIBUTE(AnimClipNode, "BoneSet", GetBoneSet, SetBoneSet, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimClipNode, "StartFrame", GetStartFrame, SetStartFrame, uint32, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimClipNode, "TotalFrame", GetTotalFrame, SetTotalFrame, uint32, 0, AM_DEFAULT);
	}

	void AnimClipNode::AdvanceTime(float fTimeElapsed)
	{
		float fFrameRate = 30.0f;
		m_fLocalFrame += m_nStartFrame + fTimeElapsed * fFrameRate * m_fPlaySpeed;

		WrapLocalFrame();
	}

	void AnimClipNode::WrapLocalFrame()
	{
		if (m_pAnimation == NULL)
			return;

		UINT uFrameNumber = m_pAnimation->GetFrameNumber();
		if (m_fLocalFrame > uFrameNumber)
		{
			if (m_playbackMode == PLAYBACK_LOOP)
			{
				m_fLocalFrame = fmod((float)m_fLocalFrame,(float)uFrameNumber);
			}
			else
			{
				m_playerStatus = PLAYER_STOP;
			}
		}
	}

	void AnimClipNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight, EBlendMode eBlendMode)
	{
		profile_code();

		if (m_pAnimation == NULL || pEvalContext == NULL)
			return;

		UINT uBoneNumber = m_pBoneSet ? m_pBoneSet->GetBoneNumber() : pEvalContext->m_arrTSFLS.size();
		for (UINT i = 0; i < uBoneNumber; ++i)
		{
			BoneIndex uBoneId = m_pBoneSet ? m_pBoneSet->GetBoneIdByIndex(i) : i;
			BoneIndex nTrackInd = m_NodeLink.MapNode(uBoneId);

			Transform source;
			if ( Math::IsValidID<BoneIndex>(nTrackInd) )
			{
				m_pAnimation->SampleSingleTrackByFrame(&source,nTrackInd,m_fLocalFrame);
			}

			Transform& dest = pEvalContext->m_arrTSFLS[uBoneId];

			if (fWeight < 1.0f - 0.00001f)
			{
				if (eBlendMode == BLENDMODE_ADDITIVE) // apply relative addition to the result of the previous layer
				{
					TransformMad(dest, source, fWeight, dest);
				}
				else if (eBlendMode == BLENDMODE_OVERWRITE) // just blend between the previous and current result, so make no additions
				{
					TransformLerp(dest, dest, fWeight, source);
				}
			}
			else
			{
				dest = source;
			}
		}
	}

	void AnimClipNode::SetFrame(float fFrame)
	{
		m_fLocalFrame = m_nStartFrame + fFrame;

		WrapLocalFrame();
	}

	const char*	AnimClipNode::GetBoneSet() const
	{
		return m_pBoneSet ? m_pBoneSet->GetBoneSetName() : NULL;
	}

	void AnimClipNode::SetBoneSet(const char* pBoneSetName)
	{
		m_strBoneSetName = pBoneSetName ? pBoneSetName : "";

		if (m_pSkeleton)
			m_pBoneSet = m_pSkeleton->GetBoneSetByName(pBoneSetName);

	}

	void AnimClipNode::SetAnimationClip(const char* pszSkaPath)
	{
		m_strSkaName = pszSkaPath ? pszSkaPath : "";
	}

	const char*	AnimClipNode::GetAnimationClip() const
	{
		return m_strSkaName.c_str();
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

		if (!m_strBoneSetName.empty())
			m_pBoneSet = m_pSkeleton->GetBoneSetByName( m_strBoneSetName.c_str() );

		m_bLoadOver = true;

		return true;
	}

	RefPtr<AnimClipNode>  CreateClipNode(const char* skaName,const char* pszName,const char* boneSetName)
	{
		AnimClipNode* pClipNode = new AnimClipNode();
		pClipNode->SetName(pszName);
		pClipNode->SetAnimationClip(skaName);
		pClipNode->SetBoneSet(boneSetName);
		return pClipNode;
	}

}
