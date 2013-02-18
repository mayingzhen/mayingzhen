#ifndef __SerializeListener_H__
#define __SerializeListener_H__

namespace ma
{
	class Object;

	class COMMON_API SerializeListener 
	{
		std::deque<xmUint> m_verStack;

	public:

		virtual ~SerializeListener();

		virtual void Serialize(bool& val,const char* pszLable = "bool");

		virtual void Serialize(unsigned char& val, const char* pszLabel = "unsigned char");

		virtual void Serialize(short& val,const char* pszLable = "short");	

		virtual void Serialize(unsigned short& val, const char* pszLabel = "unsigned short");

		virtual void Serialize(unsigned int&val,const char* pszLable = "unsigned int");

		virtual void Serialize(int&val,const char* pszLable = "int");

		virtual void Serialize(unsigned long &val,const char* pszLable = "unsigned long");

		virtual void Serialize(long &val,const char* pszLable = "long");

		virtual void Serialize(xmUint64&val,const char* pszLable = "xmUint64");

		virtual void Serialize(float& val,const char* pszLable = "float");

		virtual void Serialize(std::string& val,const char* pszLable = "string");

		//virtual void Serialize(IStringWrapper* val,const char* pszLable = "string");

		virtual void Serialize(D3DXVECTOR2 &val,const char* pszLable = "Vector2");

		virtual void Serialize(GUID &val,const char* pszLable = "GUID");

		virtual void Serialize(D3DXVECTOR3 &val,const char* pszLable = "Vector3");

		virtual void Serialize(D3DXVECTOR4 &val,const char* pszLable = "Vector4");

		virtual void Serialize(D3DXQUATERNION &val,const char* pszLable = "Quaternion");

		virtual void Serialize(D3DXMATRIX &val,const char* pszLable = "Matrix4x4");

		virtual void Serialize(maNodeTransform& val,const char *pszLable = "NodeTransform");

		template<class T>
		void Serialize(std::vector<T>& val,const char* pszLable = "array");

		template<class T>
		void Serialize(std::vector<T*>& val,const char* pszLable = "array");

		template<class T>
		void Serialize(T& val,const char* pszLable = "")
		{
			//Serialize(*this,val,pszLable);
			val.Serialize(*this,pszLable);
		}

		void SerializeObjectArray(std::vector<Object*>& vObject, const char* pszLable = "array");

		template<class T>
		void SerializeObjectArray(std::vector<T*>& vObject, const char* pszLable = "array")
		{
			BeginSection(pszLable);

			xmUint nSize = (xmUint)vObject.size();
			Serialize(nSize,"size");

			if (nSize != vObject.size())
			{
				vObject.resize(nSize);
			}
			BeginSection("element");

			for (xmUint nCnt = 0;nCnt < nSize; ++nCnt)
			{
				char buf[32];
				sprintf(&buf[0],"Element_%u",nCnt);
				std::string sTypeName;
				if (vObject[nCnt])
				{
					Class* pClass = vObject[nCnt]->GetClass();
					assert(pClass);
					if (pClass)
						sTypeName = pClass->GetName();
				}

				this->Serialize(sTypeName,"ObjectTypeName");

				if (vObject[nCnt] == NULL)
				{
					vObject[nCnt] = (T*)ObjectFactoryManager::GetInstance().CreateObject(sTypeName.c_str());
				}

				vObject[nCnt]->Serialize(*this);
			}
			EndSection();

			EndSection();
		}

// 		template<class T>
// 		void Serialize(T* val,const char* pszLable = "")
// 		{
// 			Serialize(*this,val,pszLable);
// 			//val->Serialize(this,pszLable);
// 		}


		template<class DataType>
		void SerializeRawData(std::vector<xmUint8>& val,const char* pszLable);

		////////////////
		virtual void BeginSection(const char* pszLable);

		virtual void EndSection();

		virtual xmUint GetVersion() const;

		virtual void PushVersion(xmUint nVersion);

		virtual xmUint PopVersion();
		////////////////////
		
		virtual bool SerializeByte(xmUint8* pData,xmUint nSizeInByte,const char* pszLable = "Bytes") {return false;}

		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		virtual xmUint Tell() {return 0;}

		virtual void Seek(xmUint nPos) {}

		virtual void SkipByte(xmUint nSize) {}
		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		virtual bool IsReading() const = 0;
	};


// 	template<class T>
// 	void Serialize(SerializeListener& sl,T& val,const char* pszLable)
// 	{
// 		val.Serialize(sl,pszLable);
// 	}


	template<class T>
	void SerializeListener::Serialize(std::vector<T>& val,const char* pszLable)
	{
		BeginSection(pszLable);

		xmUint nSize = (xmUint)val.size();
		Serialize(nSize,"size");

		if (nSize != val.size())
		{
			val.resize(nSize);
		}
		BeginSection("element");

		for (xmUint nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);
			Serialize(val[nCnt],buf);
		}
		EndSection();

		EndSection();
	}

	template<class T>
	void SerializeListener::Serialize(std::vector<T*>& val,const char* pszLable)
	{
		BeginSection(pszLable);

		xmUint nSize = (xmUint)val.size();
		Serialize(nSize,"size");

		if (nSize != val.size())
		{
			val.resize(nSize);
		}
		BeginSection("element");

		for (xmUint nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);
			if (val[nCnt] == NULL)
			{
				val[nCnt] = new T();
			}
			//Serialize(val[nCnt],buf);
			val[nCnt]->Serialize(*this);
		}
		EndSection();

		EndSection();
	}

	template<class DataType>
	void SerializeListener::SerializeRawData(std::vector<xmUint8>& val,const char* pszLable)
	{
		BeginSection(pszLable);

		xmUint nSize = val.size();
		Serialize(nSize,"size");

		if (nSize != val.size())
		{
			val.resize(nSize);
		}

		BeginSection("element");

		xmUint nDataNum = nSize / sizeof(DataType);

		for (xmUint nCnt = 0;nCnt < nDataNum; ++nCnt)
		{
			char buf[64];
			sprintf_s(&buf[0],64,"Element_%u",nCnt);
			Serialize((DataType&)val[nCnt*sizeof(DataType)],buf);
		}
		EndSection();

		EndSection();
	}
                                             


}











#endif// __SerializeListener_H__