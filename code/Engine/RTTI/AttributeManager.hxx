
#include "AttributeManager.h"


namespace ma
{
	void RemoveNamedAttribute(map<std::string, VEC_ATTR >& attributes, const char* objectType, const char* name)
	{
		map<std::string, VEC_ATTR >::iterator i = attributes.find(objectType);
		if (i == attributes.end())
			return;

		VEC_ATTR& infos = i->second;

		for (VEC_ATTR::iterator j = infos.begin(); j != infos.end(); ++j)
		{
			if ( strcmp((*j)->GetName(), name ) != 0 )
			{
				infos.erase(j);
				break;
			}
		}

		// If the vector became empty, erase the object type from the map
		if (infos.empty())
			attributes.erase(i);
	}

	AttributeManager::AttributeManager() 
	{
	}

	AttributeManager::~AttributeManager()
	{
	}

	void AttributeManager::RegisterAttribute(const char* objectType, RefPtr<AttributeInfo> attr)
	{
		// None or pointer types can not be supported
		//if (attr.type_ == VAR_NONE || attr.type_ == VAR_VOIDPTR || attr.type_ == VAR_PTR)
		//	return;

		attributes_[objectType].push_back(attr);
	}

	void AttributeManager::RemoveAttribute(const char* objectType, const char* name)
	{
		RemoveNamedAttribute(attributes_, objectType, name);
	}

	void AttributeManager::UpdateAttributeDefaultValue(const char* objectType, const char* name, const Any& defaultValue)
	{
		AttributeInfo* info = GetAttribute(objectType, name);
		if (info)
			info->defaultValue_ = defaultValue;
	}


	void AttributeManager::CopyBaseAttributes(const char* baseType, const char* derivedType)
	{
		const VEC_ATTR* baseAttributes = GetAttributes(baseType);
		if (baseAttributes)
		{
			for (unsigned i = 0; i < baseAttributes->size(); ++i)
			{
				RefPtr<AttributeInfo> attr = baseAttributes->at(i);
				attributes_[derivedType].push_back(attr);
			}
		}
	}


	AttributeInfo* AttributeManager::GetAttribute(const char* objectType, const char* name)
	{
		map<std::string, VEC_ATTR >::iterator i = attributes_.find(objectType);
		if (i == attributes_.end())
			return 0;

		VEC_ATTR& infos = i->second;

		for (VEC_ATTR::iterator j = infos.begin(); j != infos.end(); ++j)
		{
			RefPtr<AttributeInfo> pAtt = *j;
			if ( strcmp(pAtt->GetName(), name ) != 0 )
				return pAtt.get();
		}

		return 0;
	}


	AttributeManager* g_pAttributeManager = NULL;

	AttributeManager* GetAttributeManager()
	{
		return g_pAttributeManager;
	}

}
