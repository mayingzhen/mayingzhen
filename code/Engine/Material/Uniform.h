#ifndef __Uniform_H_
#define __Uniform_H_

#include "MethodBinding.h"

namespace ma
{
	class ConstantBuffer : public Referenced
	{
	public:

		virtual void SetParameter(unsigned offset, unsigned size, const void* data){}

		UINT		GetSize() { return m_nSize; }
		void		SetSize(uint32 nSize) { m_nSize = nSize; }

		UINT		GetBound() { return m_nBound; }
		void		SetBound(UINT nBound) { m_nBound = nBound; }

		Uniform*	AddUniform(const char* pName);
		UINT		GetUniformCount();
		Uniform*	GetUniformByIndex(UINT nIndex);

		Technique*	GetParent() { return m_pTech; }
		void		SetParent(Technique* pTech) { m_pTech = pTech; }

		void		SetName(const char* pszName);
		const char* GetName();

		RefPtr<ConstantBuffer> Clone();
		
	private:
		std::string m_strName;

		UINT m_nBound = 0;

		UINT m_nSize = 0;

		Technique* m_pTech = NULL;

		std::vector< RefPtr<Uniform> > m_vecUniform;
	};

	RefPtr<ConstantBuffer> CreateConstantBuffer(UINT nSize, uint32 nBound);
	
	class Uniform : public Referenced
	{
	public:
		Uniform();

		~Uniform();

		template <class ClassType, class ParameterType>
		void		BindMethod(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const);

		template <class ClassType, class ParameterType>
		void		BindMethod(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const, UINT (ClassType::*countMethod)() const);
		
		void		Bind(); 

		const char*	GetName() const;

		void		SetName(const char* pszName);

		uint32		GetIndex() {return m_index;}
		void		SetIndex(UINT nIndex) { m_index = nIndex; }

		void		SetParent(ConstantBuffer* pCB) { m_pCBPtr = pCB; }
		ConstantBuffer* GetParent() { return m_pCBPtr; }

		void		SetTechnique(Technique* pTech) { m_pTech = pTech; }
		Technique*	GetTechnique() { return m_pTech; }

		UINT		GetOffset() { return m_nOffSetInCB; }
		void		SetOffset(UINT nOffset) { m_nOffSetInCB = nOffset;}

		UINT		GetSize() { return m_nSizeInCB; }
		void		SetSize(UINT nSize) { m_nSizeInCB = nSize; }

	private:
		std::string		m_name;
		UINT			m_index = 0;

		ConstantBuffer* m_pCBPtr = NULL;
		UINT			m_nOffSetInCB = 0;
		UINT			m_nSizeInCB = 0;
		Technique*		m_pTech = NULL;

		MethodBinding*  m_pMethod = NULL;
	};

	RefPtr<Uniform> CreateUniform(const char* pszName);
    
 
	template <class ClassType, class ParameterType>
	void Uniform::BindMethod(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const)
	{
		SAFE_DELETE(m_pMethod);
		m_pMethod = new MethodValueBinding<ClassType, ParameterType>(this, classInstance, valueMethod);
	}

	template <class ClassType, class ParameterType>
	void Uniform::BindMethod(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const, UINT (ClassType::*countMethod)() const)
	{
		SAFE_DELETE(m_pMethod);
		m_pMethod = new MethodArrayBinding<ClassType, ParameterType>(this, classInstance, valueMethod, countMethod);
	}
    
	   
    template <class ClassType, class ParameterType>
    MethodValueBinding<ClassType, ParameterType>::MethodValueBinding(Uniform* param, ClassType* instance, ValueMethod valueMethod) :
    MethodBinding(param), m_pInstance(instance), m_pValueMethod(valueMethod)
    {
    }
    
    template <class ClassType, class ParameterType>
    void MethodValueBinding<ClassType, ParameterType>::SetValue()
    {
        Technique* pTech = m_pParameter->GetTechnique() ? m_pParameter->GetTechnique() : m_pParameter->GetParent()->GetParent();
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
        Technique* pTech = m_pParameter->GetTechnique() ? m_pParameter->GetTechnique() : m_pParameter->GetParent()->GetParent();
        ASSERT(pTech);
        if (pTech == NULL)
            return;
        
        pTech->SetValue(m_pParameter,  (m_pInstance->*m_pValueMethod)(), (m_pInstance->*m_nCountMethod)() );
    }

}

#endif

