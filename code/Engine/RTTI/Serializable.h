
#ifndef __Serializable_H__
#define __Serializable_H__

#include "Attribute.h"
#include "Engine/Resource/Resource.h"

namespace ma
{
	class AttributeManager;
	class Serializer;

	/// Vector of variants.
	typedef std::vector<Any> VariantVector;

	/// Map of variants.
	typedef std::map<std::string, Any> VariantMap;
	
	/// Base class for objects with automatic serialization through attributes.
	class Serializable : public Resource
	{
		//OBJECT(Serializable);

	public:
		/// Construct.
		Serializable();
		
		/// Destruct.
		virtual ~Serializable();

		virtual void Serialize(Serializer& sl, const char* pszLable = "");

		/// Handle attribute write access. Default implementation writes to the variable at offset, or invokes the set accessor.
		virtual void OnSetAttribute(const AttributeInfo& attr, const Any& src);

		/// Handle attribute read access. Default implementation reads the variable at offset, or invokes the get accessor.
		virtual void OnGetAttribute(const AttributeInfo& attr, Any& dest) const;

		virtual const AttributeInfo*	GetAttributeInfoByName(const char* strName) const;

		/// Apply attribute changes that can not be applied immediately. Called after scene load or a network update.
		//virtual void ApplyAttributes() {}

		/// Return whether should save default-valued attributes into XML. Default false.
		virtual bool SaveDefaultAttributes() const { return false; }

		/// Set attribute by index. Return true if successfully set.
		bool SetAttribute(unsigned index, const Any& value);

		/// Set attribute by name. Return true if successfully set.
		bool SetAttribute(const char* name, const Any& value);

		/// Reset all editable attributes to their default values.
		void ResetToDefault();

		/// Remove instance's default values if they are set previously.
		void RemoveInstanceDefault();

		/// Set temporary flag. Temporary objects will not be saved.
		void SetTemporary(bool enable);

		/// Return attribute value by index. Return empty if illegal index.
		Any GetAttribute(unsigned index) const;

		/// Return attribute value by name. Return empty if not found.
		Any GetAttribute(const char* name) const;

		/// Return attribute default value by index. Return empty if illegal index.
		Any GetAttributeDefault(unsigned index) const;

		/// Return attribute default value by name. Return empty if not found.
		Any GetAttributeDefault(const char* name) const;

		/// Return number of attributes.
		unsigned GetNumAttributes() const;

		/// Return whether is temporary.
		bool IsTemporary() const { return temporary_; }
		
	protected:
		/// Set instance-level default value. Allocate the internal data structure as necessary.
		void SetInstanceDefault(const char* name, const Any& defaultValue);

		/// Get instance-level default value.
		Any GetInstanceDefault(const char* name) const;

		/// Return attribute descriptions, or null if none defined.
		const vector<AttributeInfo>* GetAttributes() const;

		

	private:

		/// Attribute default value at each instance level.
		VariantMap* instanceDefaultValues_;

		/// Temporary flag.
		bool temporary_;
	};
}

#endif
