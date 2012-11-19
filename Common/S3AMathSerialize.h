//----------------------------------------------------------------------
// Copyright(c) TQ Digital Entertainment, All Rights Reserved
// Author:  Xu Kaijie
// Created: 2010/3/3
// Describe:S3 Animation
//----------------------------------------------------------------------

#ifndef __S3AMathSerialize_H__
#define __S3AMathSerialize_H__

#include "Common/S3AArchive.h"

void S3ASerialize(IS3ASerializeListener& sl,S3AGUID &val,const char* pszLable = "GUID");

inline void S3ASerialize(IS3ASerializeListener& sl,xmVector2 &val,const char* pszLable = "Vector2")
{
	sl.BeginSection(pszLable);
	S3ASerialize(sl,val.x,"x");
	S3ASerialize(sl,val.y,"y");
	sl.EndSection();
}

void S3ASerialize(IS3ASerializeListener& sl,xmVector3 &val,const char* pszLable = "Vector3");

void S3ASerialize(IS3ASerializeListener& sl,xmVector4 &val,const char* pszLable = "Vector4");

void S3ASerialize(IS3ASerializeListener& sl,xmQuaternion &val,const char* pszLable = "Quaternion");

void S3ASerialize(IS3ASerializeListener& sl,xmMatrix4x4 &val,const char* pszLable = "Matrix");



inline void S3ASerialize(IS3ASerializeListener& sl,S3AGUID &val,const char* pszLable)
{
	sl.BeginSection(pszLable);
	S3ASerialize(sl,val.m_a,"a");
	S3ASerialize(sl,val.m_b,"b");
	sl.EndSection();
}

inline void S3ASerialize(IS3ASerializeListener& sl,xmVector3 &val,const char* pszLable)
{
	sl.BeginSection(pszLable);
	S3ASerialize(sl,val.x,"x");
	S3ASerialize(sl,val.y,"y");
	S3ASerialize(sl,val.z,"z");
	sl.EndSection();
}


inline void S3ASerialize(IS3ASerializeListener& sl,xmVector4 &val,const char* pszLable)
{
	sl.BeginSection(pszLable);
	S3ASerialize(sl,val.x,"x");
	S3ASerialize(sl,val.y,"y");
	S3ASerialize(sl,val.z,"z");
	S3ASerialize(sl,val.w,"w");
	sl.EndSection();
}


inline void S3ASerialize(IS3ASerializeListener& sl,xmQuaternion &val,const char* pszLable)
{
	sl.BeginSection(pszLable);
	S3ASerialize(sl,val.x,"x");
	S3ASerialize(sl,val.y,"y");
	S3ASerialize(sl,val.z,"z");
	S3ASerialize(sl,val.w,"w");
	sl.EndSection();
}


inline void S3ASerialize(IS3ASerializeListener& sl,xmMatrix4x4 &val,const char* pszLable)
{
	sl.BeginSection(pszLable);
	S3ASerialize(sl,*reinterpret_cast<xmVector4*>(&val._11),"X");
	S3ASerialize(sl,*reinterpret_cast<xmVector4*>(&val._21),"Y");
	S3ASerialize(sl,*reinterpret_cast<xmVector4*>(&val._31),"Z");
	S3ASerialize(sl,*reinterpret_cast<xmVector4*>(&val._41),"T");
	sl.EndSection();
}

inline void S3ASerialize(IS3ASerializeListener& sl,xmNodeTransform& val,const char *pszLable)
{
	sl.BeginSection(pszLable);
	//S3ASerialize(sl,val.m_vLocalScale,"NonPropScale");
	S3ASerialize(sl,val.m_fScale,"PropScale");
	S3ASerialize(sl,val.m_qRot,"Rotation");
	S3ASerialize(sl,val.m_vPos,"Position");
	sl.EndSection();
}


#endif// __S3AMathSerialize_H__