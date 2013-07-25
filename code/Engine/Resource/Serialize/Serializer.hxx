#include "Serializer.h"

namespace ma
{
	Serializer::~Serializer()
	{

	}


	void Serializer::Serialize(bool& val,const char* pszLable)
	{
		SerializeByte((Uint8*)&val,sizeof(val),pszLable);
	}


	void Serializer::Serialize(short& val,const char* pszLable)
	{
		SerializeByte((Uint8*)&val,sizeof(val),pszLable);
	}

	void Serializer::Serialize(unsigned char& val, const char* pszLabel)
	{
		SerializeByte((Uint8*)&val,sizeof(val),pszLabel);
	}

	void Serializer::Serialize(unsigned short& val, const char* pszLable)
	{
		SerializeByte((Uint8*)&val,sizeof(val),pszLable);
	}


	void Serializer::Serialize(unsigned int&val,const char* pszLable)
	{
		SerializeByte((Uint8*)&val,sizeof(val),pszLable);
	}


	void Serializer::Serialize(int&val,const char* pszLable)
	{
		SerializeByte((Uint8*)&val,sizeof(val),pszLable);
	}


	void Serializer::Serialize(unsigned long &val,const char* pszLable)
	{
		SerializeByte((Uint8*)&val,sizeof(val),pszLable);
	}


	void Serializer::Serialize(long &val,const char* pszLable)
	{
		SerializeByte((Uint8*)&val,sizeof(val),pszLable);
	}


	void Serializer::Serialize(Uint64&val,const char* pszLable)
	{
		SerializeByte((Uint8*)&val,sizeof(val),pszLable);
	}


	void Serializer::Serialize(float& val,const char* pszLable)
	{
		SerializeByte((Uint8*)&val,sizeof(val),pszLable);
	}


	void Serializer::Serialize(std::string& val,const char* pszLable)
	{
		UINT nSize = (UINT)(val.size()+1);
		//bool res = SerializeByte(reinterpret_cast<Uint8*>(&nSize),sizeof(nSize),pszLable);
		Serialize(nSize,pszLable);
		val.resize(nSize,'\0');
		if (nSize > 0)
		{
			Uint8* pBuf = (Uint8*)&val[0];
			SerializeByte(pBuf,nSize,pszLable);
		}
	}

	void Serializer::Serialize(Vector2 &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(val.x,"x");
		Serialize(val.y,"y");
		EndSection();
	}


	void Serializer::Serialize(maGUID &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(val.m_a,"a");
		Serialize(val.m_b,"b");
		EndSection();
	}

	void Serializer::Serialize(Vector3 &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(val.x,"x");
		Serialize(val.y,"y");
		Serialize(val.z,"z");
		EndSection();
	}


	void Serializer::Serialize(Vector4 &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(val.x,"x");
		Serialize(val.y,"y");
		Serialize(val.z,"z");
		Serialize(val.w,"w");
		EndSection();
	}


	void Serializer::Serialize(Quaternion &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(val.x,"x");
		Serialize(val.y,"y");
		Serialize(val.z,"z");
		Serialize(val.w,"w");
		EndSection();
	}


	void Serializer::Serialize(Matrix4x4 &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(*reinterpret_cast<Vector4*>(&val._11),"X");
		Serialize(*reinterpret_cast<Vector4*>(&val._21),"Y");
		Serialize(*reinterpret_cast<Vector4*>(&val._31),"Z");
		Serialize(*reinterpret_cast<Vector4*>(&val._41),"T");
		EndSection();
	}

	void Serializer::Serialize(NodeTransform& val,const char *pszLable)
	{
		BeginSection(pszLable);
		//Serialize(val.m_vLocalScale,"NonPropScale");
		Serialize(val.m_fScale,"PropScale");
		Serialize(val.m_qRot,"Rotation");
		Serialize(val.m_vPos,"Position");
		EndSection();
	}

	void Serializer::BeginSection(const char* pszLable)
	{
	}

	void Serializer::EndSection()
	{

	}
}
