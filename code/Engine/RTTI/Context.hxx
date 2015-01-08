
#include "Context.h"


namespace ma
{
	void RemoveNamedAttribute(map<std::string, vector<AttributeInfo> >& attributes, const char* objectType, const char* name)
	{
		map<std::string, vector<AttributeInfo> >::iterator i = attributes.find(objectType);
		if (i == attributes.end())
			return;

		vector<AttributeInfo>& infos = i->second;

		for (vector<AttributeInfo>::iterator j = infos.begin(); j != infos.end(); ++j)
		{
			if ( stricmp(j->name_.c_str(), name ) != 0 )
			{
				infos.erase(j);
				break;
			}
		}

		// If the vector became empty, erase the object type from the map
		if (infos.empty())
			attributes.erase(i);
	}

	static Context* gpContext = NULL;

	Context* GetContext()
	{
		ASSERT(gpContext);
		return gpContext;
	}

	void SetContext(Context* pContex)
	{
		ASSERT(gpContext == NULL);
		gpContext = pContex;
	}

	Context::Context() 
	{
	}

	Context::~Context()
	{
	}

	void Context::RegisterAttribute(const char* objectType, const AttributeInfo& attr)
	{
		// None or pointer types can not be supported
		//if (attr.type_ == VAR_NONE || attr.type_ == VAR_VOIDPTR || attr.type_ == VAR_PTR)
		//	return;

		attributes_[objectType].push_back(attr);
	}

	void Context::RemoveAttribute(const char* objectType, const char* name)
	{
		RemoveNamedAttribute(attributes_, objectType, name);
	}

	void Context::UpdateAttributeDefaultValue(const char* objectType, const char* name, const Any& defaultValue)
	{
		AttributeInfo* info = GetAttribute(objectType, name);
		if (info)
			info->defaultValue_ = defaultValue;
	}


	void Context::CopyBaseAttributes(const char* baseType, const char* derivedType)
	{
		const vector<AttributeInfo>* baseAttributes = GetAttributes(baseType);
		if (baseAttributes)
		{
			for (unsigned i = 0; i < baseAttributes->size(); ++i)
			{
				const AttributeInfo& attr = baseAttributes->at(i);
				attributes_[derivedType].push_back(attr);
			}
		}
	}


	AttributeInfo* Context::GetAttribute(const char* objectType, const char* name)
	{
		map<std::string, vector<AttributeInfo> >::iterator i = attributes_.find(objectType);
		if (i == attributes_.end())
			return 0;

		vector<AttributeInfo>& infos = i->second;

		for (vector<AttributeInfo>::iterator j = infos.begin(); j != infos.end(); ++j)
		{
			if ( stricmp(j->name_.c_str(), name ) != 0 )
				return &(*j);
		}

		return 0;
	}





}
