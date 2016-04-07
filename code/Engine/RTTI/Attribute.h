
#ifndef __AttributeInfo_H__
#define __AttributeInfo_H__

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

	class Object;

	/// Internal helper class for invoking attribute accessors.
	class AttributeAccessor : public Referenced
	{
	public:
		/// Get the attribute.
		virtual void Get(const Object* ptr, Any& dest) const {}
		
		/// Set the attribute.
		virtual void Set(Object* ptr, const Any& src) {}
	};

	/// Description of an automatically serializable variable.
	struct AttributeInfo : public Referenced
	{
		/// Construct empty.
		AttributeInfo();
	
		/// Construct accessor attribute.
		AttributeInfo(const char* name, AttributeAccessor* accessor, const Any& defaultValue, unsigned mode);
	    
		const char* GetName() const;
		void SetName(const char* pszName);

		/// Enum names.
		//const char** enumNames_;
		/// Helper object for accessor mode.
		RefPtr<AttributeAccessor> accessor_;
		/// Default value for network replication.
		Any defaultValue_;
		/// Attribute mode: whether to use for serialization, network replication, or both.
		unsigned mode_;
		/// Attribute data pointer if elsewhere than in the Object.

	private:
		/// Name.
		std::string name_;
	};

	RefPtr<AttributeInfo> CreateAttributeInfo();
	RefPtr<AttributeInfo> CreateAttributeInfo(const char* name, AttributeAccessor* accessor, const Any& defaultValue, unsigned mode);


	/// Template implementation of the attribute accessor invoke helper class.
	template <class T, class U> class AttributeImpl : public AttributeAccessor
	{
	public:

		/// Construct with function pointers.
		AttributeImpl(size_t offset)
		{
			offset_ = offset;
		}

		/// Invoke getter function.
		virtual void Get(const Object* ptr, Any& dest) const
		{
			assert(ptr);

			const unsigned char* classPtr = reinterpret_cast<const unsigned char*>(ptr);

			const U* src = reinterpret_cast<const U*>(classPtr + offset_);

			dest = Any(*src);
		}

		/// Invoke setter function.
		virtual void Set(Object* ptr, const Any& value)
		{
			assert(ptr);

			// Calculate the destination address
			unsigned char* classPtr = reinterpret_cast<unsigned char*>(ptr);
			
			U* dest = reinterpret_cast<U*>(classPtr + offset_);

			*dest = any_cast<U>(value);
		}

		unsigned offset_;
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
		virtual void Get(const Object* ptr, Any& dest) const
		{
			assert(ptr);
			const T* classPtr = static_cast<const T*>(ptr);
			dest = (classPtr->*getFunction_)();
		}

		/// Invoke setter function.
		virtual void Set(Object* ptr, const Any& value)
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
		virtual void Get(const Object* ptr, Any& dest) const
		{
			assert(ptr);
			const T* classPtr = static_cast<const T*>(ptr);
			dest = (classPtr->*getFunction_)();
		}

		/// Invoke setter function.
		virtual void Set(Object* ptr, const Any& value)
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
	template <class T, class U> class EnumAttributeImpl : public AttributeAccessor
	{
	public:
		const char** m_arrEnumNames;

		size_t m_offset;

		/// Construct with function pointers.
		EnumAttributeImpl(const char** arrEnumNames,size_t offset) :
		m_offset(offset),
		m_arrEnumNames(arrEnumNames)
		{
			ASSERT(m_arrEnumNames);
		}

		virtual void Get(const Object* ptr, Any& dest) const
		{
			ASSERT(ptr);

			const unsigned char* classPtr = reinterpret_cast<const unsigned char*>(ptr);

			const U* src = reinterpret_cast<const U*>(classPtr + m_offset);

			U eValue = *src;

			const char* pszValue = m_arrEnumNames[eValue];

			dest = Any(pszValue);
		}

		/// Invoke setter function.
		virtual void Set(Object* ptr, const Any& value)
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

			unsigned char* classPtr = reinterpret_cast<unsigned char*>(ptr);

			U* dest = reinterpret_cast<U*>(classPtr + m_offset);

			*dest = static_cast<U>(nEnumValue);
		}
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
		virtual void Get(const Object* ptr, Any& dest) const
		{
			ASSERT(ptr);
			const T* classPtr = static_cast<const T*>(ptr);
			U eValue = (classPtr->*getFunction_)();

			const char* pszValue = m_arrEnumNames[eValue];
		
			dest = Any(pszValue);
		}

		/// Invoke setter function.
		virtual void Set(Object* ptr, const Any& value)
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

