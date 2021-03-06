#pragma once

#include "Technqiue.h"

namespace ma
{
	class Uniform;
    
	// Interface implemented by templated method bindings for simple storage and iteration.
	class MethodBinding : public Referenced
	{
	public:
		MethodBinding()
		{
		}

		virtual ~MethodBinding() 
		{
		}
		virtual void SetValue(Renderable* pRenderable, SceneContext* pScene, Technique* pTech, Uniform* pParmeter) = 0;
	};

	// Defines a method parameter binding for a single value.
	template <class ReturnType>
	class MethodFunBinding : public MethodBinding
	{
		typedef std::function<ReturnType(Renderable*,SceneContext*)> FUNTYPE;

	public:

		MethodFunBinding(const FUNTYPE& func)
		{
			mFunc = std::move(func);
		}

		void SetValue(Renderable* pRenderable, SceneContext* pScene, Technique* pTech, Uniform* pParmeter)
		{
			pTech->SetValue(pParmeter, mFunc(pRenderable, pScene));
		}

	private:
		FUNTYPE mFunc;
	};


    // Defines a method parameter binding for an array of values.
    template <class ClassType, class ReturnType, class ParameterType>
    class MethodArrayBinding : public MethodBinding
    {
        typedef ReturnType(ClassType::*ValueMethod)(ParameterType) const;
        typedef uint32_t (ClassType::*CountMethod)(ParameterType) const;
        
    public:
        
		MethodArrayBinding(ClassType* instance, ValueMethod valueMethod, CountMethod countMethod)
		{
			m_pInstance = instance;
			m_pValueMethod = valueMethod;
			m_nCountMethod = countMethod;
		}
        
		void SetValue(Renderable* pRenderable, SceneContext* pScene, Technique* pTech, Uniform* pParmeter)
		{
			pTech->SetValue(pParmeter, (m_pInstance->*m_pValueMethod)(pRenderable, pScene), (m_pInstance->*m_nCountMethod)(pRenderable));
		}
        
    private:
        ClassType*		m_pInstance;
        
        ValueMethod		m_pValueMethod;
        
        CountMethod		m_nCountMethod;
    };

}

