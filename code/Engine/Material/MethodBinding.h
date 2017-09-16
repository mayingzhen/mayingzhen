#ifndef _MethodBinding_H_
#define _MethodBinding_H_



namespace ma
{
	class Uniform;
    
	// Interface implemented by templated method bindings for simple storage and iteration.
	class MethodBinding 
	{
	public:
		MethodBinding(Uniform* param)
		{
			m_pParameter = param;
		}

		virtual ~MethodBinding() 
		{

		}
		virtual void SetValue() = 0;

	protected:
		Uniform*	m_pParameter;
	};


	// Defines a method parameter binding for a single value.
	template <class ClassType, class ParameterType>
	class MethodValueBinding : public MethodBinding
	{
		typedef ParameterType (ClassType::*ValueMethod)() const;
		
	public:

		MethodValueBinding(Uniform* param, ClassType* instance, ValueMethod valueMethod);
		
		void SetValue();
	
	private:
		ClassType*		m_pInstance;
		
		ValueMethod		m_pValueMethod;
	};


	// Defines a method parameter binding for an array of values.
	template <class ClassType, class ParameterType>
	class MethodArrayBinding : public MethodBinding
	{
		typedef ParameterType (ClassType::*ValueMethod)() const;
		typedef UINT (ClassType::*CountMethod)() const;
	
	public:

		MethodArrayBinding(Uniform* param, ClassType* instance, ValueMethod valueMethod, CountMethod countMethod);
		
		void SetValue();
	
	private:
		ClassType*		m_pInstance;

		ValueMethod		m_pValueMethod;

		CountMethod		m_nCountMethod;
	};


	   
	template <class ClassType, class ParameterType>
	MethodValueBinding<ClassType, ParameterType>::MethodValueBinding(Uniform* param, ClassType* instance, ValueMethod valueMethod) :
		MethodBinding(param), m_pInstance(instance), m_pValueMethod(valueMethod)
	{
	}

	template <class ClassType, class ParameterType>
	void MethodValueBinding<ClassType, ParameterType>::SetValue()
	{
		Technique* pTech = m_pParameter->GetParent()->GetParent();
		ASSERT(pTech);
		if (pTech == NULL)
			return;

		pTech->SetValue(m_pParameter, (m_pInstance->*m_pValueMethod)() );
	}

	template <class ClassType, class ParameterType>
	MethodArrayBinding<ClassType, ParameterType>::MethodArrayBinding(Uniform* param, ClassType* instance, ValueMethod valueMethod, CountMethod countMethod) :
		MethodBinding(param), m_pInstance(instance), m_pValueMethod(valueMethod), m_nCountMethod(countMethod)
	{
	}

	template <class ClassType, class ParameterType>
	void MethodArrayBinding<ClassType, ParameterType>::SetValue()
	{
		Technique* pTech = m_pParameter->GetParent()->GetParent();
		ASSERT(pTech);
		if (pTech == NULL)
			return;

		pTech->SetValue(m_pParameter,  (m_pInstance->*m_pValueMethod)(), (m_pInstance->*m_nCountMethod)() );
	}

}

#endif
