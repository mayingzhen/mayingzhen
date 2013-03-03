#ifndef __Class_H__
#define __Class_H__


#include <Common/Singleton.h>

namespace ma
{
	class COMMON_API Class
	{
		std::string m_className;
		const Class* m_pParent;

		//UINT m_nClassID;

	public:

		Class(const char* className,const Class* pParent);

		//UINT GetClassID() const;

		const char* GetName() const;

		const Class* GetParent() const;

		bool IsA(const Class* pAnisister) const;

		void DbgLog() const;

	};

	class COMMON_API ClassManager : public Singleton<ClassManager>
	{
		typedef std::map<std::string,Class*> ClassNameMap;
		//typedef std::map<int,Class*> ClassIDMap;
		ClassNameMap m_nameMap;
		//ClassIDMap m_idMap;

	public:

		ClassManager();

		void Shutdown();

		bool RegisterClass(Class* pCls);

		bool UnRegisterClass(Class* pCls);

		const Class* GetClassByName(const char* clsName);

		//const Class* GetClassById(UINT nClsID);


	};

	inline Class::Class(const char* className,const Class* pParent)
		:m_className(className)
		,m_pParent(pParent)
		//,m_nClassID(nClassID)
	{

	}

// 	inline UINT Class::GetClassID() const
// 	{
// 		return m_nClassID;
// 	}

	inline const char* Class::GetName() const
	{
		return m_className.c_str();
	}


	inline const Class* Class::GetParent() const
	{
		return m_pParent;
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
