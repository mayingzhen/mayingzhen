#include "Common/Serialize/SerializeListener.h"

namespace ma
{
	SerializeListener::~SerializeListener()
	{

	}


	void SerializeListener::Serialize(bool& val,const char* pszLable)
	{
		SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
	}


	void SerializeListener::Serialize(short& val,const char* pszLable)
	{
		SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
	}

	void SerializeListener::Serialize(unsigned char& val, const char* pszLabel)
	{
		SerializeByte((xmUint8*)&val,sizeof(val),pszLabel);
	}

	void SerializeListener::Serialize(unsigned short& val, const char* pszLable)
	{
		SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
	}


	void SerializeListener::Serialize(unsigned int&val,const char* pszLable)
	{
		SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
	}


	void SerializeListener::Serialize(int&val,const char* pszLable)
	{
		SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
	}


	void SerializeListener::Serialize(unsigned long &val,const char* pszLable)
	{
		SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
	}


	void SerializeListener::Serialize(long &val,const char* pszLable)
	{
		SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
	}


	void SerializeListener::Serialize(xmUint64&val,const char* pszLable)
	{
		SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
	}


	void SerializeListener::Serialize(float& val,const char* pszLable)
	{
		SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
	}


	void SerializeListener::Serialize(std::string& val,const char* pszLable)
	{

		xmUint nSize = (xmUint)(val.size()+1);
		SerializeByte(reinterpret_cast<xmUint8*>(&nSize),sizeof(nSize),pszLable);
		val.resize(nSize,'\0');
		if (nSize > 0)
		{
			xmUint8* pBuf = (xmUint8*)&val[0];
			SerializeByte(pBuf,nSize,pszLable);
		}
	}



	// void SerializeListener::Serialize(IStringWrapper* val,const char* pszLable)
	// {
	// 	xmUint nSize = (xmUint)(val->GetSize()+1);
	// 	SerializeByte(reinterpret_cast<xmUint8*>(&nSize),sizeof(nSize),pszLable);
	// 	val->Resize(nSize,'\0');
	// 	if (nSize > 0)
	// 	{
	// 		xmUint8* pBuf = (xmUint8*)val->GetValue();
	// 		SerializeByte(pBuf,nSize,pszLable);
	// 	}
	// }

	void SerializeListener::Serialize(D3DXVECTOR2 &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(val.x,"x");
		Serialize(val.y,"y");
		EndSection();
	}


	void SerializeListener::Serialize(GUID &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(val.m_a,"a");
		Serialize(val.m_b,"b");
		EndSection();
	}

	void SerializeListener::Serialize(D3DXVECTOR3 &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(val.x,"x");
		Serialize(val.y,"y");
		Serialize(val.z,"z");
		EndSection();
	}


	void SerializeListener::Serialize(D3DXVECTOR4 &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(val.x,"x");
		Serialize(val.y,"y");
		Serialize(val.z,"z");
		Serialize(val.w,"w");
		EndSection();
	}


	void SerializeListener::Serialize(D3DXQUATERNION &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(val.x,"x");
		Serialize(val.y,"y");
		Serialize(val.z,"z");
		Serialize(val.w,"w");
		EndSection();
	}


	void SerializeListener::Serialize(D3DXMATRIX &val,const char* pszLable)
	{
		BeginSection(pszLable);
		Serialize(*reinterpret_cast<D3DXVECTOR4*>(&val._11),"X");
		Serialize(*reinterpret_cast<D3DXVECTOR4*>(&val._21),"Y");
		Serialize(*reinterpret_cast<D3DXVECTOR4*>(&val._31),"Z");
		Serialize(*reinterpret_cast<D3DXVECTOR4*>(&val._41),"T");
		EndSection();
	}

	void SerializeListener::Serialize(maNodeTransform& val,const char *pszLable)
	{
		BeginSection(pszLable);
		//Serialize(val.m_vLocalScale,"NonPropScale");
		Serialize(val.m_fScale,"PropScale");
		Serialize(val.m_qRot,"Rotation");
		Serialize(val.m_vPos,"Position");
		EndSection();
	}

	void SerializeListener::BeginSection(const char* pszLable)
	{
		(void)pszLable;
	}

	void SerializeListener::EndSection()
	{

	}

	xmUint SerializeListener::GetVersion() const
	{
		return m_verStack.back();
	}

	void SerializeListener::PushVersion(xmUint nVersion)
	{
		m_verStack.push_back(nVersion);
	}

	xmUint SerializeListener::PopVersion()
	{
		xmUint nVer = m_verStack.back();
		m_verStack.pop_back();
		return nVer;
	}

}
