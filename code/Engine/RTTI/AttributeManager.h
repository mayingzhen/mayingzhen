
#ifndef __AttributeManager_H__
#define __AttributeManager_H__

#include "Attribute.h"

namespace ma
{
	typedef vector< RefPtr<AttributeInfo> > VEC_ATTR;

	class AttributeManager 
	{
		friend class Object;
	
	public:
		/// Construct.
		AttributeManager();
		/// Destruct.
		~AttributeManager();

		/// Register object attribute.
		void RegisterAttribute(const char* objectType, RefPtr<AttributeInfo> attr);
		/// Remove object attribute.
		void RemoveAttribute(const char* objectType, const char* name);
		/// Update object attribute's default value.
		void UpdateAttributeDefaultValue(const char* objectType, const char* name, const Any& defaultValue);
	    
		/// Copy base class attributes to derived class.
		void CopyBaseAttributes(const char* baseType, const char* derivedType);

		/// Template version of registering an object attribute.
		template <class T> void RegisterAttribute( RefPtr<AttributeInfo> attr);
		/// Template version of removing an object attribute.
		template <class T> void RemoveAttribute(const char* name);
		/// Template version of copying base class attributes to derived class.
		template <class T, class U> void CopyBaseAttributes();
		/// Template version of updating an object attribute's default value.
		template <class T> void UpdateAttributeDefaultValue(const char* name, const Any& defaultValue);

		/// Return a specific attribute description for an object, or null if not found.
		AttributeInfo* GetAttribute(const char* objectType, const char* name);

		/// Template version of returning a specific attribute description.
		template <class T> AttributeInfo* GetAttribute(const char* name);

		/// Return attribute descriptions for an object type, or null if none defined.
		const VEC_ATTR* GetAttributes(const char* type) const
		{
			std::map<std::string, VEC_ATTR >::const_iterator i = attributes_.find(type);
			return i != attributes_.end() ? &i->second : 0;
		}


	private:

		/// Attribute descriptions per object type.
		map<std::string, VEC_ATTR> attributes_;
	};
	
	extern AttributeManager* g_pAttributeManager;
	AttributeManager* GetAttributeManager();

	template <class T> void AttributeManager::RegisterAttribute(RefPtr<AttributeInfo> attr) { RegisterAttribute(T::StaticGetTypeName(), attr); }
	template <class T> void AttributeManager::RemoveAttribute(const char* name) { RemoveAttribute(T::StaticGetTypeName(), name); }
	template <class T, class U> void AttributeManager::CopyBaseAttributes() { CopyBaseAttributes(T::StaticGetTypeName(), U::StaticGetTypeName()); }
	template <class T> AttributeInfo* AttributeManager::GetAttribute(const char* name) { return GetAttribute(T::StaticGetTypeName(), name); }
	template <class T> void AttributeManager::UpdateAttributeDefaultValue(const char* name, const Any& defaultValue) { UpdateAttributeDefaultValue(T::StaticGetTypeName(), name, defaultValue); }

	#define COPY_BASE_ATTRIBUTES(className, sourceClassName) GetAttributeManager()->CopyBaseAttributes<sourceClassName, className>()
	#define REMOVE_ATTRIBUTE(className, name) GetAttributeManager()->RemoveAttribute<className>(name)
	#define ATTRIBUTE(className, name, variable, typeName, defaultValue, mode) GetAttributeManager()->RegisterAttribute<className>(CreateAttributeInfo(name, new AttributeImpl<className, typeName>( offsetof(className, variable) ) , Any(defaultValue), mode))
	#define ENUM_ATTRIBUTE(className, name, variable, typeName, enumNames, defaultValue, mode) GetAttributeManager()->RegisterAttribute<className>(CreateAttributeInfo(name, new EnumAttributeImpl<className, typeName>(enumNames, offsetof(className, variable)), Any(defaultValue), mode))
	#define ACCESSOR_ATTRIBUTE(className, name, getFunction, setFunction, typeName, defaultValue, mode) GetAttributeManager()->RegisterAttribute<className>(CreateAttributeInfo(name, new AttributeAccessorImpl<className, typeName>(&className::getFunction, &className::setFunction), Any(defaultValue), mode))
	#define ENUM_ACCESSOR_ATTRIBUTE(className, name, getFunction, setFunction, typeName, enumNames, defaultValue, mode) GetAttributeManager()->RegisterAttribute<className>(CreateAttributeInfo(name, new EnumAttributeAccessorImpl<className, typeName>(enumNames,&className::getFunction, &className::setFunction), Any(defaultValue), mode))
	#define REF_ACCESSOR_ATTRIBUTE(className, name, getFunction, setFunction, typeName, defaultValue, mode) GetAttributeManager()->RegisterAttribute<className>(CreateAttributeInfo(name, new RefAttributeAccessorImpl<className, typeName>(&className::getFunction, &className::setFunction), Any(defaultValue), mode))
	#define UPDATE_ATTRIBUTE_DEFAULT_VALUE(className, name, defaultValue) GetAttributeManager()->UpdateAttributeDefaultValue<className>(name, defaultValue)

}


#endif
