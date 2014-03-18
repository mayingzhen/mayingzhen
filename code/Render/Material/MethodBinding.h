#ifndef _MethodBinding_H_
#define _MethodBinding_H_



namespace ma
{
	struct Uniform;
    

	// Interface implemented by templated method bindings for simple storage and iteration.
	class MethodBinding 
	{
		friend struct RenderState;

	public:

		virtual void SetValue(Uniform* pUniform,ShaderProgram* effect) = 0;

	protected:

		MethodBinding(MaterialParameter* param):
		m_pParameter(param), m_bAutoBinding(false)
		{
		}

		virtual ~MethodBinding() 
		{
		}

		MethodBinding& operator=(const MethodBinding&);

	protected:

		MaterialParameter*	m_pParameter;

		bool				m_bAutoBinding;
	};



	// Defines a method parameter binding for a single value.
	template <class ClassType, class ParameterType>
	class MethodValueBinding : public MethodBinding
	{
		typedef ParameterType (ClassType::*ValueMethod)() const;
		
	public:

		MethodValueBinding(MaterialParameter* param, ClassType* instance, ValueMethod valueMethod);
		
		void SetValue(Uniform* pUniform,ShaderProgram* effect);
	
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

		MethodArrayBinding(MaterialParameter* param, ClassType* instance, ValueMethod valueMethod, CountMethod countMethod);
		
		void SetValue(Uniform* pUniform,ShaderProgram* effect);
	
	private:
		ClassType*		m_pInstance;

		ValueMethod		m_pValueMethod;

		CountMethod		m_nCountMethod;
	};


	   
	template <class ClassType, class ParameterType>
	MethodValueBinding<ClassType, ParameterType>::MethodValueBinding(MaterialParameter* param, ClassType* instance, ValueMethod valueMethod) :
		MethodBinding(param), m_pInstance(instance), m_pValueMethod(valueMethod)
	{
	}

	template <class ClassType, class ParameterType>
	void MethodValueBinding<ClassType, ParameterType>::SetValue(Uniform* pUniform,ShaderProgram* effect)
	{
		effect->SetValue(pUniform, (m_pInstance->*m_pValueMethod)());
	}

	template <class ClassType, class ParameterType>
	MethodArrayBinding<ClassType, ParameterType>::MethodArrayBinding(MaterialParameter* param, ClassType* instance, ValueMethod valueMethod, CountMethod countMethod) :
		MethodBinding(param), m_pInstance(instance), m_pValueMethod(valueMethod), m_nCountMethod(countMethod)
	{
	}

	template <class ClassType, class ParameterType>
	void MethodArrayBinding<ClassType, ParameterType>::SetValue(Uniform* pUniform,ShaderProgram* effect)
	{
		effect->SetValue(pUniform, (m_pInstance->*m_pValueMethod)(), (m_pInstance->*m_nCountMethod)());
	}

}

#endif
