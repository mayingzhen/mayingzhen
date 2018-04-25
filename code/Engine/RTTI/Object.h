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
		virtual const char*	GetTypeName() const {return #ClassName;}\
		static const char* StaticGetTypeName() {return #ClassName;}
	
		DECL_OBJECT(Object);

		virtual bool			Import(rapidxml::xml_node<>* pXmlObject);
		virtual bool			Export(rapidxml::xml_node<>* pXmlObject,rapidxml::xml_document<>& doc);		

		bool					SetAttribute(uint32_t index, const Any& value);
		bool					SetAttribute(const char* name, const Any& value);

		Any						GetAttribute(unsigned index) const;
		Any						GetAttribute(const char* name) const;
		uint32_t				GetNumAttributes() const;
		const AttributeInfo*	GetAttributeInfoByName(const char* strName) const;

	protected:
		virtual void			OnSetAttribute(const AttributeInfo& attr, const Any& src);
		virtual void			OnGetAttribute(const AttributeInfo& attr, Any& dest) const;
		const VEC_ATTR*			GetAttributes() const;

	};

}

#endif
