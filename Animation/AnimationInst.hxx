namespace ma
{

	AnimationInst::AnimationInst(Animation* pAnimation)
	{
		m_pAnimation = pAnimation;
		m_fLocalFrame = 0;
		m_fPlaySpeed = 1.0f;
	}

	AnimationInst::~AnimationInst()
	{

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

	void AnimationInst::EvaluateAnimation(std::vector<maNodeTransform>&m_arrTSFLS,float fWeight)
	{
		if (m_pAnimation == NULL)
			return;

		for (xmUint uBoneId = 0; uBoneId < m_arrTSFLS.size(); ++uBoneId)
		{
			m_pAnimation->SampleAndAddSingleTrackByFrame(&m_arrTSFLS[uBoneId],uBoneId,fWeight,m_fLocalFrame);
		}
	}
}
