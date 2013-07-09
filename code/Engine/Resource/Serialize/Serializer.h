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

		virtual void Serialize(unsigned int&val,const char* pszLable = "unsigned int");

		virtual void Serialize(int&val,const char* pszLable = "int");

		virtual void Serialize(unsigned long &val,const char* pszLable = "unsigned long");

		virtual void Serialize(long &val,const char* pszLable = "long");

		virtual void Serialize(Uint64&val,const char* pszLable = "Uint64");

		virtual void Serialize(float& val,const char* pszLable = "float");

		virtual void Serialize(std::string& val,const char* pszLable = "string");

		//virtual void Serialize(IStringWrapper* val,const char* pszLable = "string");

		virtual void Serialize(Vector2 &val,const char* pszLable = "Vector2");

		virtual void Serialize(maGUID &val,const char* pszLable = "GUID");

		virtual void Serialize(Vector3 &val,const char* pszLable = "Vector3");

		virtual void Serialize(Vector4 &val,const char* pszLable = "Vector4");

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

// 		//template<class T>
// 		ArchiveObj* SerializeObject(ArchiveObj* val,const char* pszLable = "")
// 		{
// 			if (this->IsReading())
// 			{	
// 				BeginSection(pszLable);
// 
// 				ASSERT(val == NULL);
// 
// 				std::string strClassName;
// 
// 				Serialize(strClassName,"classType");
// 
// 				val = (ArchiveObj*)ObjectFactoryManager::GetInstance().CreateObject(strClassName.c_str());
// 				ASSERT(val);
// 				
// 				val->Serialize(*this,pszLable);
// 
// 				EndSection();
// 
// 				return val;
// 			}
// 			else
// 			{
// 				BeginSection(pszLable);
// 
// 				ASSERT(val);
// 
// 				std::string strClassName = val->GetClass()->GetName(); 
// 
// 				Serialize(strClassName,"classType");
// 
// 				val->Serialize(*this,pszLable);
// 
// 				EndSection();
// 
// 				return val;
// 			}
// 			
// 			//SerializeData(*this,val,pszLable);
// 		}

		template<class T>
		void Serialize(T* val,const char* pszLable = "")
		{
			SerializeData(*this,val,pszLable);
		}

 		//template<class T>
 		//void SerializeObjectArray(std::vector<T*>& vObject, const char* pszLable = "array");


		////////////////
		virtual void BeginSection(const char* pszLable);

		virtual void EndSection();

		//virtual const char* GetFirstChildName();

		//virtual const char* GetNextChildName();

		////////////////////
		
		virtual bool SerializeByte(Uint8* pData,UINT nSizeInByte,const char* pszLable = "Bytes") {return false;}

		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		virtual UINT Tell() {return 0;}

		virtual void Seek(UINT nPos) {}

		virtual void SkipByte(UINT nSize) {}
		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		virtual bool IsReading() const = 0;
	};


// 
// 	template<class T>
// 	void Serializer::SerializeObjectArray(std::vector<T*>& vObject, const char* pszLable)
// 	{
// 		BeginSection(pszLable);
// 	
// 		if (this->IsReading())
// 		{
// 			UINT nSize = 0;
// 
// 			Serialize(nSize,"arrSize");
// 
// 			vObject.resize(nSize);
// 
// 			for (UINT nCnt = 0; nCnt < nSize; ++nCnt)
// 			{
// 				std::string str = std::string("arr_") + StringConverter::ToString(nCnt);
// 				BeginSection(str.c_str());
// 				
// 				std::string strClassName;
// 				Serialize(strClassName,"classType");
// 
// 				vObject[nCnt] = (T*)ObjectFactoryManager::GetInstance().CreateObject(strClassName.c_str());
// 				ASSERT(vObject[nCnt]);
// 				if (vObject[nCnt] == NULL)
// 					continue;
// 
// 				vObject[nCnt]->Serialize(*this,strClassName.c_str());
// 
// 				EndSection();
// 			}
// 		}
// 		else
// 		{
// 			UINT nSize = vObject.size();
// 			
// 			Serialize(nSize,"arrSize");
// 
// 			for (UINT nCnt = 0; nCnt < nSize; ++nCnt)
// 			{
// 				std::string str = std::string("arr_") + StringConverter::ToString(nCnt);
// 				BeginSection(str.c_str());
// 
// 				const RTTIClass* pClass = vObject[nCnt]->GetClass();
// 				ASSERT(pClass);
// 				if (pClass == NULL)
// 					continue;
// 				
// 				std::string strClassName = pClass->GetName();
// 				Serialize(strClassName,"classType");
// 
// 				vObject[nCnt]->Serialize(*this,strClassName.c_str());
// 			
// 				EndSection();
// 			}
// 		}
// 		
// 		EndSection();
// 
// // 		BeginSection(pszLable);
// // 		
// // 		if (this->IsReading())
// // 		{
// // 			const char* pClassName = GetFirstChildName();
// // 			while (pClassName)
// // 			{
// // 				T* pObj = (T*)ObjectFactoryManager::GetInstance().CreateObject(pClassName);
// // 				ASSERT(pObj);
// // 				if (pObj == NULL)
// // 					continue;
// // 
// // 				pObj->Serialize(*this,pClassName);
// // 
// // 				vObject.push_back(pObj);
// // 
// // 				pClassName = GetNextChildName();
// // 			}
// // 		}
// // 		else
// // 		{
// // 			for (UINT nCnt = 0; nCnt < vObject.size(); ++nCnt)
// // 			{
// // 				const RTTIClass* pClass = vObject[nCnt]->GetClass();
// // 				ASSERT(pClass);
// // 				if (pClass == NULL)
// // 					continue;
// // 
// // 				vObject[nCnt]->Serialize(*this,pClass->GetName());
// // 			}
// // 		}
// // 		
// // 		EndSection();
// 
// 	}

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
		BeginSection("element");

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

		EndSection();
	}

                                           
}











#endif// __SerializeListener_H__