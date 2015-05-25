
#ifndef __AttributeInfo_H__
#define __AttributeInfo_H__


//#include "Variant.h"

namespace ma
{

	/// Attribute shown only in the editor, but not serialized.
	static const unsigned AM_EDIT = 0x0;
	/// Attribute used for file serialization.
	static const unsigned AM_FILE = 0x1;
	/// Attribute used for network replication.
	static const unsigned AM_NET = 0x2;
	/// Attribute used for both file serialization and network replication (default.)
	static const unsigned AM_DEFAULT = 0x3;
	/// Attribute should use latest data grouping instead of delta update in network replication.
	static const unsigned AM_LATESTDATA = 0x4;
	/// Attribute should not be shown in the editor.
	static const unsigned AM_NOEDIT = 0x8;
	/// Attribute is a node ID and may need rewriting.
	static const unsigned AM_NODEID = 0x10;
	/// Attribute is a component ID and may need rewriting.
	static const unsigned AM_COMPONENTID = 0x20;
	/// Attribute is a node ID vector where first element is the amount of nodes.
	static const unsigned AM_NODEIDVECTOR = 0x40;

	class Serializable;

	/// Internal helper class for invoking attribute accessors.
	class AttributeAccessor : public Referenced
	{
	public:
		/// Get the attribute.
		virtual void Get(const Serializable* ptr, Any& dest) const {}
		
		/// Set the attribute.
		virtual void Set(Serializable* ptr, const Any& src) {}
	};

	/// Description of an automatically serializable variable.
	struct AttributeInfo
	{
		/// Construct empty.
		AttributeInfo() :
			offset_(0),
			enumNames_(0),
			mode_(AM_DEFAULT),
			ptr_(0)
		{
		}
	    
		/// Construct offset attribute.
		AttributeInfo(const char* name, size_t offset, const Any& defaultValue, unsigned mode) :
			//type_(type),
			name_(name),
			offset_((unsigned)offset),
			enumNames_(0),
			defaultValue_(defaultValue),
			mode_(mode),
			ptr_(0)
		{
		}
	    
		/// Construct offset enum attribute.
		AttributeInfo(const char* name, size_t offset, const char** enumNames, const Any& defaultValue, unsigned mode) :
			//type_(VAR_INT),
			name_(name),
			offset_((unsigned)offset),
			enumNames_(enumNames),
			defaultValue_(defaultValue),
			mode_(mode),
			ptr_(0)
		{
		}
	    
		/// Construct accessor attribute.
		AttributeInfo(const char* name, AttributeAccessor* accessor, const Any& defaultValue, unsigned mode) :
			//type_(type),
			name_(name),
			offset_(0),
			enumNames_(0),
			accessor_(accessor),
			defaultValue_(defaultValue),
			mode_(mode),
			ptr_(0)
		{
		}
	    
		/// Construct accessor enum attribute.
		AttributeInfo(const char* name, AttributeAccessor* accessor, const char** enumNames, const Any& defaultValue, unsigned mode) :
			//type_(VAR_INT),
			name_(name),
			offset_(0),
			enumNames_(enumNames),
			accessor_(accessor),
			defaultValue_(defaultValue),
			mode_(mode),
			ptr_(0)
		{
		}
	    
		/// Attribute type.
		//VariantType type_;
		/// Name.
		std::string name_;
		/// Byte offset from start of object.
		unsigned offset_;
		/// Enum names.
		const char** enumNames_;
		/// Helper object for accessor mode.
		RefPtr<AttributeAccessor> accessor_;
		/// Default value for network replication.
		Any defaultValue_;
		/// Attribute mode: whether to use for serialization, network replication, or both.
		unsigned mode_;
		/// Attribute data pointer if elsewhere than in the Serializable.
		void* ptr_;
	};


	/// Template implementation of the attribute accessor invoke helper class.
	template <class T, class U> class AttributeAccessorImpl : public AttributeAccessor
	{
	public:
		typedef U (T::*GetFunctionPtr)() const;
		typedef void (T::*SetFunctionPtr)(U);

		/// Construct with function pointers.
		AttributeAccessorImpl(GetFunctionPtr getFunction, SetFunctionPtr setFunction) :
		getFunction_(getFunction),
			setFunction_(setFunction)
		{
			assert(getFunction_);
			assert(setFunction_);
		}

		/// Invoke getter function.
		virtual void Get(const Serializable* ptr, Any& dest) const
		{
			assert(ptr);
			const T* classPtr = static_cast<const T*>(ptr);
			dest = (classPtr->*getFunction_)();
		}

		/// Invoke setter function.
		virtual void Set(Serializable* ptr, const Any& value)
		{
			assert(ptr);
			T* classPtr = static_cast<T*>(ptr);
			(classPtr->*setFunction_)( any_cast<U>(value) );
		}

		/// Class-specific pointer to getter function.
		GetFunctionPtr getFunction_;

		/// Class-specific pointer to setter function.
		SetFunctionPtr setFunction_;
	};

	/// Template implementation of the attribute accessor invoke helper class using const references.
	template <class T, class U> class RefAttributeAccessorImpl : public AttributeAccessor
	{
	public:
		typedef const U& (T::*GetFunctionPtr)() const;
		typedef void (T::*SetFunctionPtr)(const U&);

		/// Construct with function pointers.
		RefAttributeAccessorImpl(GetFunctionPtr getFunction, SetFunctionPtr setFunction) :
		getFunction_(getFunction),
			setFunction_(setFunction)
		{
			assert(getFunction_);
			assert(setFunction_);
		}

		/// Invoke getter function.
		virtual void Get(const Serializable* ptr, Any& dest) const
		{
			assert(ptr);
			const T* classPtr = static_cast<const T*>(ptr);
			dest = (classPtr->*getFunction_)();
		}

		/// Invoke setter function.
		virtual void Set(Serializable* ptr, const Any& value)
		{
			assert(ptr);
			T* classPtr = static_cast<T*>(ptr);
			(classPtr->*setFunction_)( any_cast<U>(value) );
		}

		/// Class-specific pointer to getter function.
		GetFunctionPtr getFunction_;
		/// Class-specific pointer to setter function.
		SetFunctionPtr setFunction_;
	};

	/// Template implementation of the attribute accessor invoke helper class using const references.
	template <class T, class U> class EnumAttributeAccessorImpl : public AttributeAccessor
	{
	public:
		typedef U (T::*GetFunctionPtr)() const;
		typedef void (T::*SetFunctionPtr)(U);

		const char** m_arrEnumNames;

		/// Construct with function pointers.
		EnumAttributeAccessorImpl(const char** arrEnumNames,GetFunctionPtr getFunction, SetFunctionPtr setFunction) :
		m_arrEnumNames(arrEnumNames),
		getFunction_(getFunction),
			setFunction_(setFunction)
		{
			ASSERT(m_arrEnumNames);
			ASSERT(getFunction_);
			ASSERT(setFunction_);
		}

		/// Invoke getter function.
		virtual void Get(const Serializable* ptr, Any& dest) const
		{
			ASSERT(ptr);
			const T* classPtr = static_cast<const T*>(ptr);
			U eValue = (classPtr->*getFunction_)();

			const char* pszValue = m_arrEnumNames[eValue];
		
			dest = Any(pszValue);
		}

		/// Invoke setter function.
		virtual void Set(Serializable* ptr, const Any& value)
		{
			ASSERT(ptr);

			const char* pszVlue = any_cast<const char*>(value);
			const char** pEnumNames = m_arrEnumNames;
			int nEnumValue = 0;
			while (*pEnumNames)
			{
				if ( strcmp( *pEnumNames, pszVlue ) == 0 )
				{
					break;
				}
				++pEnumNames;
				++nEnumValue;
			}

			T* classPtr = static_cast<T*>(ptr);
			(classPtr->*setFunction_)( static_cast<U>(nEnumValue) );
		}

		/// Class-specific pointer to getter function.
		GetFunctionPtr getFunction_;
		
		/// Class-specific pointer to setter function.
		SetFunctionPtr setFunction_;
	};

}

#endif

