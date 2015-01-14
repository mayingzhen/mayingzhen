#include "AnimationClip.h"
#include "BoneSet.h"
#include "AnimEvalContext.h"
#include "BoneMap.h"

namespace ma
{

	AnimationClip::AnimationClip()
	{
		m_pAnimation = NULL;
		m_fLocalFrame = 0;
		m_fPlaySpeed = 1.0f;
		m_playbackMode = PLAYBACK_LOOP;
		m_playerStatus = PLAYER_PLAYING;
		m_pSkeleton = NULL;
		m_bLoadOver = false;
	}

	void AnimationClip::SetSkeleton(Skeleton* pSkeleton)
	{
		m_pSkeleton = pSkeleton;
		
		if (m_pAnimation->IsInited())
		{
			m_NodeLink.Build(m_pSkeleton.get(),m_pAnimation.get());
		}
	}

	AnimationClip::~AnimationClip()
	{
	}

	void AnimationClip::SetAnimationPath(const char* pszSka)
	{
		m_pAnimation = DeclareResource<Animation>(pszSka);		
	}

	const char* AnimationClip::GetAnimationPath() const
	{
		return m_pAnimation ? m_pAnimation->GetResPath() : NULL;
	}

	void AnimationClip::Instance()
	{
		ASSERT(m_pAnimation);
		if (m_pAnimation == NULL)
			return;

		m_pAnimation->Load();
		
		OnLoadOver();
	}

	bool AnimationClip::OnLoadOver()
	{
		if (m_bLoadOver)
			return true;

		ASSERT(m_pAnimation && m_pSkeleton);
		if (m_pAnimation == NULL || m_pSkeleton == NULL)
			return false;

		if (!m_pAnimation->IsInited() )
			return false;

		if (!m_pSkeleton->IsInited() )
			return false;

		m_NodeLink.Build(m_pSkeleton.get(),m_pAnimation.get());

		m_bLoadOver = true;

		return true;
	}

	void AnimationClip::AdvanceTime(float fTimeElapsed)
	{	
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

	void AnimationClip::EvaluateAnimation(AnimEvalContext* pEvalContext,float fWeight,BoneSet* pBoneSet,EBlendMode eBlendMode)
	{
		profile_code();

		if (m_pAnimation == NULL || pEvalContext == NULL)
			return;

		UINT uBoneNumber = pBoneSet ? pBoneSet->GetBoneNumber() : pEvalContext->m_arrTSFLS.size();
		for (UINT i = 0; i < uBoneNumber; ++i)
		{
			BoneIndex uBoneId = pBoneSet ? pBoneSet->GetBoneIdByIndex(i) : i;
			BoneIndex nTrackInd = m_NodeLink.MapNode(uBoneId);

			Transform source;
			if ( Math::IsValidID<BoneIndex>(nTrackInd) )
			{
				m_pAnimation->SampleSingleTrackByFrame(&source,nTrackInd,m_fLocalFrame);
			}

			// calculate the blend value, based on cosine interpolation
			//float finalWeight = (1.0f - std::cos(fWeight * PI)) * 0.5f;

			Transform result;
			const Transform& dest = pEvalContext->m_arrTSFLS[uBoneId];
			if (eBlendMode == BLENDMODE_ADDITIVE)
			{
				Quaternion addRot = Quaternion::Slerp(fWeight,Quaternion::IDENTITY,source.m_qRot);

				// apply relative addition to the result of the previous layer
				result.m_vPos = dest.m_vPos + (source.m_vPos * fWeight);
				result.m_qRot = dest.m_qRot * addRot;
				//result.m_fScale	 = Vector3::UNIT_SCALE * dest.m_fScale; //+ (relScale * fWeight);
			}
			else if (eBlendMode == BLENDMODE_OVERWRITE)
			{
				// just blend between the previous and current result, so make no additions
				result.m_vPos = Math::Lerp(dest.m_vPos,source.m_vPos,fWeight);//dest.m_vPos + (source.m_vPos - dest.m_vPos) * fWeight;
				result.m_qRot = Quaternion::Slerp(fWeight,dest.m_qRot,source.m_qRot);
				//result.m_fScale	 = dest.m_vScale.sl dest.m_fScale + (source.m_fScale - dest.m_fScale) * fWeight;
			}

			pEvalContext->m_arrTSFLS[uBoneId] = result;
		}
	}
}
