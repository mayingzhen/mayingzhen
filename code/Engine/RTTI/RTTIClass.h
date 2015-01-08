#ifndef __RTTIClass_H__
#define __RTTIClass_H__


namespace ma
{
	class ENGINE_API RTTIClass
	{
	public:

		RTTIClass(const char* className,const RTTIClass* pParent);

		const char* GetName() const;

		const RTTIClass* GetParent() const {return m_pParentNode;};

		bool IsA(const RTTIClass* pAnisister) const;

	private:	
		std::string			m_className;

		const RTTIClass*	m_pParentNode;
	};

	class ENGINE_API ClassManager 
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

	ClassManager* GetClassManager();

	void SetClassManager(ClassManager* pClassMang);

}

#endif //__Class_H__
