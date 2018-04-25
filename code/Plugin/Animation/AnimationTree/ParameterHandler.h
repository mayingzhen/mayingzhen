#pragma once

namespace ma
{

	class MachineTransition;
	class MachineCondition;
	class BaseParameter;

	/* 应用于参数变更处理的基类 */

	class AnimationBaseHandler
	{
	public:
		AnimationBaseHandler();
		virtual ~AnimationBaseHandler();

		virtual void AddParameter(BaseParameter *param);

		virtual bool IsActive(void);

		virtual void Handle(void) = 0;
		virtual void Handle(uint8_t index);
		virtual bool CheckParameter(void) = 0;

		float InverseLerp(float from, float to, float value);
		float Lerp(float from, float to, float ratio);
		BaseParameter * GetParameter(uint8_t param_index);


	protected:
		uint8_t m_param_index;
		std::vector<BaseParameter *> m_params;
	};

	/* 应用于状态机约束的基类 */

	enum class Condition
	{
		Greater,
		Less,
		Equal,
		NotEqual,
	};

	static const char* strDECL_Condition[] =
	{
		"Greater",
		"Less",
		"Equal",
		"NotEqual",
		0,
	};

	class BaseConditionHandler : public AnimationBaseHandler
	{
	public:
		BaseConditionHandler(MachineCondition* cond);
		virtual ~BaseConditionHandler();

		static BaseConditionHandler *CreateHandler(Condition mode, MachineCondition *cond, BaseParameter *param);

		virtual void Handle(void);
		virtual bool CheckParameter(void);
		virtual bool Check(void) = 0;

		MachineCondition* m_cond;
	};

	/* 应用于状态机约束的处理逻辑 */

	class TrueConditionHandler : public BaseConditionHandler
	{
	public:
		TrueConditionHandler(MachineCondition* cond) : BaseConditionHandler(cond) {};
		virtual ~TrueConditionHandler() {};
		virtual bool Check(void);
	};

	class FalseConditionHandler : public BaseConditionHandler
	{
	public:
		FalseConditionHandler(MachineCondition* cond) : BaseConditionHandler(cond) {};
		virtual ~FalseConditionHandler() {};
		virtual bool Check(void);
	};

	class GreaterConditionHandler : public BaseConditionHandler
	{
	public:
		GreaterConditionHandler(MachineCondition* cond) : BaseConditionHandler(cond) {};
		virtual ~GreaterConditionHandler() {};
		virtual bool Check(void);
	};

	class LessConditionHandler : public BaseConditionHandler
	{
	public:
		LessConditionHandler(MachineCondition* cond) : BaseConditionHandler(cond) {};
		virtual ~LessConditionHandler() {};
		virtual bool Check(void);
	};

	class EqualConditionHandler : public BaseConditionHandler
	{
	public:
		EqualConditionHandler(MachineCondition* cond) : BaseConditionHandler(cond) {};
		virtual ~EqualConditionHandler() {};
		virtual bool Check(void);
	};

	class UnequalConditionHandler : public BaseConditionHandler
	{
	public:
		UnequalConditionHandler(MachineCondition* cond) : BaseConditionHandler(cond) {};
		virtual ~UnequalConditionHandler() {};
		virtual bool Check(void);
	};

}

