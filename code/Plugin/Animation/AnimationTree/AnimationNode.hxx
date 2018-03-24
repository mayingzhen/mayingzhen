#include "AnimationNode.h"
#include "AnimationNodeOutput.h"
#include "BlendNode.h"
#include "Animator.h"

namespace ma
{
	AnimationNode::AnimationNode(Animator *animator) :
		m_active(false),
		m_timeScale(1.0f),
		m_exitTimeCallback(NULL),
		m_animator(animator),
		nodeName(""),
		m_activeCallback(NULL),
		m_deactiveCallback(NULL),
		m_exitTransition(NULL)
	{
		m_direction[0] = 0;
		m_direction[1] = 0;
		m_direction[2] = 0;
	}

	AnimationNode::~AnimationNode(void)
	{
		ASSERT(m_exitTimeCallback == NULL);
		ASSERT(m_deactiveCallback == NULL);
		ASSERT(m_activeCallback == NULL);
	}

	void AnimationNode::Initial(void)
	{
	}


	UINT AnimationNode::GetBoneCount() const
	{
		return m_animator->GetBoneCount();
	}

	void AnimationNode::Activate(AnimationComponent* pAnimator)
	{
		m_pAnimator = pAnimator;

		if (m_activeCallback)
		{
			//m_animator->m_active_allback_vec.push_back(m_activeCallback);
		}
	}

	void AnimationNode::Deactivate()
	{
		if (m_deactiveCallback)
		{
			//m_animator->m_active_allback_vec.push_back(m_deactiveCallback);
		}
	}

	BoneIndex AnimationNode::GetBoneIndex(const char *boneName) const
	{
		return m_animator->GetSkeleton()->GetBoneIdByName(boneName);
	}

	void AnimationNode::Reset(void)
	{
		SetPhase(0);
	}

	void AnimationNode::SetPhase(float phase)
	{
		m_syncInfo.phase = phase;
	}

	void AnimationNode::ApplyRotation(AnimationNodeOutput &output)
	{
		if (fabs(m_direction[0]) < 1e-5 && fabs(m_direction[1]) < 1e-5 && fabs(m_direction[2]) < 1e-5)
		{
			return;
		}

		Quaternion rot;
		EulerAngleXYZ euler(m_direction[0], m_direction[1], m_direction[2]);
		QuaternionFromEulerAngleXYZ(&rot, &euler);
		output.boneTrans[0].m_qRot = rot * output.boneTrans[0].m_qRot;
	}

	bool AnimationNode::SetExitTimeCallback(float normalizedTime, ExitTimeCallback* callback)
	{
		m_exitTimeCallback = callback;
		return true;
	}

	void AnimationNode::PrintTree(int depth, std::string suffix)
	{
		std::string msg = "";
		std::string prefix = "";
		for (int i = 0; i < depth; ++i)
			prefix += "        ";
		if (depth > 0)
			prefix += "|______ ";
		else
			prefix += "        ";
		char buf[128];
		snprintf(buf, sizeof(buf), " %s, phase=%f, duration=%f, timeScale=%f\n", IsActive() ? "active" : "inactive", GetSyncInfo().phase, GetSyncInfo().duration, m_timeScale);
		std::string extra = buf;
		msg = prefix + GetNodePrint() + " (" + nodeName + ") " + suffix + extra;
		LogInfo(msg.c_str());
	}


	Animator* AnimationNode::GetAnimator(void)
	{
		return m_animator;
	}

	bool AnimationNode::SetActiveNotifier(ActiveCallback* callback)
	{
		m_activeCallback = callback;
		return true;
	}

	bool AnimationNode::SetDeactiveNotifier(ActiveCallback* callback)
	{
		m_deactiveCallback = callback;
		return true;
	}

	bool AnimationNode::SetBoneTreeWeight(std::string subtreeBone, float weight)
	{
		if (!m_blendState)
		{
			ASSERT(false);
			return false;
		}

		if (!m_blendState->SetBoneTreeWeight(subtreeBone, weight))
		{
			ASSERT(false && "[Animator] Can't set bone tree weight.");
			return false;
		}
		return true;
	}


	void ExitTimeCallback::UpdateFrame(float oldPhase, float newPhase)
	{
		if ((oldPhase < newPhase && oldPhase < normalizedTime && normalizedTime <= newPhase) ||
			(oldPhase > newPhase && (oldPhase < normalizedTime || normalizedTime <= newPhase)))
		{
			//m_animator->m_callback_vec.push_back(this);
		}
	}

	void StateEventCallback::Execute()
	{
		//m_animator->onStateLeave(m_signal);
	}

}

