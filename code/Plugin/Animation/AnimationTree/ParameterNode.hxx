#include "ParameterNode.h"
#include "ParameterHandler.h"

namespace ma
{

	/* 参数的基类 */
	BaseParameter::BaseParameter(const char* pszName) :
		m_name(pszName)
	{
	}

	BaseParameter::~BaseParameter()
	{
		m_handlers.clear();
	}

	void BaseParameter::AddHandler(AnimationBaseHandler* handler, uint8 index)
	{
		m_handlers.push_back(std::make_pair(index, handler));
	}

	void BaseParameter::ValueChanged(void)
	{
		for (handlerVec::iterator it = m_handlers.begin(); it != m_handlers.end(); ++it)
		{
			if (it->second->IsActive())
				it->second->Handle(it->first);
		}
	}

	const char* BaseParameter::GetName()
	{ 
		return m_name.c_str(); 
	}

	/* 浮点型参数 */
	FloatParameter::FloatParameter(const char* pszName, float defaultValue) :
		BaseParameter(pszName)
	{
		m_value = m_defaultValue = defaultValue;
	}

	bool FloatParameter::Equal(CondValue value)
	{
		return (m_value == value.Getfloat());
	}

	bool FloatParameter::Greater(CondValue value)
	{
		return (m_value > value.Getfloat());
	}

	float FloatParameter::GetValue()
	{
		return m_value;
	}

	void FloatParameter::SetValue(float value)
	{
		if (std::abs(m_value - value) > 1e-5)
		{
			m_value = value;
			ValueChanged();
		}
	}

	BaseParameter *FloatParameter::CopyParam()
	{
		return new FloatParameter(m_name.c_str(), m_defaultValue);
	}

	void FloatParameter::Reset(void)
	{
		SetValue(m_defaultValue);
	}

	// 动画树中参数管理
	ParameterNode::ParameterNode() 
	{
	}

	ParameterNode::~ParameterNode()
	{
	}

	BaseParameter* ParameterNode::AddParameter(const char* paramName, float paramValue)
	{
		auto it = m_paramMap.find(paramName);
		ASSERT(it == m_paramMap.end());

		FloatParameter* pParame = new FloatParameter(paramName, paramValue);
		m_paramMap[paramName] = pParame;
		return pParame;
	}

	BaseParameter* ParameterNode::GetParameter(const char* paramName)
	{
		auto it = m_paramMap.find(paramName);
		if (it != m_paramMap.end())
			return it->second.get();
		return NULL;
	}

	bool ParameterNode::SetParameterValue(const char* paramName, float paramValue)
	{
		BaseParameter* pParameter = this->GetParameter(paramName);
		ASSERT(pParameter);
		if (pParameter == NULL)
			return false;

		FloatParameter* pFloatParm = dynamic_cast<FloatParameter*>(pParameter);
		ASSERT(pFloatParm);
		if (pFloatParm == NULL)
			return false;

		pFloatParm->SetValue(paramValue);

		return true;
	}

	void ParameterNode::Reset(void)
	{
		for (ParamMap::iterator it = m_paramMap.begin(); it != m_paramMap.end(); ++it)
		{
			it->second->Reset();
		}
	}
}

