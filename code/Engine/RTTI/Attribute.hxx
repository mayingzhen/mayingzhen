#include "Attribute.h"

namespace ma
{
	/// Construct empty.
	AttributeInfo::AttributeInfo() :
	mode_(AM_DEFAULT)
	{

	}

	/// Construct accessor attribute.
	AttributeInfo::AttributeInfo(const char* name, AttributeAccessor* accessor, const Any& defaultValue, unsigned mode) :
	name_(name),
	accessor_(accessor),
	defaultValue_(defaultValue),
	mode_(mode)
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

}

