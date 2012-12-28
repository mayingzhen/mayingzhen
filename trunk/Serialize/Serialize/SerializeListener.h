#ifndef __SerializeListener_H__
#define __SerializeListener_H__


class SerializeListener 
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

	virtual void Serialize(xmVector2 &val,const char* pszLable = "Vector2");

	virtual void Serialize(GUID &val,const char* pszLable = "GUID");

	virtual void Serialize(D3DXVECTOR3 &val,const char* pszLable = "Vector3");

	virtual void Serialize(xmVector4 &val,const char* pszLable = "Vector4");

	virtual void Serialize(xmQuaternion &val,const char* pszLable = "Quaternion");

	virtual void Serialize(xmMatrix4x4 &val,const char* pszLable = "Matrix4x4");

	virtual void Serialize(xmNodeTransform& val,const char *pszLable = "NodeTransform");

	template<class T>
	void Serialize(std::vector<T>& val,const char* pszLable = "array");

	template<class T>
	void Serialize(std::vector<T*>& val,const char* pszLable = "array");

	//template<class T>
	//void SerializeArray(std::vector<T>& val,const char* pszLable = "array");

	template<class T>
	void Serialize(T& val,const char* pszLable = "");

	template<class T>
	void Serialize(T* val,const char* pszLable = "");

	template<class DataType>
	void SerializeRawData(std::vector<xmUint8>& val,const char* pszLable);

	////////////////
	virtual void BeginSection(const char* pszLable);

	virtual void EndSection();

	virtual xmUint GetVersion() const;

	virtual void PushVersion(xmUint nVersion);

	virtual xmUint PopVersion();
	////////////////////
	
	virtual bool SerializeByte(xmUint8* pData,xmUint nSizeInByte,const char* pszLable = "Bytes") = 0;

	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	virtual xmUint Tell() = 0;

	virtual void Seek(xmUint nPos) = 0;

	virtual void SkipByte(xmUint nSize) = 0;
	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	virtual bool IsReading() const = 0;

};



// template<class T>
// void Serialize(SerializeListener& pSL, T& val, const char* pszLable)
// {
// 	val.Serialize(pSL,pszLable);
// }

// template<class T>
// void SerializeListener::Serialize(T& val,const char* pszLable)
// {
// 	val.Serialize(*this,pszLable);
// 	//::Serialize(*this,val,pszLable);
// }

// template<class T>
// void Serialize(T& val,const char* pszLable)
// {
// 	val.Serialize(sl,pszLable);
// }

// 
// 
// 
// 
// 
// //------------------------------------------------------------------------------
// //
// //------------------------------------------------------------------------------
// 
// 
// inline void Serialize(bool &val,const char* pszLable = "bool")
// {
// 	sl.Serialize(val,pszLable);
// }
// 
// inline void Serialize(unsigned char &val,const char* pszLable = "unsigned char")
// {
// 	sl.Serialize(val,pszLable);
// }
// 
// inline void Serialize(unsigned short &val,const char* pszLable = "unsigned short")
// {
// 	sl.Serialize(val,pszLable);
// }
// 
// inline void Serialize(short &val,const char* pszLable = "short")
// {
// 	sl.Serialize(val,pszLable);
// }
// 
// inline void Serialize(unsigned int &val,const char* pszLable = "unsigned in")
// {
// 	sl.Serialize(val,pszLable);
// }
// 
// inline void Serialize(int &val,const char* pszLable = "int")
// {
// 	sl.Serialize(val,pszLable);
// }
// 
// inline void Serialize(unsigned long &val,const char* pszLable = "unsigned in")
// {
// 	sl.Serialize(val,pszLable);
// }
// 
// inline void Serialize(long &val,const char* pszLable = "int")
// {
// 	sl.Serialize(val,pszLable);
// }
// 
// inline void Serialize(xmUint64 &val,const char* pszLable = "bool")
// {
// 	sl.Serialize(val,pszLable);
// }
// 
// inline void Serialize(float &val,const char* pszLable = "float")
// {
// 	sl.Serialize(val,pszLable);
// }
// 
// 
// inline void Serialize(std::string &val,const char* pszLable = "string")
// {
// 	sl.Serialize(val,pszLable);
// }
// 
// template<class T>
// void Serialize(T& val,const char* pszLable)
// {
// 	val.Serialize(sl,pszLable);
// }
// 
// template<class T>
// void Serialize(std::vector<T>& val,const char* pszLable = "array")
// {
// 	sl.BeginSection(pszLable);
//     
// 	xmUint nSize = (xmUint)val.size();
// 	Serialize(sl,nSize,"size");
// 	
// 	if (nSize != val.size())
// 	{
// 		val.resize(nSize);
// 	}
// 	sl.BeginSection("element");
//     
// 	for (xmUint nCnt = 0;nCnt < nSize; ++nCnt)
// 	{
// 		char buf[32];
// 		sprintf(&buf[0],"Element_%u",nCnt);
// 		Serialize(sl,val[nCnt],buf);
// 	}
// 	sl.EndSection();
//     
// 	sl.EndSection();
// }












#endif// __SerializeListener_H__