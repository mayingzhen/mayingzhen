#pragma once

namespace ma
{
	class AnimationBaseHandler;

	// 用于统一表示整数、浮点数的联合体
	class CondValue
	{
	public:
		CondValue() { m_value.f = 0; }
		CondValue(float value) { m_value.f = value; }

		float Getfloat(void) { return m_value.f; }
		void Setfloat(float value) { m_value.f = value; }

		union
		{
			float f;
		}m_value;
	};

	class BaseParameter : public Referenced
	{

	public:
		BaseParameter(const char* pszName);
		virtual ~BaseParameter();
		void AddHandler(AnimationBaseHandler* handler, uint8_t index);
		virtual bool Equal(CondValue value) = 0;
		virtual bool Greater(CondValue value) = 0;
		virtual BaseParameter *CopyParam() = 0;
		void ValueChanged(void);
		virtual void Reset(void) = 0;
		const char* GetName();
	public:
		std::string m_name;

	protected:
		typedef std::vector < std::pair<uint8_t, AnimationBaseHandler*> > handlerVec;
		handlerVec m_handlers;
	};

	class FloatParameter : public BaseParameter
	{
	public:

		FloatParameter(const char* pszName, float defaultValue);

		virtual bool Equal(CondValue value);
		virtual bool Greater(CondValue value);
		virtual BaseParameter *CopyParam();
		virtual void Reset(void);
		virtual float GetValue();

		void SetValue(float value);
	
	protected:
		float m_value;
		float m_defaultValue;
	};

	class ParameterNode
	{
	public:
		ParameterNode();
		
		~ParameterNode();

		BaseParameter* AddParameter(const char* paramName, float paramValue);
		
		bool SetParameterValue(const char* paramName, float paramValue);
		
		BaseParameter* GetParameter(const char* paramName);
		
		void Reset(void);
	private:
		typedef std::map<std::string, RefPtr<BaseParameter> > ParamMap;
		ParamMap m_paramMap;
	};

}