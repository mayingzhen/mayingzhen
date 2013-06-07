#ifndef __RTTIClass_H__
#define __RTTIClass_H__


#include <Common/Singleton.h>

namespace ma
{
	class COMMON_API RTTIClass
	{
		std::string m_className;
		const RTTIClass* m_pParentNode;

	public:

		RTTIClass(const char* className,const RTTIClass* pParent);

		const char* GetName() const;

		const RTTIClass* GetParent() const;

		bool IsA(const RTTIClass* pAnisister) const;

		void DbgLog() const;

	};

	class COMMON_API ClassManager : public Singleton<ClassManager>
	{
	public:

		ClassManager();

		void Shutdown();

		bool RegisterRTTIClass(RTTIClass* pCls);

		bool UnRegisterRTTIClass(RTTIClass* pCls);

		const RTTIClass* GetClassByName(const char* clsName);

	private:
		typedef std::map<std::string,RTTIClass*> ClassNameMap;
		ClassNameMap m_nameMap;

	};

	inline RTTIClass::RTTIClass(const char* className,const RTTIClass* pParent)
		:m_className(className)
		,m_pParentNode(pParent)
	{

	}

	inline const char* RTTIClass::GetName() const
	{
		return m_className.c_str();
	}


	inline const RTTIClass* RTTIClass::GetParent() const
	{
		return m_pParentNode;
	}


	inline bool RTTIClass::IsA(const RTTIClass* pAnisister) const
	{
		for (const RTTIClass* pCurParent = this; 
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
