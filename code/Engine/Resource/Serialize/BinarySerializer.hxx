#include "BinarySerializer.h"

namespace ma
{
	void BinarySerializer::Serialize(bool& val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(val),pszLable);
	}

	void BinarySerializer::Serialize(UINT&val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(val),pszLable);
	}


	void BinarySerializer::Serialize(int&val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(val),pszLable);
	}


	void BinarySerializer::Serialize(unsigned short &val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(val),pszLable);
	}

	void BinarySerializer::Serialize(long &val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(val),pszLable);
	}


	void BinarySerializer::Serialize(Uint64&val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(val),pszLable);
	}


	void BinarySerializer::Serialize(float& val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(val),pszLable);
	}


	void BinarySerializer::Serialize(std::string& val,const char* pszLable)
	{
		UINT nSize = (UINT)(val.size()+1);
		Serialize(nSize,pszLable);
		val.resize(nSize,'\0');
		if (nSize > 0)
		{
			Uint8* pBuf = (Uint8*)&val[0];
			SerializeByte(pBuf,nSize,pszLable);
		}
	}

	void BinarySerializer::Serialize(Vector2 &val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(Vector2),pszLable);
	}


	void BinarySerializer::Serialize(maGUID &val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(maGUID),pszLable);
	}

	void BinarySerializer::Serialize(Vector3 &val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(Vector3),pszLable);
	}


	void BinarySerializer::Serialize(Vector4 &val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(Vector4),pszLable);
	}


	void BinarySerializer::Serialize(Quaternion &val,const char* pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(Quaternion),pszLable);
	}

	void BinarySerializer::Serialize(Transform& val,const char *pszLable)
	{
		Uint8* pTemp = (Uint8*)&val;
		SerializeByte(pTemp,sizeof(Transform),pszLable);
	}

	void BinarySerializer::BeginSection(const char* pszLable)
	{
	}

	void BinarySerializer::EndSection()
	{

	}


}
