#ifndef __MathSerialize_H__
#define __MathSerialize_H__


COMMON_API void Serialize(SerializeListener& sl,bool &val,const char* pszLable = "bool");

COMMON_API void Serialize(SerializeListener& sl,unsigned char &val,const char* pszLable = "unsigned char");

COMMON_API void Serialize(SerializeListener& sl,unsigned short &val,const char* pszLable = "unsigned short");

COMMON_API void Serialize(SerializeListener& sl,short &val,const char* pszLable = "short");

COMMON_API void Serialize(SerializeListener& sl,unsigned int &val,const char* pszLable = "unsigned in");

COMMON_API void Serialize(SerializeListener& sl,int &val,const char* pszLable = "int");

COMMON_API void Serialize(SerializeListener& sl,unsigned long &val,const char* pszLable = "unsigned in");

COMMON_API void Serialize(SerializeListener& sl,long &val,const char* pszLable = "int");

COMMON_API void Serialize(SerializeListener& sl,xmUint64 &val,const char* pszLable = "bool");

COMMON_API void Serialize(SerializeListener& sl,float &val,const char* pszLable = "float");

COMMON_API void Serialize(SerializeListener& sl,std::string &val,const char* pszLable = "string");

COMMON_API void Serialize(SerializeListener& sl,xmVector2 &val,const char* pszLable = "Vector2");

COMMON_API void Serialize(SerializeListener& sl,GUID &val,const char* pszLable = "GUID");

COMMON_API void Serialize(SerializeListener& sl,xmVector3 &val,const char* pszLable = "Vector3");

COMMON_API void Serialize(SerializeListener& sl,xmVector4 &val,const char* pszLable = "Vector4");

COMMON_API void Serialize(SerializeListener& sl,xmMatrix4x4 &val,const char* pszLable = "Matrix4x4");

COMMON_API void Serialize(SerializeListener& sl,xmNodeTransform& val,const char *pszLable = "NodeTransform");

template<class T>
COMMON_API void Serialize(SerializeListener& sl,T& val,const char* pszLable);

template<class T>
COMMON_API void Serialize(SerializeListener& sl,std::vector<T>& val,const char* pszLable = "array");

#endif// __MathSerialize_H__