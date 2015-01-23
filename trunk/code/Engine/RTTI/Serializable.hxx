
#include "AttributeManager.h"
#include "Serializable.h"

namespace ma
{

	Serializable::Serializable() :
		instanceDefaultValues_(0),
		temporary_(false)
	{
		m_eCreateType = XML_RES;
	}

	Serializable::~Serializable()
	{
		delete instanceDefaultValues_;
		instanceDefaultValues_ = 0;
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
		else if (strVarType == "TexturePath")
		{
			return Any( CreateTexture(strVarValue.c_str()) );
		}
// 		else if (strVarType == "SamplerState")
// 		{
// 			return Any( SamplerState )
// 		}
		else
		{
			ASSERT(false);
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
		else if (type == typeid(RefPtr<Texture>))
		{
			strVarType = "TexturePath";
			RefPtr<Texture> pTexture = any_cast< RefPtr<Texture> >(varValue);
			strVarValue = pTexture->GetResPath();
		}
// 		else if (type = typeid(RefPtr<SamplerState>))
// 		{
// 			strVarType = "SamplerState";
// 			RefPtr<SamplerState> pSamplerState = any_cast< RefPtr<SamplerState> >(varValue);
// 			//pSamplerState->Serialize(sl);
// 		}
		else 
		{
			ASSERT(false);
		}
	}

	void Serializable::Serialize(Serializer& sl, const char* pszLable)
	{
		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes || attributes->empty())
			return;

		sl.BeginSection("AttributeInfo");

		UINT nSize = (UINT)attributes->size();
		sl.Serialize(nSize,"size");
		ASSERT(nSize == attributes->size());
		
		for (UINT i = 0; i < attributes->size(); ++i)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",i);
			sl.BeginSection(buf);
				
			if (sl.IsReading())
			{
				string strAttrName;
				string strVarType;
				string strVarValue;
				Any varValue;

				sl.Serialize(strAttrName,"VariantName");
				sl.Serialize(strVarType,"VariantType");
				sl.Serialize(strVarValue,"VariantValue");
			
				varValue = AnyFromString(strVarType,strVarValue);
					
				const AttributeInfo* attribute = GetAttributeInfoByName(strAttrName.c_str());
				ASSERT(attribute);
				if (attribute == NULL)
					continue;

				OnSetAttribute(*attribute,varValue);
			}
			else
			{
				string strAttrName;
				string strVarType;
				string strVarValue;
				Any varValue;

				AttributeInfo& attribute = (AttributeInfo&)attributes->at(i);

				strAttrName = attribute.name_;
				
				OnGetAttribute(attribute,varValue);	

				AnyGetString(varValue,strVarType,strVarValue);
				
				sl.Serialize(strAttrName,"VariantName");
				sl.Serialize(strVarType,"VariantType");
				sl.Serialize(strVarValue,"VariantValue");
			}

			sl.EndSection();
		}

		sl.EndSection();
	}

	void Serializable::OnSetAttribute(const AttributeInfo& attr, const Any& src)
	{
		// Check for accessor function mode
		if (attr.accessor_)
		{
			attr.accessor_->Set(this, src);
			return;
		}

		// Calculate the destination address
		void* dest = attr.ptr_ ? attr.ptr_ : reinterpret_cast<unsigned char*>(this) + attr.offset_;

// 		switch (attr.type_)
// 		{
// 		case VAR_INT:
// 			// If enum type, use the low 8 bits only
// 			if (attr.enumNames_)
// 				*(reinterpret_cast<unsigned char*>(dest)) = src.GetInt();
// 			else
// 				*(reinterpret_cast<int*>(dest)) = src.GetInt();
// 			break;
// 
// 		case VAR_BOOL:
// 			*(reinterpret_cast<bool*>(dest)) = src.GetBool();
// 			break;
// 
// 		case VAR_FLOAT:
// 			*(reinterpret_cast<float*>(dest)) = src.GetFloat();
// 			break;
// 
// 		case VAR_VECTOR2:
// 			*(reinterpret_cast<Vector2*>(dest)) = src.GetVector2();
// 			break;
// 
// 		case VAR_VECTOR3:
// 			*(reinterpret_cast<Vector3*>(dest)) = src.GetVector3();
// 			break;
// 
// 		case VAR_VECTOR4:
// 			*(reinterpret_cast<Vector4*>(dest)) = src.GetVector4();
// 			break;
// 
// 		case VAR_QUATERNION:
// 			*(reinterpret_cast<Quaternion*>(dest)) = src.GetQuaternion();
// 			break;
// 
// 		case VAR_COLOR:
// 			*(reinterpret_cast<Color*>(dest)) = src.GetColor();
// 			break;
// 
// 		case VAR_STRING:
// 			*(reinterpret_cast<std::string*>(dest)) = src.GetString();
// 			break;
// 
// 		case VAR_BUFFER:
// 			*(reinterpret_cast<PODVector<unsigned char>*>(dest)) = src.GetBuffer();
// 			break;
// 
// 		case VAR_VARIANTVECTOR:
// 			*(reinterpret_cast<VariantVector*>(dest)) = src.GetVariantVector();
// 			break;
// 
// 		case VAR_VARIANTMAP:
// 			*(reinterpret_cast<VariantMap*>(dest)) = src.GetVariantMap();
// 			break;
// 
// 		case VAR_INTRECT:
// 			*(reinterpret_cast<IntRect*>(dest)) = src.GetIntRect();
// 			break;
// 
// 		case VAR_INTVECTOR2:
// 			*(reinterpret_cast<IntVector2*>(dest)) = src.GetIntVector2();
// 			break;
// 
// 		default:
// 			ASSERT("Unsupported attribute type for OnSetAttribute()");
// 			return;
// 		}
	}

	void Serializable::OnGetAttribute(const AttributeInfo& attr, Any& dest) const
	{
		// Check for accessor function mode
		if (attr.accessor_)
		{
			attr.accessor_->Get(this, dest);
			return;
		}

		// Calculate the source address
		const void* src = attr.ptr_ ? attr.ptr_ : reinterpret_cast<const unsigned char*>(this) + attr.offset_;

// 		switch (attr.type_)
// 		{
// 		case VAR_INT:
// 			// If enum type, use the low 8 bits only
// 			if (attr.enumNames_)
// 				dest = *(reinterpret_cast<const unsigned char*>(src));
// 			else
// 				dest = *(reinterpret_cast<const int*>(src));
// 			break;
// 
// 		case VAR_BOOL:
// 			dest = *(reinterpret_cast<const bool*>(src));
// 			break;
// 
// 		case VAR_FLOAT:
// 			dest = *(reinterpret_cast<const float*>(src));
// 			break;
// 
// 		case VAR_VECTOR2:
// 			dest = *(reinterpret_cast<const Vector2*>(src));
// 			break;
// 
// 		case VAR_VECTOR3:
// 			dest = *(reinterpret_cast<const Vector3*>(src));
// 			break;
// 
// 		case VAR_VECTOR4:
// 			dest = *(reinterpret_cast<const Vector4*>(src));
// 			break;
// 
// 		case VAR_QUATERNION:
// 			dest = *(reinterpret_cast<const Quaternion*>(src));
// 			break;
// 
// 		case VAR_COLOR:
// 			dest = *(reinterpret_cast<const Color*>(src));
// 			break;
// 
// 		case VAR_STRING:
// 			dest = *(reinterpret_cast<const std::string*>(src));
// 			break;
// 
// 		case VAR_BUFFER:
// 			dest = *(reinterpret_cast<const PODVector<unsigned char>*>(src));
// 			break;
// 
// 		case VAR_VARIANTVECTOR:
// 			dest = *(reinterpret_cast<const VariantVector*>(src));
// 			break;
// 
// 		case VAR_VARIANTMAP:
// 			dest = *(reinterpret_cast<const VariantMap*>(src));
// 			break;
// 
// 		case VAR_INTRECT:
// 			dest = *(reinterpret_cast<const IntRect*>(src));
// 			break;
// 
// 		case VAR_INTVECTOR2:
// 			dest = *(reinterpret_cast<const IntVector2*>(src));
// 			break;
// 
// 		default:
// 			ASSERT("Unsupported attribute type for OnGetAttribute()");
// 			return;
// 		}
	}

	const vector<AttributeInfo>* Serializable::GetAttributes() const
	{
		return GetAttributeManager()->GetAttributes( this->GetClass()->GetName() );
	}
	
	const AttributeInfo*	Serializable::GetAttributeInfoByName(const char* strName) const
	{
		const vector<AttributeInfo>* attributes = GetAttributes();
		for (UINT i = 0; i < attributes->size(); ++i)
		{
			if (attributes->at(i).name_ == strName)
				return &attributes->at(i);
		}

		return NULL;
	}

	bool Serializable::SetAttribute(unsigned index, const Any& value)
	{
		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes)
		{
			//ASSERT(GetTypeName() + " has no attributes");
			return false;
		}
		if (index >= attributes->size())
		{
			//ASSERT("Attribute index out of bounds");
			return false;
		}

		const AttributeInfo& attr = attributes->at(index);

		// Check that the new value's type matches the attribute type
		//if (value.GetType() == attr.type_)
		{
			OnSetAttribute(attr, value);
			return true;
		}
		//else
		{
			//ASSERT("Could not set attribute " + attr.name_ + ": expected type " + Variant::GetTypeName(attr.type_) +
			//	" but got " + value.GetTypeName());
		//	return false;
		}
	}

	bool Serializable::SetAttribute(const char* name, const Any& value)
	{
		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes)
		{
			//ASSERT(GetTypeName() + " has no attributes");
			return false;
		}

		for (vector<AttributeInfo>::const_iterator i = attributes->begin(); i != attributes->end(); ++i)
		{
			if ( stricmp(i->name_.c_str(), name )  == 0 )
			{
				// Check that the new value's type matches the attribute type
				//if (value.GetType() == i->type_)
				{
					OnSetAttribute(*i, value);
					return true;
				}
				//else
				{
					//ASSERT("Could not set attribute " + i->name_ + ": expected type " + Variant::GetTypeName(i->type_)
					//	+ " but got " + value.GetTypeName());
				//	return false;
				}
			}
		}

		//ASSERT("Could not find attribute " + name + " in " + GetTypeName());
		return false;
	}

	void Serializable::ResetToDefault()
	{
		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes)
			return;

		for (unsigned i = 0; i < attributes->size(); ++i)
		{
			const AttributeInfo& attr = attributes->at(i);
			if (attr.mode_ & (AM_NOEDIT | AM_NODEID | AM_COMPONENTID | AM_NODEIDVECTOR))
				continue;

			Any defaultValue = GetInstanceDefault(attr.name_.c_str());
			if (defaultValue.isEmpty())
				defaultValue = attr.defaultValue_;

			OnSetAttribute(attr, defaultValue);
		}
	}

	void Serializable::RemoveInstanceDefault()
	{
		delete instanceDefaultValues_;
		instanceDefaultValues_ = 0;
	}

	void Serializable::SetTemporary(bool enable)
	{
		if (enable != temporary_)
		{
			temporary_ = enable;
	        
			//using namespace TemporaryChanged;
	        
			//VariantMap& eventData = GetEventDataMap();
			//eventData[P_SERIALIZABLE] = this;
	        
			//SendEvent(E_TEMPORARYCHANGED, eventData);
		}
	}


	Any Serializable::GetAttribute(unsigned index) const
	{
		Any ret;

		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes)
		{
			//ASSERT(GetTypeName() + " has no attributes");
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

	Any Serializable::GetAttribute(const char* name) const
	{
		Any ret;

		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes)
		{
			//ASSERT(GetTypeName() + " has no attributes");
			return ret;
		}

		for (vector<AttributeInfo>::const_iterator i = attributes->begin(); i != attributes->end(); ++i)
		{
			if ( stricmp( i->name_.c_str() , name ) == 0 )
			{
				OnGetAttribute(*i, ret);
				return ret;
			}
		}

		//ASSERT("Could not find attribute " + name + " in " + GetTypeName());
		return ret;
	}

	Any Serializable::GetAttributeDefault(unsigned index) const
	{
		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes)
		{
			//ASSERT(GetTypeName() + " has no attributes");
			return Any();
		}
		if (index >= attributes->size())
		{
			//ASSERT("Attribute index out of bounds");
			return Any();
		}

		AttributeInfo attr = attributes->at(index);
		Any defaultValue = GetInstanceDefault(attr.name_.c_str());
		return defaultValue.isEmpty() ? attr.defaultValue_ : defaultValue;
	}

	Any Serializable::GetAttributeDefault(const char* name) const
	{
		Any defaultValue = GetInstanceDefault(name);
		if (!defaultValue.isEmpty())
			return defaultValue;

		const vector<AttributeInfo>* attributes = GetAttributes();
		if (!attributes)
		{
			//ASSERT(GetTypeName() + " has no attributes");
			return Any();
		}

		for (vector<AttributeInfo>::const_iterator i = attributes->begin(); i != attributes->end(); ++i)
		{
			if ( stricmp( i->name_.c_str(), name ) == 0 )
				return i->defaultValue_;
		}

		//ASSERT("Could not find attribute " + name + " in " + GetTypeName());
		return Any();
	}

	unsigned Serializable::GetNumAttributes() const
	{
		const vector<AttributeInfo>* attributes = GetAttributes();
		return attributes ? attributes->size() : 0;
	}

// 	unsigned Serializable::GetNumNetworkAttributes() const
// 	{
// 		const vector<AttributeInfo>* attributes = networkState_ ? networkState_->attributes_ :
// 			context_->GetNetworkAttributes(GetType());
// 		return attributes ? attributes->Size() : 0;
// 	}

	void Serializable::SetInstanceDefault(const char* name, const Any& defaultValue)
	{
		// Allocate the instance level default value
		if (!instanceDefaultValues_)
			instanceDefaultValues_ = new VariantMap();
		instanceDefaultValues_->operator[] (name) = defaultValue;
	}

	Any Serializable::GetInstanceDefault(const char* name) const
	{
		if (instanceDefaultValues_)
		{
			VariantMap::const_iterator i = instanceDefaultValues_->find(name);
			if (i != instanceDefaultValues_->end())
				return i->second;
		}

		return Any();
	}

}
