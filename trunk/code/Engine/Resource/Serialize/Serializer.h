#ifndef __SerializeListener_H__
#define __SerializeListener_H__

namespace ma
{
	class Object;
	class Serializer;

	template<class T>
	void SerializeData(Serializer& sl, T& val,const char* pszLable = "")
	{
		val.Serialize(sl,pszLable);
	}


	class ENGINE_API Serializer 
	{
	public:

		virtual ~Serializer() {}

		virtual void	Serialize(bool& val,const char* pszLable) = 0;

		virtual void	Serialize(UINT& val,const char* pszLable) = 0;

		virtual void	Serialize(int& val,const char* pszLable) = 0;

		virtual void	Serialize(unsigned short& val,const char* pszLable) = 0;

		virtual void	Serialize(long& val,const char* pszLable) = 0;

		virtual void	Serialize(Uint64& val,const char* pszLable) = 0;

		virtual void	Serialize(float& val,const char* pszLable) = 0;

		virtual void	Serialize(std::string& val,const char* pszLable) = 0;

		virtual void	Serialize(Vector2 &val,const char* pszLable) = 0;

		virtual void	Serialize(maGUID &val,const char* pszLable) = 0;

		virtual void	Serialize(Vector3 &val,const char* pszLable) = 0;

		virtual void	Serialize(Vector4 &val,const char* pszLable) = 0;

		virtual void	Serialize(Quaternion &val,const char* pszLable) = 0;

		virtual void	Serialize(Transform& val,const char *pszLable) = 0;

		virtual void	SerializeByte(Uint8* &pData,UINT nSizeInByte,const char* pszLable) = 0;

		virtual void	BeginSection(const char* pszLable) = 0;

		virtual void	EndSection() = 0;

		virtual bool	IsReading() const = 0;

		template<class EnumType>
		void			Serialize(EnumType& type,const char** pEnumNames,const char *pszLable);

		//template<class ClassType,class ValueType>
		//void Serialize(const ClassType* classIns, const ValueType& (ClassType::*GetValue)() const, 
		//	void (ClassType::*SetValue)(const ValueType&), char *pszLable = "Value");

		template<class T>
		void			Serialize(std::vector<T>& val,const char* pszLable);

		template<class T>
		void			Serialize(std::vector<T*>& val,const char* pszLable);

		template<class T>
		void			Serialize(std::vector< RefPtr<T> >& val,const char* pszLable);

		template<class T>
		void			Serialize(T& val,const char* pszLable);

		template<class T>
		void			Serialize(T* val,const char* pszLable);
	};

// 	template<class ClassType,class ValueType>
// 	void Serializer::Serialize(const ClassType* classIns, const ValueType& (ClassType::*GetValue)() const, 
// 		void (ClassType::*SetValue)(const ValueType&), char *pszLable)
// 	{
// 		if ( IsReading() )
// 		{
// 			const ValueType& value = (classIns->*GetValue)();
// 			Serialize(value,pszLable);
// 		}
// 		else
// 		{
// 			ValueType value;
// 			Serialize(value,pszLable);
// 			//(classIns->*SetValue)(value);
// 		}
// 	}

	template<class EnumType>
	void Serializer::Serialize(EnumType& type,const char** pEnumNames,const char *pszLable/* = "Enum"*/)
	{
		std::string strEnumName = pEnumNames[type];
		
		Serialize(strEnumName,pszLable);

		if ( this->IsReading() )
		{
			bool enumFound = false;
			int enumValue = 0;
			while (*pEnumNames)
			{
				if ( strcmp( *pEnumNames, strEnumName.c_str() ) == 0 )
				{
					enumFound = true;
					break;
				}
				++pEnumNames;
				++enumValue;
			}

			if (enumFound)
			{
				type = (EnumType)enumValue;
			}
			else
			{
				ASSERT(false);
			}
		}
	}

	template<class T>
	void Serializer::Serialize(T& val,const char* pszLable)
	{
		SerializeData(*this,val,pszLable);
	}

	template<class T>
	void Serializer::Serialize(T* val,const char* pszLable)
	{
		SerializeData(*this,val,pszLable);
	}

	template<class T>
	void Serializer::Serialize(std::vector<T>& val,const char* pszLable)
	{
		BeginSection(pszLable);

		UINT nSize = (UINT)val.size();
		Serialize(nSize,"size");

		if (nSize != val.size())
		{
			val.resize(nSize);
		}

		for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);
			Serialize(val[nCnt],buf);
		}

		EndSection();
	}

	template<class T>
	void Serializer::Serialize(std::vector<T*>& val,const char* pszLable)
	{
		BeginSection(pszLable);

		UINT nSize = (UINT)val.size();
		Serialize(nSize,"size");

		if (nSize != val.size())
		{
			val.resize(nSize);
		}

		for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);
			if (val[nCnt] == NULL)
			{
				val[nCnt] = new T();
			}
			Serialize(*(val[nCnt]),buf);
		}

		EndSection();
	}

	template<class T>
	void Serializer::Serialize(std::vector< RefPtr<T> >& val,const char* pszLable /*= "array"*/)
	{
		BeginSection(pszLable);

		UINT nSize = (UINT)val.size();
		Serialize(nSize,"size");

		if (nSize != val.size())
		{
			val.resize(nSize);
		}

		for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);
			if (val[nCnt] == NULL)
			{
				val[nCnt] = new T();
			}
			Serialize(*(val[nCnt]),buf);
		}

		EndSection();
	}


	template<class T>
	inline void SerializeObject(Serializer& sl, T* &pObject, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		std::string strClassName = pObject ? pObject->GetClass()->GetName() : "";
		sl.Serialize(strClassName,"ClassName");

		if (pObject == NULL)
		{
			ObjectFactoryManager& objFac = ObjectFactoryManager::GetInstance();
			pObject = SafeCast<T>( objFac.CreateObject(strClassName.c_str()) );
		}
		sl.Serialize(*(pObject),pszLable);

		sl.EndSection();
	}

	template<class T>
	inline void SerializeObjectArg(Serializer& sl, T* &pObject, void* arg,const char* pszLable)
	{
		sl.BeginSection(pszLable);

		std::string strClassName = pObject ? pObject->GetClass()->GetName() : "";
		sl.Serialize(strClassName,"ClassName");

		if (pObject == NULL)
		{
			ObjectFactoryManager& objFac = ObjectFactoryManager::GetInstance();
			pObject = SafeCast<T>( objFac.CreateObjectArg(strClassName.c_str(),arg) );
		}
		sl.Serialize(*(pObject),pszLable);

		sl.EndSection();
	}


	template<class T>
	inline void SerializeArrObj(Serializer& sl,std::vector<T*>& arrObject,const char* pszLable)
	{
		sl.BeginSection(pszLable);

		UINT nSize = arrObject.size();
		sl.Serialize(nSize,"size");

		if (nSize != arrObject.size())
		{
			arrObject.resize(nSize);
		}


		for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);

			SerializeObject<T>(sl,arrObject[nCnt], buf);
		}

		sl.EndSection();
	}

	template<class T>
	inline void SerializeArrObjArg(Serializer& sl,std::vector<T*>& arrObject,void* arg,const char* pszLable)
	{
		sl.BeginSection(pszLable);

		UINT nSize = arrObject.size();
		sl.Serialize(nSize,"size");

		if (nSize != arrObject.size())
		{
			arrObject.resize(nSize);
		}

		for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);

			SerializeObjectArg<T>(sl,arrObject[nCnt],arg,buf);
		}

		sl.EndSection();
	}
   
	template<class T>
	inline void SerializeAnyTypeValue(Serializer& sl, Any& anyValue)
	{
		if ( sl.IsReading() ) 
		{
			T vec;
			sl.Serialize(vec,"Value");
			anyValue = Any(vec);
		}
		else
		{
			T vec = any_cast<T>(anyValue);
			sl.Serialize(vec,"Value");
		}
	}

	inline void SerializeAnyValue(Serializer& sl, std::string& strType,Any& anyValue)
	{
		sl.Serialize(strType,"type");

		if (strType == "int")
		{
			SerializeAnyTypeValue<int>(sl,anyValue);
		}
		if (strType == "float")
		{
			SerializeAnyTypeValue<float>(sl,anyValue);
		}
		else if (strType == "Vector2")
		{
			SerializeAnyTypeValue<Vector2>(sl,anyValue);
		}
		else if (strType == "Vector3")
		{
			SerializeAnyTypeValue<Vector3>(sl,anyValue);
		}
		else if (strType == "Vector4")
		{
			SerializeAnyTypeValue<Vector4>(sl,anyValue);
		}
// 		else 
// 		{
// 			ASSERT(false);
// 		}
	}
}

#endif// __Serialize_H__