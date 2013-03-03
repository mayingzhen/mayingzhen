#ifndef __StringConverter_H__
#define __StringConverter_H__

class StringConverter
{
	enum { STRING_BUFFER_SIZE = 1024};
public:
	static std::string ToString(bool val);

	static std::string ToString(unsigned char val);

	static std::string ToString(short val);

	static std::string ToString(unsigned short val);

	static std::string ToString(int val);

	static std::string ToString(unsigned int val);

	static std::string ToString(long val);

	static std::string ToString(unsigned long val);

	static std::string ToString(xmUint64 val);

	static std::string ToString(float val);

	static std::string ToString(maGUID val);

	static void ToValue(bool& val,const char* pszVal);

	static void ToValue(unsigned char& val,const char* pszVal);

	static void ToValue(short& val,const char* pszVal);

	static void ToValue(unsigned short& val,const char* pszVal);

	static void ToValue(int& val,const char* pszVal);

	static void ToValue(unsigned int& val,const char* pszVal);

	static void ToValue(long& val,const char* pszVal);
	
	static void ToValue(unsigned long& val,const char* pszVal);
	
	static void ToValue(xmUint64& val,const char* pszVal);

	static void ToValue(float& val,const char* pszVal);




};

inline std::string StringConverter::ToString(bool val)
{
	return val ? "true" : "false";
}

inline std::string StringConverter::ToString(unsigned char val)
{
	int iVal = val;
	char buf[STRING_BUFFER_SIZE];
	sprintf(&buf[0],"%d",iVal);
	return buf;
}

inline  std::string StringConverter::ToString(short val)
{
	int iVal = val;
	char buf[STRING_BUFFER_SIZE];
	sprintf(&buf[0],"%d",iVal);
	return buf;
}

inline  std::string StringConverter::ToString(unsigned short val)
{
	unsigned int iVal = val;
	char buf[STRING_BUFFER_SIZE];
	sprintf(&buf[0],"%u",iVal);
	return buf;
}

inline std::string StringConverter::ToString(unsigned int val)
{
	char buf[STRING_BUFFER_SIZE];
	sprintf(&buf[0],"%u",val);
	return buf;
}

inline std::string StringConverter::ToString(int val)
{
	char buf[STRING_BUFFER_SIZE];
	sprintf(&buf[0],"%d",val);
	return buf;
}


inline std::string StringConverter::ToString(unsigned long val)
{
	char buf[STRING_BUFFER_SIZE];
	sprintf(&buf[0],"%lu",val);
	return buf;
}

inline std::string StringConverter::ToString(long val)
{
	char buf[STRING_BUFFER_SIZE];
	sprintf(&buf[0],"%ld",val);
	return buf;
}


inline std::string StringConverter::ToString(xmUint64 val)
{
	char buf[STRING_BUFFER_SIZE];
	sprintf(&buf[0],"%llu",val);
	return buf;
}

inline std::string StringConverter::ToString(float val)
{
	char buf[STRING_BUFFER_SIZE];
	sprintf(&buf[0],"%f",val);
	return buf;
}




inline void StringConverter::ToValue(bool& val,const char* pszVal)
{
	if (_stricmp("true",pszVal) == 0)
	{
		val = true;
	}else{
		val = false;
	}
}

inline  void StringConverter::ToValue(unsigned char& val,const char* pszVal)
{
	val = (unsigned char)atoi(pszVal);
}

inline void StringConverter::ToValue(short& val,const char* pszVal)
{
	val = (short)atoi(pszVal);
}

inline void StringConverter::ToValue(unsigned short& val,const char* pszVal)
{
	unsigned int iVal;
	sscanf(pszVal,"%u",&iVal);
	val = (unsigned short)iVal;
}


inline void StringConverter::ToValue(int& val,const char* pszVal)
{
	val = atoi(pszVal);
}

inline void StringConverter::ToValue(unsigned int& val,const char* pszVal)
{
	sscanf(pszVal,"%u",&val);
}

inline void StringConverter::ToValue(long& val,const char* pszVal)
{
	val = atoi(pszVal);
}

inline void StringConverter::ToValue(unsigned long& val,const char* pszVal)
{
	sscanf(pszVal,"%u",&val);
}

inline void StringConverter::ToValue(xmUint64& val,const char* pszVal)
{
	sscanf(pszVal,"%llu",&val);
}


inline void StringConverter::ToValue(float& val,const char* pszVal)
{
	val = (float)atof(pszVal);
}

std::string StringConverter::ToString(maGUID val)
{
	char buf[256];
	sprintf(buf,"(0x%lld,0x%lld)",val.m_a,val.m_b);
	return buf;
}


#endif// __StringConverter_H__