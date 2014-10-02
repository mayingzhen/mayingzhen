#ifndef _MaterialParameter_H_
#define _MaterialParameter_H_

#include "ShaderProgram.h"
#include "MethodBinding.h"

namespace ma
{

	class RENDER_API MaterialParameter 
	{
	public:
		MaterialParameter(const char* pName = NULL);

		~MaterialParameter();

		const char* GetName() const {return m_sName.c_str();}

		void		SetValue(const Any& value) {m_anyValue = value;}
		
		const Any&	GetValue() const {return m_anyValue;}

		template <class ClassType, class ParameterType>
		void		BindMethod(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const);

		template <class ClassType, class ParameterType>
		void		BindMethod(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const, UINT (ClassType::*countMethod)() const);

		void		Bind(Uniform* pUniform);

		void		Serialize(Serializer& sl, const char* pszLable = "Parameter");

	private:
		Any				m_anyValue;

		MethodBinding*  m_pMethod;

		std::string		m_sName;
	};

	template <class ClassType, class ParameterType>
	void MaterialParameter::BindMethod(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const)
	{
		SAFE_DELETE(m_pMethod);
		m_pMethod = new MethodValueBinding<ClassType, ParameterType>(this, classInstance, valueMethod);
	}

	template <class ClassType, class ParameterType>
	void MaterialParameter::BindMethod(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const, UINT (ClassType::*countMethod)() const)
	{
		SAFE_DELETE(m_pMethod);
		m_pMethod = new MethodArrayBinding<ClassType, ParameterType>(this, classInstance, valueMethod, countMethod);
	}

}

#endif
