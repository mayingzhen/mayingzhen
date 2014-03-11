#include "AnimationClip.h"
#include "BoneSet.h"
#include "AnimEvalContext.h"
#include "BoneMap.h"

namespace ma
{

	AnimationClip::AnimationClip(ref_ptr<Animation> pAnimation,Skeleton* pSkeleton)
	{
		m_pAnimation = pAnimation;
		m_fLocalFrame = 0;
		m_fPlaySpeed = 1.0f;
		m_playbackMode = PLAYBACK_LOOP;
		m_playerStatus = PLAYER_PLAYING;
		m_pNodeLink = new BoneMap;
		if (pSkeleton)
		{
			m_pNodeLink->Build(pSkeleton,pAnimation.get());
		}
	}

	void AnimationClip::SetSkeleton(Skeleton* pSkeleton)
	{
		m_pNodeLink->Build(pSkeleton,m_pAnimation.get());	
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

	void AnimationClip::EvaluateAnimation(AnimEvalContext* pEvalContext,float fWeight,BoneSet* pBoneSet,EBlendMode eBlendMode)
	{
		profile_code();

		if (m_pAnimation == NULL || pEvalContext == NULL)
			return;
		
		UINT uBoneNumber = pBoneSet ? pBoneSet->GetBoneNumber() : pEvalContext->m_arrTSFLS.size();
		for (UINT i = 0; i < uBoneNumber; ++i)
		{
			BoneIndex uBoneId = pBoneSet ? pBoneSet->GetBoneIdByIndex(i) : i;
			BoneIndex nTrackInd = m_pNodeLink->MapNode(uBoneId);

			NodeTransform source;
			if ( IsInvalidID<BoneIndex>(nTrackInd) )
			{
				TransformSetIdentity(&source);
			}
			else
			{
				m_pAnimation->SampleSingleTrackByFrame(&source,nTrackInd,m_fLocalFrame);
			}

			// calculate the blend value, based on cosine interpolation
			//float finalWeight = (1.0f - std::cos(fWeight * PI)) * 0.5f;

			NodeTransform result;
			const NodeTransform& dest = pEvalContext->m_arrTSFLS[uBoneId];
			if (eBlendMode == BLENDMODE_ADDITIVE)
			{
				Quaternion addRot;
				Quaternion indet = QuaternionIden();
				QuaternionSlerp(&addRot,&indet,&source.m_qRot,fWeight);

				// apply relative addition to the result of the previous layer
				result.m_vPos = dest.m_vPos + (source.m_vPos * fWeight);
				result.m_qRot = dest.m_qRot * addRot;
				result.m_fScale	 = dest.m_fScale; //+ (relScale * fWeight);
			}
			else if (eBlendMode == BLENDMODE_OVERWRITE)
			{
				// just blend between the previous and current result, so make no additions
				result.m_vPos = dest.m_vPos + (source.m_vPos - dest.m_vPos) * fWeight;
				//result.m_qRot =  dest.mRotation.Slerp(source.mRotation, fWeight);
				QuaternionSlerp(&result.m_qRot,&dest.m_qRot,&source.m_qRot,fWeight);
				result.m_fScale	 = dest.m_fScale + (source.m_fScale - dest.m_fScale) * fWeight;
			}

			pEvalContext->m_arrTSFLS[uBoneId] = result;
		}
	}
}
