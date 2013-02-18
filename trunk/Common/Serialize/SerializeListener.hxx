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


// template<class T>
// void SerializeListener::Serialize(std::vector<T>& val,const char* pszLable)
// {
// 	BeginSection(pszLable);
// 
// 	xmUint nSize = (xmUint)val.size();
// 	Serialize(nSize,"size");
// 
// 	if (nSize != val.size())
// 	{
// 		val.resize(nSize);
// 	}
// 	BeginSection("element");
// 
// 	for (xmUint nCnt = 0;nCnt < nSize; ++nCnt)
// 	{
// 		char buf[32];
// 		sprintf(&buf[0],"Element_%u",nCnt);
// 		Serialize(val[nCnt],buf);
// 	}
// 	EndSection();
// 
// 	EndSection();
//}

// template<class T>
// void SerializeListener::Serialize(std::vector<T*>& val,const char* pszLable)
// {
// 	BeginSection(pszLable);
// 
// 	xmUint nSize = (xmUint)val.size();
// 	Serialize(nSize,"size");
// 
// 	if (nSize != val.size())
// 	{
// 		val.resize(nSize);
// 	}
// 	BeginSection("element");
// 
// 	for (xmUint nCnt = 0;nCnt < nSize; ++nCnt)
// 	{
// 		char buf[32];
// 		sprintf(&buf[0],"Element_%u",nCnt);
// 		if (val[nCnt] == NULL)
// 		{
// 			//val[nCnt] = (T*)ObjectFactoryManager::GetInstance().
// 			//	CreateObject(T::StaticGetClass()->GetName());
// 			val[nCnt] = new T();
// 			//val[nCnt] = TypeFactory.CreatebyTypeName(T::StaticTypeName());
// 		}
// 		Serialize(val[nCnt],buf);
// 		//val[nCnt]->Serialize(*this);
// 	}
// 	EndSection();
// 
// 	EndSection();
// }


void SerializeListener::SerializeObjectArray(std::vector<Object*>& vObject, const char* pszLable)
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
			vObject[nCnt] = ObjectFactoryManager::GetInstance().CreateObject(sTypeName.c_str());
		}

		vObject[nCnt]->Serialize(*this);
	}
	EndSection();

	EndSection();
}


// template<class DataType>
// void SerializeListener::SerializeRawData(std::vector<xmUint8>& val,const char* pszLable)
// {
// 	BeginSection(pszLable);
// 
// 	xmUint nSize = val.size();
// 	Serialize(nSize,"size");
// 
// 	if (nSize != val.size())
// 	{
// 		val.resize(nSize);
// 	}
// 
// 	BeginSection("element");
// 
// 	xmUint nDataNum = nSize / sizeof(DataType);
// 
// 	for (xmUint nCnt = 0;nCnt < nDataNum; ++nCnt)
// 	{
// 		char buf[64];
// 		sprintf_s(&buf[0],64,"Element_%u",nCnt);
// 		Serialize((DataType&)val[nCnt*sizeof(DataType)],buf);
// 	}
// 	EndSection();
// 
// 	EndSection();
// }


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
