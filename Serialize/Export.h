#ifndef  _Serialize_Export__H__
#define  _Serialize_Export__H__

#ifdef SERIALIZE_EXPORTS
#define SERIALIZE_API _declspec (dllexport)
#else
#define SERIALIZE_API _declspec (dllimport)
#endif






#endif