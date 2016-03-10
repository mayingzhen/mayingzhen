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
		ACCESSOR_ATTRIBUTE(AnimClipNode, "BoneSet", GetBoneSet, SetBoneSet, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimClipNode, "StartFrame", GetStartFrame, SetStartFrame, uint32, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimClipNode, "EndFrame", GetEndFrame, SetEndFrame, uint32, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimClipNode, "RefSkeleton", GetRefSkeleton, SetRefSkeleton, const char*, NULL, AM_DEFAULT);
	}

	void AnimClipNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight)
	{
		profile_code();

		if (m_pAnimation == NULL || pEvalContext == NULL)
			return;

		UINT uBoneNumber = m_pBoneSet ? m_pBoneSet->GetBoneNumber() : pEvalContext->m_arrTSFPS.size();
		for (UINT i = 0; i < uBoneNumber; ++i)
		{
			BoneIndex uBoneId = m_pBoneSet ? m_pBoneSet->GetBoneIdByIndex(i) : i;
			BoneIndex nTrackInd = m_NodeLink.MapNode(uBoneId);

			Transform source;
			if ( Math::IsValidID<BoneIndex>(nTrackInd) )
			{
				m_pAnimation->SampleSingleTrackByFrame(&source,nTrackInd,m_fSkaFrame);
			}
			else
			{
				source = pEvalContext->m_refNodePos->GetTransformPS(uBoneId);
			}

			Transform& dest = pEvalContext->m_arrTSFPS[uBoneId];

			if (fWeight < 1.0f - 0.00001f)
			{
				TransformLerp(dest, dest, fWeight, source);
			}
			else
			{
				dest = source;
			}
		}
	}

	void AnimClipNode::SetFrame(float fFrame)
	{
		AnimTreeNode::SetFrame(fFrame);

		m_fSkaFrame = (float)m_nStartFrame + m_fLocalFrame;

		LogInfo("m_fSkaFrmae %f",m_fSkaFrame);
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

	void AnimClipNode::SetRefSkeleton(const char* pszRefSkeleton)
	{
		if (pszRefSkeleton == NULL)
			return;

		m_pRefSkeleton = CreateSkeleton(pszRefSkeleton);
	}

	const char*	AnimClipNode::GetRefSkeleton() const
	{
		return m_pRefSkeleton ? m_pRefSkeleton->GetResPath() : NULL;
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
		if (m_nEndFrame <= 0)
		{
			return m_pAnimation->GetFrameNumber();
		}
		else
		{
			UINT nFrameCount = m_nEndFrame - m_nStartFrame + 1;
			ASSERT(nFrameCount != 0);
			if (nFrameCount == 0)
				return 1;

			return nFrameCount;
		}
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

		m_pAnimation = CreateAnimation( m_strSkaName.c_str(), pSkeleton->GetResPath(), GetRefSkeleton() ); 

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

	RefPtr<AnimClipNode>  CreateClipNode(const char* skaName,const char* pszName,const char* boneSetName,const char* pRefSke)
	{
		AnimClipNode* pClipNode = new AnimClipNode();
		pClipNode->SetName(pszName);
		pClipNode->SetAnimationClip(skaName);
		pClipNode->SetBoneSet(boneSetName);
		pClipNode->SetRefSkeleton(pRefSke);
		return pClipNode;
	}

}
