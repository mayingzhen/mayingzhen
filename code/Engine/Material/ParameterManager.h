#pragma once

namespace ma
{
	class Parameter;
	class MethodBinding;

	class ParameterManager
	{
	public:
		ParameterManager();

		~ParameterManager();

		void AddMethodBinding(const char* pszName, MethodBinding* pMethod);
		MethodBinding* GetMethodBinding(const char* pszName);

		template<class ReturnType>
		void AddFunMethodBinding(const char* pszName, const std::function<ReturnType(Renderable*)>& fun)
		{
			MethodBinding* pMethod = new MethodFunBinding<ReturnType>(fun);
			AddMethodBinding(pszName, pMethod);
		}

	private:	
		std::map<std::string, RefPtr<MethodBinding> > m_autoDefaultBings;
	};

	extern ParameterManager* g_pParameterManager;
	ParameterManager*	GetParameterManager();		

}

