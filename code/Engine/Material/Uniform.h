#ifndef __Uniform_H_
#define __Uniform_H_

#include "MethodBinding.h"

namespace ma
{
	
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

		int				m_location;
		int				m_type;
		bool			m_vshShder;
		UINT			m_index;
		UINT			m_nCount; 

		void*		    m_pD3D11CBPtr;
		UINT			m_nCBOffset;
		
	private:
		std::string		m_name;
		MethodBinding*  m_pMethod;
		
	};

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

}

#endif
