#ifndef __Class_H__
#define __Class_H__


#include <Common/Singleton.h>

namespace ma
{
	class COMMON_API Class
	{
		std::string m_className;
		const Class* m_pParentNode;

	public:

		Class(const char* className,const Class* pParent);

		const char* GetName() const;

		const Class* GetParent() const;

		bool IsA(const Class* pAnisister) const;

		void DbgLog() const;

	};

	class COMMON_API ClassManager : public Singleton<ClassManager>
	{
	public:

		ClassManager();

		void Shutdown();

		bool RegisterClass(Class* pCls);

		bool UnRegisterClass(Class* pCls);

		const Class* GetClassByName(const char* clsName);

	private:
		typedef std::map<std::string,Class*> ClassNameMap;
		ClassNameMap m_nameMap;

	};

	inline Class::Class(const char* className,const Class* pParent)
		:m_className(className)
		,m_pParentNode(pParent)
	{

	}

	inline const char* Class::GetName() const
	{
		return m_className.c_str();
	}


	inline const Class* Class::GetParent() const
	{
		return m_pParentNode;
	}


	inline bool Class::IsA(const Class* pAnisister) const
	{
		for (const Class* pCurParent = this; 
			pCurParent != NULL;
			pCurParent = pCurParent->GetParent()
			)
		{
			if (pCurParent == pAnisister)
			{
				return true;
			}
		}
		return false;

	}

}

#endif //__Class_H__
