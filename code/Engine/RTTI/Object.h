#ifndef  _Object__H__
#define  _Object__H__


class TiXmlElement;

namespace ma
{
	struct AttributeInfo;
	typedef vector< RefPtr<AttributeInfo> > VEC_ATTR;

	class Object : public Referenced
	{
		
	public:
		Object();

		virtual ~Object();

		#define DECL_OBJECT(ClassName) \
		virtual const char*	GetClassName() const {return #ClassName;}\
		static const char* StaticGetClassName() {return #ClassName;}
	
		DECL_OBJECT(Object);

		virtual void			Improt(TiXmlElement* pXmlObject);
		virtual void			Export(TiXmlElement* pXmlObject);		

		bool					SetAttribute(UINT index, const Any& value);
		bool					SetAttribute(const char* name, const Any& value);

		Any						GetAttribute(unsigned index) const;
		Any						GetAttribute(const char* name) const;
		UINT					GetNumAttributes() const;
		const AttributeInfo*	GetAttributeInfoByName(const char* strName) const;

	protected:
		virtual void			OnSetAttribute(const AttributeInfo& attr, const Any& src);
		virtual void			OnGetAttribute(const AttributeInfo& attr, Any& dest) const;
		const VEC_ATTR*			GetAttributes() const;

	};

}

#endif
