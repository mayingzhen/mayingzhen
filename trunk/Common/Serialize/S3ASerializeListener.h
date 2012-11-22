#ifndef __S3ASerializeListener_H__
#define __S3ASerializeListener_H__

#include <Common/S3AArchive.h>

//#include <Animation/Common/xmCommonPrefix.h>

class COMMON_API S3ASerializeListener : public IS3ASerializeListener
{
	std::deque<xmUint> m_verStack;

public:

	virtual ~S3ASerializeListener();

	virtual void SerializeBasicType(bool& val,const char* pszLable = "bool");

	virtual void SerializeBasicType(unsigned char& val, const char* pszLabel = "unsigned char");

	virtual void SerializeBasicType(short& val,const char* pszLable = "short");	

	virtual void SerializeBasicType(unsigned short& val, const char* pszLabel = "unsigned short");

	virtual void SerializeBasicType(unsigned int&val,const char* pszLable = "unsigned int");

	virtual void SerializeBasicType(int&val,const char* pszLable = "int");

	virtual void SerializeBasicType(unsigned long &val,const char* pszLable = "unsigned long");

	virtual void SerializeBasicType(long &val,const char* pszLable = "long");

	virtual void SerializeBasicType(xmUint64&val,const char* pszLable = "xmUint64");

	virtual void SerializeBasicType(float& val,const char* pszLable = "float");

	virtual void SerializeBasicType(std::string& val,const char* pszLable = "string");

	virtual void SerializeBasicType(IS3AStringWrapper* val,const char* pszLable = "string");

	virtual bool SerializeByte(xmUint8* pData,xmUint nSizeInByte,const char* pszLable = "Bytes");

	

	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	virtual xmUint Tell();

	virtual void Seek(xmUint nPos);

	virtual void SkipByte(xmUint nSize);
	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	virtual bool IsReading() const = 0;

	virtual void BeginSection(const char* pszLable);

	virtual void EndSection();

	//Version for current module
	virtual xmUint GetVersion() const;

	virtual void PushVersion(xmUint nVersion);

	virtual xmUint PopVersion();

	template<class T>
	void Serialize(T& val,const char* pszLable);

};


template<class T>
void S3ASerializeListener::Serialize(T& val,const char* pszLable)
{
	S3ASerialize(*this,val,pszLable);
}





//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------


inline void S3ASerialize(IS3ASerializeListener& sl,bool &val,const char* pszLable = "bool")
{
	sl.SerializeBasicType(val,pszLable);
}

inline void S3ASerialize(IS3ASerializeListener& sl,unsigned char &val,const char* pszLable = "unsigned char")
{
	sl.SerializeBasicType(val,pszLable);
}

inline void S3ASerialize(IS3ASerializeListener& sl,unsigned short &val,const char* pszLable = "unsigned short")
{
	sl.SerializeBasicType(val,pszLable);
}

inline void S3ASerialize(IS3ASerializeListener& sl,short &val,const char* pszLable = "short")
{
	sl.SerializeBasicType(val,pszLable);
}

inline void S3ASerialize(IS3ASerializeListener& sl,unsigned int &val,const char* pszLable = "unsigned in")
{
	sl.SerializeBasicType(val,pszLable);
}

inline void S3ASerialize(IS3ASerializeListener& sl,int &val,const char* pszLable = "int")
{
	sl.SerializeBasicType(val,pszLable);
}

inline void S3ASerialize(IS3ASerializeListener& sl,unsigned long &val,const char* pszLable = "unsigned in")
{
	sl.SerializeBasicType(val,pszLable);
}

inline void S3ASerialize(IS3ASerializeListener& sl,long &val,const char* pszLable = "int")
{
	sl.SerializeBasicType(val,pszLable);
}

inline void S3ASerialize(IS3ASerializeListener& sl,xmUint64 &val,const char* pszLable = "bool")
{
	sl.SerializeBasicType(val,pszLable);
}

inline void S3ASerialize(IS3ASerializeListener& sl,float &val,const char* pszLable = "float")
{
	sl.SerializeBasicType(val,pszLable);
}

// inline void S3ASerialize(IS3ASerializeListener& sl,FL &val,const char* pszLable = "float")
// {
// 	sl.SerializeBasicType(val,pszLable);
// }

inline void S3ASerialize(IS3ASerializeListener& sl,std::string &val,const char* pszLable = "string")
{
	sl.SerializeBasicType(val,pszLable);
}

template<class T>
void S3ASerialize(IS3ASerializeListener& sl,T& val,const char* pszLable)
{
	val.Serialize(sl,pszLable);
}

template<class T>
void S3ASerialize(IS3ASerializeListener& sl,std::vector<T>& val,const char* pszLable = "array")
{
	sl.BeginSection(pszLable);
    
	xmUint nSize = (xmUint)val.size();
	S3ASerialize(sl,nSize,"size");
	
	if (nSize != val.size())
	{
		val.resize(nSize);
	}
	sl.BeginSection("element");
    
	for (xmUint nCnt = 0;nCnt < nSize; ++nCnt)
	{
		char buf[32];
		sprintf(&buf[0],"Element_%u",nCnt);
		S3ASerialize(sl,val[nCnt],buf);
	}
	sl.EndSection();
    
	sl.EndSection();
}


// 
// template < class T, class Allocator>
// void S3ASerialize(IS3ASerializeListener& sl,std::vector< T,Allocator >& val,const char* pszLable = "array")
// {
// 	sl.BeginSection(pszLable);
//     
// 	xmUint nSize = val.size();
// 	S3ASerialize(sl,nSize,"size");
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
// 		S3ASerialize(sl,val[nCnt],buf);
// 	}
// 	sl.EndSection();
//     
// 	sl.EndSection();
// }



// template<class T>
// void S3ASerialize(IS3ASerializeListener& sl,std::vector<T>& val,const char* pszLable = "array")
// {
// 	sl.BeginSection(pszLable);
//     
// 	xmUint nSize = val.size();
// 	S3ASerialize(sl,nSize,"size");
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
// 		S3ASerialize(sl,val[nCnt],buf);
// 	}
// 	sl.EndSection();
//     
// 	sl.EndSection();
// }



//#include <Animation/Common/xmCommonSuffix.h>

#endif// __S3ASerializeListener_H__