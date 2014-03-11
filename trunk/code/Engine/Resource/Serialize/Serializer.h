#ifndef __SerializeListener_H__
#define __SerializeListener_H__

namespace ma
{
	class Object;

	template<class T>
	void SerializeData(Serializer& sl, T& val,const char* pszLable = "")
	{
		val.Serialize(sl,pszLable);
	}

	class ENGINE_API Serializer 
	{
	public:

		virtual ~Serializer();

		virtual void Serialize(bool& val,const char* pszLable = "bool");

		virtual void Serialize(unsigned char& val, const char* pszLabel = "unsigned char");

		virtual void Serialize(short& val,const char* pszLable = "short");	

		virtual void Serialize(unsigned short& val, const char* pszLabel = "unsigned short");

		virtual void Serialize(UINT&val,const char* pszLable = "UINT");

		virtual void Serialize(int&val,const char* pszLable = "int");

		virtual void Serialize(unsigned long &val,const char* pszLable = "unsigned long");

		virtual void Serialize(long &val,const char* pszLable = "long");

		virtual void Serialize(Uint64&val,const char* pszLable = "Uint64");

		virtual void Serialize(float& val,const char* pszLable = "float");

		virtual void Serialize(std::string& val,const char* pszLable = "string");

		virtual void Serialize(Vector2 &val,const char* pszLable = "Vector2");

		virtual void Serialize(maGUID &val,const char* pszLable = "GUID");

		//virtual void Serialize(const float* arrFloat, UINT nCount,const char* pszLable = "arrFloat");

		virtual void Serialize(Vector3 &val,const char* pszLable = "Vector3");

		virtual void Serialize(Vector4 &val,const char* pszLable = "Vector4");

		virtual void Serialize(float* &arrVal, UINT &nCount, const char* pszLable = "arrFloat");

		virtual void Serialize(Quaternion &val,const char* pszLable = "Quaternion");

		virtual void Serialize(Matrix4x4 &val,const char* pszLable = "Matrix4x4");

		virtual void Serialize(NodeTransform& val,const char *pszLable = "NodeTransform");

		template<class T>
		void Serialize(std::vector<T>& val,const char* pszLable = "array");

		template<class T>
		void Serialize(std::vector<T*>& val,const char* pszLable = "array");

		template<class T>
		void Serialize(T& val,const char* pszLable = "")
		{
			SerializeData(*this,val,pszLable);
		}

		template<class T>
		void Serialize(T* val,const char* pszLable = "")
		{
			SerializeData(*this,val,pszLable);
		}

		////////////////
		virtual void BeginSection(const char* pszLable);

		virtual void EndSection();

		//virtual const char* GetFirstChildName();

		//virtual const char* GetNextChildName();

		////////////////////
		virtual bool SerializeByte(Uint8* &pData,UINT nSizeInByte,const char* pszLable = "Bytes") {return false;}

		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		virtual UINT Tell() {return 0;}

		virtual void Seek(UINT nPos) {}
// 
// 		virtual void SkipByte(UINT nSize) {}
		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		virtual bool IsReading() const = 0;
	};


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
		BeginSection("element");

		for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);
			Serialize(val[nCnt],buf);
		}
		EndSection();

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
		//BeginSection("element");

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
		//EndSection();

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



                                           
}











#endif// __SerializeListener_H__