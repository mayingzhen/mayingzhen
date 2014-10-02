#ifndef  _Object__H__
#define  _Object__H__

#include "Engine/RTTI/RTTIClass.h"


#define DECL_OBJECT(ClassName) private:\
	static RTTIClass* ms_p##ClassName##Class;\
	public:\
	static void StaticInitClass();\
	static void StaticShutdownClass();\
	static const RTTIClass*		StaticGetClass();\
	virtual RTTIClass*	GetClass() const;\
	private:

#define IMPL_OBJECT(ClassName,ParentName) RTTIClass* ClassName::ms_p##ClassName##Class = NULL; \
	void ClassName::StaticInitClass(){\
	/*std::string*/const char* className = #ClassName;\
	ms_p##ClassName##Class = new RTTIClass(className,ParentName::StaticGetClass());\
	ClassManager::GetInstance().RegisterRTTIClass(ms_p##ClassName##Class); \
	}\
	void ClassName::StaticShutdownClass(){ \
	ClassManager::GetInstance().UnRegisterRTTIClass(ms_p##ClassName##Class); \
	SAFE_DELETE(ms_p##ClassName##Class);\
	}\
	const RTTIClass*		ClassName::StaticGetClass(){\
	return ms_p##ClassName##Class;\
	}\
	RTTIClass*	ClassName::GetClass() const{\
	return ms_p##ClassName##Class;\
	}


namespace ma
{
	class RTTIClass;

	class ENGINE_API Object : public Referenced
	{
		DECL_OBJECT(Object)

	public:
		Object();

		Object(const char* pName);
		
		virtual ~Object();
	
		const char*		GetName() {return m_sName.c_str();}

		void			SetName(const char* pName) {m_sName = pName;}

		bool			IsA(const RTTIClass* pParent) const;

	protected:
		std::string		m_sName;
	};


	template<class T>
	T*		SafeCast( Object* pObj)
	{
		T* ret = NULL;
		if (NULL != pObj && pObj->IsA(T::StaticGetClass()))
		{
			ret = reinterpret_cast<T*>(pObj);
		}
		return ret;
	}

	template<class T>
	const T*		SafeCast(const Object* pObj)
	{
		const T* ret = NULL;
		if (NULL != pObj && pObj->IsA(T::StaticGetClass()))
		{
			ret = reinterpret_cast<const T*>(pObj);
		}
		return ret;
	}

	inline bool Object::IsA(const RTTIClass* pParent) const
	{
		return this->GetClass()->IsA(pParent);
	}
}

#endif
