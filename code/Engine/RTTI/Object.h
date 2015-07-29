#ifndef  _Object__H__
#define  _Object__H__



#define DECL_OBJECT(ClassName) \
	virtual const char*	GetClassName() const {return #ClassName;}\
	static const char* StaticGetClassName() {return #ClassName;}

#define IMPL_OBJECT(ClassName,ParentName)


namespace ma
{
	class RTTIClass;

	class Object : public Referenced
	{
		
	public:
		Object();

		Object(const char* pName);

		DECL_OBJECT(Object)
		
		virtual ~Object();
	
		const char*		GetName() const;

		void			SetName(const char* pName);

	protected:
		std::string		m_sName;
	};

}

#endif
