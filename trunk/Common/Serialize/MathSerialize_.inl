//#include "Common/Serialize/MathSerialize.h"
#include "Common/Serialize/SerializeListener.h"


inline void Serialize(SerializeListener& sl,bool &val,const char* pszLable)
{
	sl.Serialize(val,pszLable);
}

inline void Serialize(SerializeListener& sl,unsigned char &val,const char* pszLable)
{
	sl.Serialize(val,pszLable);
}

inline void Serialize(SerializeListener& sl,unsigned short &val,const char* pszLable)
{
	sl.Serialize(val,pszLable);
}

inline void Serialize(SerializeListener& sl,short &val,const char* pszLable)
{
	sl.Serialize(val,pszLable);
}

inline void Serialize(SerializeListener& sl,unsigned int &val,const char* pszLable)
{
	sl.Serialize(val,pszLable);
}

inline void Serialize(SerializeListener& sl,int &val,const char* pszLable)
{
	sl.Serialize(val,pszLable);
}

inline void Serialize(SerializeListener& sl,unsigned long &val,const char* pszLable)
{
	sl.Serialize(val,pszLable);
}

inline void Serialize(SerializeListener& sl,long &val,const char* pszLable)
{
	sl.Serialize(val,pszLable);
}

inline void Serialize(SerializeListener& sl,xmUint64 &val,const char* pszLable)
{
	sl.Serialize(val,pszLable);
}

inline void Serialize(SerializeListener& sl,float &val,const char* pszLable)
{
	sl.Serialize(val,pszLable);
}


inline void Serialize(SerializeListener& sl,std::string &val,const char* pszLable)
{
	sl.Serialize(val,pszLable);
}


inline void Serialize(SerializeListener& sl,xmVector2 &val,const char* pszLable)
{
	sl.BeginSection(pszLable);
	sl.Serialize(val.x,"x");
	sl.Serialize(val.y,"y");
	sl.EndSection();
}


inline void Serialize(SerializeListener& sl,GUID &val,const char* pszLable)
{
	sl.BeginSection(pszLable);
	sl.Serialize(val.m_a,"a");
	sl.Serialize(val.m_b,"b");
	sl.EndSection();
}

inline void Serialize(SerializeListener& sl,xmVector3 &val,const char* pszLable)
{
	sl.BeginSection(pszLable);
	sl.Serialize(val.x,"x");
	sl.Serialize(val.y,"y");
	sl.Serialize(val.z,"z");
	sl.EndSection();
}


inline void Serialize(SerializeListener& sl,xmVector4 &val,const char* pszLable)
{
	sl.BeginSection(pszLable);
	sl.Serialize(val.x,"x");
	sl.Serialize(val.y,"y");
	sl.Serialize(val.z,"z");
	sl.Serialize(val.w,"w");
	sl.EndSection();
}


inline void Serialize(SerializeListener& sl,xmQuaternion &val,const char* pszLable)
{
	sl.BeginSection(pszLable);
	sl.Serialize(val.x,"x");
	sl.Serialize(val.y,"y");
	sl.Serialize(val.z,"z");
	sl.Serialize(val.w,"w");
	sl.EndSection();
}


inline void Serialize(SerializeListener& sl,xmMatrix4x4 &val,const char* pszLable)
{
	sl.BeginSection(pszLable);
	Serialize(sl,*reinterpret_cast<xmVector4*>(&val._11),"X");
	Serialize(sl,*reinterpret_cast<xmVector4*>(&val._21),"Y");
	Serialize(sl,*reinterpret_cast<xmVector4*>(&val._31),"Z");
	Serialize(sl,*reinterpret_cast<xmVector4*>(&val._41),"T");
	sl.EndSection();
}

inline void Serialize(SerializeListener& sl,xmNodeTransform& val,const char *pszLable)
{
	sl.BeginSection(pszLable);
	//Serialize(sl,val.m_vLocalScale,"NonPropScale");
	sl.Serialize(val.m_fScale,"PropScale");
	Serialize(sl,val.m_qRot,"Rotation");
	Serialize(sl,val.m_vPos,"Position");
	sl.EndSection();
}

template<class T>
inline void Serialize(SerializeListener& sl,T& val,const char* pszLable)
{
	val.Serialize(sl,pszLable);
}

template<class T>
inline void Serialize(SerializeListener& sl,std::vector<T>& val,const char* pszLable)
{
	sl.BeginSection(pszLable);

	xmUint nSize = (xmUint)val.size();
	Serialize(sl,nSize,"size");

	if (nSize != val.size())
	{
		val.resize(nSize);
	}
	sl.BeginSection("element");

	for (xmUint nCnt = 0;nCnt < nSize; ++nCnt)
	{
		char buf[32];
		sprintf(&buf[0],"Element_%u",nCnt);
		Serialize(sl,val[nCnt],buf);
	}
	sl.EndSection();

	sl.EndSection();
}
