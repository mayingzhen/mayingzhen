#ifndef  _Object__H__
#define  _Object__H__

#include "Common/RefCountObject.h"
#include "Common/RTTI/Class.h"


#define DECL_OBJECT(ClassName) private:\
	static Class* ms_p##ClassName##Class;\
	public:\
	static void StaticInitClass();\
	static void StaticShutdownClass();\
	static const Class*		StaticGetClass();\
	virtual Class*	GetClass() const;\
	private:

#define IMPL_OBJECT(ClassName,ParentName) Class* ClassName::ms_p##ClassName##Class = NULL; \
	void ClassName::StaticInitClass(){\
	/*std::string*/const char* className = #ClassName;\
	ms_p##ClassName##Class = new Class(className,ParentName::StaticGetClass());\
	ClassManager::GetInstance().RegisterClass(ms_p##ClassName##Class); \
	}\
	void ClassName::StaticShutdownClass(){ \
	ClassManager::GetInstance().UnRegisterClass(ms_p##ClassName##Class); \
	SAFE_DELETE(ms_p##ClassName##Class);\
	}\
	const Class*		ClassName::StaticGetClass(){\
	return ms_p##ClassName##Class;\
	}\
	Class*	ClassName::GetClass() const{\
	return ms_p##ClassName##Class;\
	}


namespace ma
{
	class Class;

	class COMMON_API Object : public RefCountObject
	{
		DECL_OBJECT(Object)

	public:
		Object(const char* pName = NULL);
		
		virtual ~Object();
	
		const char*		GetName() {return m_sName.c_str();}

		void			SetName(const char* pName) {m_sName = pName;}

		bool			IsA(const Class* pParent) const;

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

	inline bool Object::IsA(const Class* pParent) const
	{
		return this->GetClass()->IsA(pParent);
	}
}

#endif
