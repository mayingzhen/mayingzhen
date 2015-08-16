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
		else if (strVarType == "UINT")
		{
			return Any( (uint32)StringConverter::parseInt(strVarValue) );
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
	}

	void Object::Improt(TiXmlElement* pXmlObject)
	{
		const char* pszClassName = pXmlObject->Attribute("ClassName");
		ASSERT(pszClassName && strcmp(pszClassName,this->GetClassName() ) == 0 );

		TiXmlElement* pXmlAttribute = pXmlObject->FirstChildElement("Attribute");
		while (pXmlAttribute)
		{
			const char* pszName = pXmlAttribute->Attribute("Name");
			const char* pszType = pXmlAttribute->Attribute("Type");
			const char* pszValue = pXmlAttribute->Attribute("Value");

			const AttributeInfo* pAttribute = GetAttributeInfoByName(pszName);
			ASSERT(pAttribute);
			if (pAttribute == NULL)
				continue;

			string strType = pszType;
			string strValue = pszValue;

			Any anyValue = AnyFromString(strType,strValue);

			OnSetAttribute(*pAttribute,anyValue);				

			pXmlAttribute = pXmlAttribute->NextSiblingElement("Attribute");
		}
	}

	void Object::Export(TiXmlElement* pXmlElem)
	{
		pXmlElem->SetAttribute("ClassName",this->GetClassName());

		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes || attributes->empty())
			return;

		for (UINT i = 0; i < attributes->size(); ++i)
		{
			TiXmlElement* pXmlAttribute = new TiXmlElement("Attribute");
			pXmlElem->LinkEndChild(pXmlAttribute);

			AttributeInfo& attribute = (AttributeInfo&)attributes->at(i);

			Any anyValue;

			OnGetAttribute(attribute,anyValue);	

			string strName = attribute.name_;
			string strType;
			string strValue;

			AnyGetString(anyValue,strType,strValue);

			pXmlAttribute->SetAttribute("Name",strName.c_str());
			pXmlAttribute->SetAttribute("Type",strType.c_str());
			pXmlAttribute->SetAttribute("Value",strValue.c_str());
		}
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

	const vector<AttributeInfo>* Object::GetAttributes() const
	{
		return GetAttributeManager()->GetAttributes( this->GetClassName() );
	}

	const AttributeInfo* Object::GetAttributeInfoByName(const char* strName) const
	{
		const vector<AttributeInfo>* attributes = GetAttributes();
		for (UINT i = 0; i < attributes->size(); ++i)
		{
			if (attributes->at(i).name_ == strName)
				return &attributes->at(i);
		}

		return NULL;
	}

	bool Object::SetAttribute(unsigned index, const Any& value)
	{
		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes)
		{
			return false;
		}
		if (index >= attributes->size())
		{
			return false;
		}

		const AttributeInfo& attr = attributes->at(index);

		OnSetAttribute(attr, value);

		return true;
	}

	bool Object::SetAttribute(const char* name, const Any& value)
	{
		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes)
		{
			return false;
		}

		for (vector<AttributeInfo>::const_iterator i = attributes->begin(); i != attributes->end(); ++i)
		{
			if ( strcmp(i->name_.c_str(), name )  == 0 )
			{
				OnSetAttribute(*i, value);
				return true;
			}
		}

		return false;
	}

	Any Object::GetAttribute(unsigned index) const
	{
		Any ret;

		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes)
		{
			return ret;
		}
		if (index >= attributes->size())
		{
			ASSERT("Attribute index out of bounds");
			return ret;
		}

		OnGetAttribute(attributes->at(index), ret);
		return ret;
	}

	Any Object::GetAttribute(const char* name) const
	{
		Any ret;

		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes)
		{
			return ret;
		}

		for (vector<AttributeInfo>::const_iterator i = attributes->begin(); i != attributes->end(); ++i)
		{
			if ( strcmp( i->name_.c_str() , name ) == 0 )
			{
				OnGetAttribute(*i, ret);
				return ret;
			}
		}

		return ret;
	}

	UINT Object::GetNumAttributes() const
	{
		const vector<AttributeInfo>* attributes = GetAttributes();
		return attributes ? attributes->size() : 0;
	}

}

