#include "Attribute.h"

namespace ma
{
	/// Construct empty.
	AttributeInfo::AttributeInfo() :
	enumNames_(0),
	mode_(AM_DEFAULT),
	ptr_(0)
	{

	}

	/// Construct accessor attribute.
	AttributeInfo::AttributeInfo(const char* name, AttributeAccessor* accessor, const Any& defaultValue, unsigned mode) :
	name_(name),
	enumNames_(0),
	accessor_(accessor),
	defaultValue_(defaultValue),
	mode_(mode),
	ptr_(0)
	{
	}

	/// Construct accessor enum attribute.
	AttributeInfo::AttributeInfo(const char* name, AttributeAccessor* accessor, const char** enumNames, const Any& defaultValue, unsigned mode) :
	name_(name),
	enumNames_(enumNames),
	accessor_(accessor),
	defaultValue_(defaultValue),
	mode_(mode),
	ptr_(0)
	{
	}

	const char* AttributeInfo::GetName() const
	{
		return name_.c_str();
	}

	void AttributeInfo::SetName(const char* pszName)
	{
		name_ = pszName ? pszName : "";
	}

	RefPtr<AttributeInfo> CreateAttributeInfo()
	{
		return new AttributeInfo();
	}

	RefPtr<AttributeInfo> CreateAttributeInfo(const char* name, AttributeAccessor* accessor, const Any& defaultValue, unsigned mode)
	{
		return new AttributeInfo(name,accessor,defaultValue,mode);
	}

	RefPtr<AttributeInfo> CreateAttributeInfo(const char* name, AttributeAccessor* accessor, const char** enumNames, const Any& defaultValue, unsigned mode)
	{
		return new AttributeInfo(name,accessor,enumNames,defaultValue,mode);
	}
}

