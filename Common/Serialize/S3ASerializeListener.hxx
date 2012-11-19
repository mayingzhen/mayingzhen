
#include <Common/Serialize/S3ASerializeListener.h>
S3ASerializeListener::~S3ASerializeListener()
{

}


void S3ASerializeListener::SerializeBasicType(bool& val,const char* pszLable)
{
	SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
}


void S3ASerializeListener::SerializeBasicType(short& val,const char* pszLable)
{
	SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
}

void S3ASerializeListener::SerializeBasicType(unsigned char& val, const char* pszLabel)
{
	SerializeByte((xmUint8*)&val,sizeof(val),pszLabel);
}

void S3ASerializeListener::SerializeBasicType(unsigned short& val, const char* pszLable)
{
	SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
}


void S3ASerializeListener::SerializeBasicType(unsigned int&val,const char* pszLable)
{
	SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
}


void S3ASerializeListener::SerializeBasicType(int&val,const char* pszLable)
{
	SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
}


void S3ASerializeListener::SerializeBasicType(unsigned long &val,const char* pszLable)
{
	SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
}


void S3ASerializeListener::SerializeBasicType(long &val,const char* pszLable)
{
	SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
}


void S3ASerializeListener::SerializeBasicType(xmUint64&val,const char* pszLable)
{
	SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
}


void S3ASerializeListener::SerializeBasicType(float& val,const char* pszLable)
{
	SerializeByte((xmUint8*)&val,sizeof(val),pszLable);
}


void S3ASerializeListener::SerializeBasicType(std::string& val,const char* pszLable)
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

void S3ASerializeListener::SerializeBasicType(IS3AStringWrapper* val,const char* pszLable)
{
	xmUint nSize = (xmUint)(val->GetSize()+1);
	SerializeByte(reinterpret_cast<xmUint8*>(&nSize),sizeof(nSize),pszLable);
	val->Resize(nSize,'\0');
	if (nSize > 0)
	{
		xmUint8* pBuf = (xmUint8*)val->GetValue();
		SerializeByte(pBuf,nSize,pszLable);
	}
}

bool S3ASerializeListener::SerializeByte(xmUint8* pData,xmUint nSizeInByte,const char* pszLable)
{
	S3ALog("Derived class has not implement function 'SerializeByte' yet");
	S3ASSERT(false);
	return false;
}

xmUint S3ASerializeListener::Tell()
{
	S3ALog("Derived class has not implement function 'Tell' yet");
	S3ASSERT(false);
	return 0;
}

void S3ASerializeListener::Seek(xmUint nPos)
{
	S3ALog("Derived class has not implement function 'Seek' yet");
	S3ASSERT(false);
}

void S3ASerializeListener::SkipByte(xmUint nSize)
{
	S3ALog("Derived class has not implement function 'SkipByte' yet");
	S3ASSERT(false);
}


void S3ASerializeListener::BeginSection(const char* pszLable)
{
	(void)pszLable;
}

void S3ASerializeListener::EndSection()
{

}

xmUint S3ASerializeListener::GetVersion() const
{
	return m_verStack.back();
}

void S3ASerializeListener::PushVersion(xmUint nVersion)
{
	m_verStack.push_back(nVersion);
}

xmUint S3ASerializeListener::PopVersion()
{
	xmUint nVer = m_verStack.back();
	m_verStack.pop_back();
	return nVer;
}
