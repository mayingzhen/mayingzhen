#include "ParameterHandler.h"
#include "ParameterNode.h"
#include "StateMachineNode.h"

namespace ma
{

	/* 应用于动画树的参数处理基类 */

	AnimationBaseHandler::AnimationBaseHandler()
	{
		m_param_index = 0;
	}

	AnimationBaseHandler::~AnimationBaseHandler()
	{
		m_params.clear();
	}

	void AnimationBaseHandler::AddParameter(BaseParameter *param)
	{
		m_params.push_back(param);
		param->AddHandler(this, m_param_index);
		m_param_index++;
	}

	float AnimationBaseHandler::InverseLerp(float from, float to, float value)
	{
		float ans = 0;
		if (std::abs(from - to) > 1e-5)
			ans = (value - from) / (to - from);
        return std::min(1.0f, std::max(0.0f, ans));
	}

	float AnimationBaseHandler::Lerp(float from, float to, float ratio)
	{
        ratio = std::min(1.0f, std::max(0.0f, ratio));
		return from * (1.0f - ratio) + to * ratio;
	}

	void AnimationBaseHandler::Handle(uint8_t index)
	{
		Handle();
	}

	BaseParameter * AnimationBaseHandler::GetParameter(uint8_t param_index)
	{
		if (param_index >= m_params.size())
			return NULL;
		return m_params[param_index];
	}

	bool AnimationBaseHandler::IsActive(void)
	{
		return true;	// 默认是激活状态，不接受剪枝
	}

	/* 应用于状态机约束的处理逻辑 */

	BaseConditionHandler::BaseConditionHandler(MachineCondition* cond) :
		AnimationBaseHandler(),
		m_cond(cond)
	{
	}

	BaseConditionHandler::~BaseConditionHandler()
	{
		m_cond = NULL;
	}

	BaseConditionHandler *BaseConditionHandler::CreateHandler(Condition mode, MachineCondition *cond, BaseParameter *param)
	{
		BaseConditionHandler *handler = NULL;
		
		if (mode == Condition::Greater)
			handler = new GreaterConditionHandler(cond);
		else if (mode == Condition::Less)
			handler = new LessConditionHandler(cond);
		else if (mode == Condition::Equal)
			handler = new EqualConditionHandler(cond);
		else if (mode == Condition::NotEqual)
			handler = new UnequalConditionHandler(cond);
		else
		{
			LogError("[ParamHandler] Invalid Condition Handler type: %s", mode);
			return NULL;
		}
		handler->AddParameter(param);
		return handler;
	}

	bool BaseConditionHandler::CheckParameter(void)
	{
		if (m_params.size() != 1)
			return false;
		return true;
	}

	void BaseConditionHandler::Handle(void)
	{
		m_cond->HandleEvent();
	}

	/* 各种约束的处理逻辑 */

	bool TrueConditionHandler::Check(void)
	{
		return m_params[0]->Equal(CondValue(true));
	}

	bool FalseConditionHandler::Check(void)
	{
		return m_params[0]->Equal(CondValue(false));
	}

	bool GreaterConditionHandler::Check(void)
	{
		return m_params[0]->Greater(m_cond->m_threshold);
	}

	bool LessConditionHandler::Check(void)
	{
		return !(m_params[0]->Greater(m_cond->m_threshold));
	}

	bool EqualConditionHandler::Check(void)
	{
		return m_params[0]->Equal(m_cond->m_threshold);
	}

	bool UnequalConditionHandler::Check(void)
	{
		return !(m_params[0]->Equal(m_cond->m_threshold));
	}

}

