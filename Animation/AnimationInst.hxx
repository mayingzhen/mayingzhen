#include "Animation/AnimationInst.h"
#include "Animation/BoneSet.h"
#include "Animation/AnimEvalContext.h"

namespace ma
{

	AnimationInst::AnimationInst(Animation* pAnimation,Skeleton* pSkeleton)
	{
		m_pAnimation = pAnimation;
		m_fLocalFrame = 0;
		m_fPlaySpeed = 1.0f;
		m_playbackMode = S3L_PLAYBACK_LOOP;
		m_playerStatus = S3L_PLAYER_PLAYING;
		m_pBoneSet = NULL;
		m_pNodeLink = new NodeLink;
		if (pSkeleton)
		{
			m_pNodeLink->Build(pSkeleton,pAnimation);
		}
	}

	AnimationInst::~AnimationInst()
	{
		SAFE_DELETE(m_pNodeLink);
	}

	void AnimationInst::AdvanceTime(float fTimeElapsed)
	{
		if (m_pAnimation == NULL)
			return;

		float fFrameRate = 30.0f;
		m_fLocalFrame += fTimeElapsed * fFrameRate * m_fPlaySpeed;
		
		UINT uFrameNumber = m_pAnimation->GetFrameNumber();
		if (m_fLocalFrame > uFrameNumber)
		{
			if (m_playbackMode == S3L_PLAYBACK_LOOP)
			{
				m_fLocalFrame = 0.0f;
			}
			else
			{
				m_playerStatus = S3L_PLAYER_STOP;
			}
		}
	}

	void AnimationInst::EvaluateAnimation(AnimEvalContext* pEvalContext,float fWeight)
	{
		if (m_pAnimation == NULL || pEvalContext == NULL)
			return;
		
		UINT uBoneNumber = m_pBoneSet ? m_pBoneSet->GetBoneNumber() : pEvalContext->m_arrTSFLS.size();
		for (UINT i = 0; i < uBoneNumber; ++i)
		{
			BoneIndex uBoneId = m_pBoneSet ? m_pBoneSet->GetBoneIdByIndex(i) : i;
			BoneIndex nTrackInd = m_pNodeLink->MapNode(uBoneId);
			//m_pAnimation->SampleAndAddSingleTrackByFrame(&m_arrTSFLS[uBoneId],nTrackInd,fWeight,m_fLocalFrame);
			xmNodeTransform tsfLS;
			m_pAnimation->SampleSingleTrackByFrame(&tsfLS,nTrackInd,m_fLocalFrame);
			pEvalContext->m_arrTSFLS[uBoneId] = tsfLS;
			//maTransformMad(&pEvalContext->m_arrTSFLS[uBoneId], &pEvalContext->m_arrTSFLS[uBoneId], &tsfLS, fWeight);
		}
	}
}
