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
		virtual void SetValue(Renderable* pRenderable) = 0;

	protected:
		Uniform*	m_pParameter;
	};

	// Defines a method parameter binding for a single value.
	template <class ClassType, class ReturnType>
	class MethodValueBinding : public MethodBinding
	{
		typedef ReturnType(ClassType::*ValueMethod)() const;

	public:

		MethodValueBinding(Uniform* param, ClassType* instance, ValueMethod valueMethod);

		void SetValue(Renderable* pRenderable);

	private:
		ClassType*		m_pInstance;

		ValueMethod		m_pValueMethod;
	};

	// Defines a method parameter binding for a single value.
	template <class ClassType, class ReturnType, class ParameterType>
	class MethodValueParameterBinding : public MethodBinding
	{
		typedef ReturnType(ClassType::*ValueMethod)(ParameterType) const;
		
	public:

		MethodValueParameterBinding(Uniform* param, ClassType* instance, ValueMethod valueMethod);
		
		void SetValue(Renderable* pRenderable);
	
	private:
		ClassType*		m_pInstance;
		
		ValueMethod		m_pValueMethod;
	};


    // Defines a method parameter binding for an array of values.
    template <class ClassType, class ReturnType, class ParameterType>
    class MethodArrayBinding : public MethodBinding
    {
        typedef ReturnType(ClassType::*ValueMethod)(ParameterType) const;
        typedef UINT (ClassType::*CountMethod)(ParameterType) const;
        
    public:
        
        MethodArrayBinding(Uniform* param, ClassType* instance, ValueMethod valueMethod, CountMethod countMethod);
        
        void SetValue(Renderable* pRenderable);
        
    private:
        ClassType*		m_pInstance;
        
        ValueMethod		m_pValueMethod;
        
        CountMethod		m_nCountMethod;
    };



}

#endif
