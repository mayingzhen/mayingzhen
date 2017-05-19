#include "Object.h"

namespace ma
{
	Object::Object()
	{

	}

	Object::~Object()
	{

	}

	Any AnyFromString(const string& strVarType,const string& strVarValue)
	{
		if (strVarType == "string")
		{
			return Any(strVarValue.c_str());
		}
		else if (strVarType == "bool")
		{
			return Any( StringConverter::parseBool(strVarValue) );
		}
		else if (strVarType == "int")
		{
			return Any( StringConverter::parseInt(strVarValue) );
		}
		else if (strVarType == "short")
		{
			return Any((short)StringConverter::parseInt(strVarValue));
		}
		else if (strVarType == "BYTE")
		{
			return Any((unsigned char)StringConverter::parseInt(strVarValue));
		}
		else if (strVarType == "UINT")
		{
			return Any( StringConverter::parseUnsignedInt(strVarValue) );
		}
		else if (strVarType == "float")
		{
			return Any( StringConverter::parseReal(strVarValue) );
		}
		else if (strVarType == "Vector2")
		{
			return Any( StringConverter::parseVector2(strVarValue) );
		}
		else if (strVarType == "Vector3")
		{
			return Any( StringConverter::parseVector3(strVarValue) );		
		}
		else if (strVarType == "Vector4")
		{
			return Any( StringConverter::parseVector4(strVarValue) );	
		}
		else if (strVarType == "Quaternion")
		{
			return Any( StringConverter::parseQuaternion(strVarValue) );
		}
		else if (strVarType == "Vec3i")
		{
			return Any( StringConverter::parseVec3i(strVarValue) );
		}

		return Any();
	}

	void AnyGetString(const Any& varValue, string& strVarType,string& strVarValue)
	{
		const std::type_info& type = varValue.getType();
		if (type == typeid(const char*))
		{
			strVarType = "string";
			strVarValue = any_cast<const char*>(varValue) ? any_cast<const char*>(varValue) : "";
		}
		else if (type == typeid(bool))
		{
			strVarType = "bool";
			strVarValue = StringConverter::toString( any_cast<bool>(varValue) );
		}
		else if (type == typeid(int))
		{
			strVarType = "int";
			strVarValue = StringConverter::toString( any_cast<int>(varValue) ) ;
		}
		else if (type == typeid(short))
		{
			strVarType = "short";
			strVarValue = StringConverter::toString(any_cast<short>(varValue));
		}
		else if (type == typeid(unsigned char))
		{
			strVarType = "BYTE";
			strVarValue = StringConverter::toString(any_cast<unsigned char>(varValue));
		}
		else if (type == typeid(uint32))
		{
			strVarType = "UINT";
			strVarValue = StringConverter::toString( any_cast<uint32>(varValue) ) ;
		}
		else if (type == typeid(float))
		{
			strVarType = "float";
			strVarValue = StringConverter::toString( any_cast<float>(varValue) ) ;
		}
		else if (type == typeid(Vector2))
		{
			strVarType = "Vector2";
			strVarValue = StringConverter::toString( any_cast<Vector2>(varValue) ) ;
		}
		else if (type == typeid(Vector3))
		{
			strVarType = "Vector3";
			strVarValue = StringConverter::toString( any_cast<Vector3>(varValue) ) ;
		}
		else if (type == typeid(Vector4))
		{
			strVarType = "Vector4";
			strVarValue = StringConverter::toString( any_cast<Vector4>(varValue) ) ;
		}
		else if (type == typeid(Quaternion))
		{
			strVarType = "Quaternion";
			strVarValue = StringConverter::toString( any_cast<Quaternion>(varValue) );
		}
		else if (type == typeid(Vec3i))
		{
			strVarType = "Vec3i";
			strVarValue = StringConverter::toString( any_cast<Vec3i>(varValue) );
		}
	}

	bool Object::Import(rapidxml::xml_node<>* pXmlObject)
	{
		const char* pszClassName = pXmlObject->findAttribute("ClassName");
		ASSERT(pszClassName && strcmp(pszClassName,this->GetClassName() ) == 0 );

		rapidxml::xml_node<>* pXmlAttribute = pXmlObject->first_node("Attribute");
		while (pXmlAttribute)
		{
			const char* pszName = pXmlAttribute->findAttribute("Name");
			const char* pszType = pXmlAttribute->findAttribute("Type");
			const char* pszValue = pXmlAttribute->findAttribute("Value");

			const AttributeInfo* pAttribute = GetAttributeInfoByName(pszName);
			if (pAttribute == NULL)
			{
				pXmlAttribute = pXmlAttribute->next_sibling("Attribute");
				continue;
			}

			string strType = pszType;
			string strValue = pszValue;

			Any anyValue = AnyFromString(strType,strValue);

			OnSetAttribute(*pAttribute,anyValue);				

			pXmlAttribute = pXmlAttribute->next_sibling("Attribute");
		}

		return true;
	}

	bool Object::Export(rapidxml::xml_node<>* pXmlObject,rapidxml::xml_document<>& doc)
	{
		pXmlObject->append_attribute(doc.allocate_attribute(doc.allocate_string("ClassName"),doc.allocate_string(this->GetClassName())));

		const VEC_ATTR* attributes = GetAttributes();
		if (!attributes || attributes->empty())
			return false;

		for (UINT i = 0; i < attributes->size(); ++i)
		{
			rapidxml::xml_node<>* pXmlAttribute = doc.allocate_node(rapidxml::node_element,doc.allocate_string("Attribute"));
			pXmlObject->append_node(pXmlAttribute);

			RefPtr<AttributeInfo> attribute = attributes->at(i);

			Any anyValue;

			OnGetAttribute(*attribute,anyValue);	

			string strName = attribute->GetName();
			string strType;
			string strValue;

			AnyGetString(anyValue,strType,strValue);

			pXmlAttribute->append_attribute(doc.allocate_attribute(doc.allocate_string("Name"),doc.allocate_string(strName.c_str())));
			pXmlAttribute->append_attribute(doc.allocate_attribute(doc.allocate_string("Type"),doc.allocate_string(strType.c_str())));
			pXmlAttribute->append_attribute(doc.allocate_attribute(doc.allocate_string("Value"),doc.allocate_string(strValue.c_str())));
		}

		return true;
	}

	void Object::OnSetAttribute(const AttributeInfo& attr, const Any& src)
	{
		if (attr.accessor_)
		{
			attr.accessor_->Set(this, src);
			return;
		}
	}

	void Object::OnGetAttribute(const AttributeInfo& attr, Any& dest) const
	{
		if (attr.accessor_)
		{
			attr.accessor_->Get(this, dest);
			return;
		}
	}

	const VEC_ATTR* Object::GetAttributes() const
	{
		return GetAttributeManager()->GetAttributes( this->GetClassName() );
	}

	const AttributeInfo* Object::GetAttributeInfoByName(const char* strName) const
	{
		const VEC_ATTR* attributes = GetAttributes();
		for (UINT i = 0; i < attributes->size(); ++i)
		{
			RefPtr<AttributeInfo> pAtt = attributes->at(i);
			if (string(pAtt->GetName()) == strName)
				return pAtt.get();
		}

		return NULL;
	}

	bool Object::SetAttribute(unsigned index, const Any& value)
	{
		const VEC_ATTR* attributes = GetAttributes();
		if (!attributes)
		{
			return false;
		}
		if (index >= attributes->size())
		{
			return false;
		}

		const AttributeInfo& attr = *(attributes->at(index).get());

		OnSetAttribute(attr, value);

		return true;
	}

	bool Object::SetAttribute(const char* name, const Any& value)
	{
		const VEC_ATTR* attributes = GetAttributes();
		if (!attributes)
		{
			return false;
		}

		for (VEC_ATTR::const_iterator i = attributes->begin(); i != attributes->end(); ++i)
		{
			RefPtr<AttributeInfo> pArrt = *i;
			if ( strcmp(pArrt->GetName(), name )  == 0 )
			{
				OnSetAttribute(*pArrt, value);
				return true;
			}
		}

		return false;
	}

	Any Object::GetAttribute(unsigned index) const
	{
		Any ret;

		const VEC_ATTR* attributes = GetAttributes();
		if (!attributes)
		{
			return ret;
		}
		if (index >= attributes->size())
		{
			ASSERT("Attribute index out of bounds");
			return ret;
		}

		OnGetAttribute(*attributes->at(index), ret);
		return ret;
	}

	Any Object::GetAttribute(const char* name) const
	{
		Any ret;

		const VEC_ATTR* attributes = GetAttributes();
		if (!attributes)
		{
			return ret;
		}

		for (VEC_ATTR::const_iterator i = attributes->begin(); i != attributes->end(); ++i)
		{
			RefPtr<AttributeInfo> pAtt = *i;
 			if ( strcmp( pAtt->GetName(), name ) == 0 )
			{
				OnGetAttribute(*pAtt, ret);
				return ret;
			}
		}

		return ret;
	}

	UINT Object::GetNumAttributes() const
	{
		const VEC_ATTR* attributes = GetAttributes();
		return attributes ? attributes->size() : 0;
	}

}

