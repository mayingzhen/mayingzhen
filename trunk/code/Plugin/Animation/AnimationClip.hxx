#include "AnimationClip.h"
#include "BoneSet.h"
#include "AnimEvalContext.h"
#include "BoneMap.h"

namespace ma
{

	AnimationClip::AnimationClip(Animation* pAnimation,Skeleton* pSkeleton)
	{
		m_pAnimation = pAnimation;
		m_fLocalFrame = 0;
		m_fPlaySpeed = 1.0f;
		m_playbackMode = S3L_PLAYBACK_LOOP;
		m_playerStatus = S3L_PLAYER_PLAYING;
		m_pNodeLink = new BoneMap;
		if (pSkeleton)
		{
			m_pNodeLink->Build(pSkeleton,pAnimation);
		}
	}

	AnimationClip::~AnimationClip()
	{
		SAFE_DELETE(m_pNodeLink);
	}

	void AnimationClip::AdvanceTime(float fTimeElapsed)
	{
		if (m_pAnimation == NULL)
			return;

		float fFrameRate = 30.0f;
		m_fLocalFrame += fTimeElapsed * fFrameRate * m_fPlaySpeed;
		
		WrapLocalFrame();
	}

	void AnimationClip::SetFrame(float fFrame)
	{
		m_fLocalFrame = fFrame;

		WrapLocalFrame();
	}

	void AnimationClip::WrapLocalFrame()
	{
		UINT uFrameNumber = m_pAnimation->GetFrameNumber();
		if (m_fLocalFrame > uFrameNumber)
		{
			if (m_playbackMode == S3L_PLAYBACK_LOOP)
			{
				m_fLocalFrame = fmod((float)m_fLocalFrame,(float)uFrameNumber);
			}
			else
			{
				m_playerStatus = S3L_PLAYER_STOP;
			}
		}
	}

	void AnimationClip::EvaluateAnimation(AnimEvalContext* pEvalContext,float fWeight,BoneSet* pBoneSet)
	{
		profile_code();

		if (m_pAnimation == NULL || pEvalContext == NULL)
			return;
		
		UINT uBoneNumber = pBoneSet ? pBoneSet->GetBoneNumber() : pEvalContext->m_arrTSFLS.size();
		for (UINT i = 0; i < uBoneNumber; ++i)
		{
			BoneIndex uBoneId = pBoneSet ? pBoneSet->GetBoneIdByIndex(i) : i;
			BoneIndex nTrackInd = m_pNodeLink->MapNode(uBoneId);
			NodeTransform tsfLS;
			if ( IsInvalidID<BoneIndex>(nTrackInd) )
			{
				TransformSetIdentity(&tsfLS);
			}
			else
			{
				m_pAnimation->SampleSingleTrackByFrame(&tsfLS,nTrackInd,m_fLocalFrame);
			}

			//pEvalContext->m_arrTSFLS[uBoneId] = tsfLS;
			TransformMad(&pEvalContext->m_arrTSFLS[uBoneId], &pEvalContext->m_arrTSFLS[uBoneId], fWeight, &tsfLS);	
		}
	}
}
