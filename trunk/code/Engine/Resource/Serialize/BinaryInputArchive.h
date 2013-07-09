#ifndef __BinaryInputArchive_H__
#define __BinaryInputArchive_H__

#include "Engine/Resource/Serialize/Serializer.h"

namespace ma
{

class ENGINE_API BinaryInputArchive : public Serializer
{
	std::ifstream m_file;
public:

	BinaryInputArchive();

	~BinaryInputArchive();

	
	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	bool IsReading() const;

	UINT Tell();

	void Seek(UINT nPos);

	bool SerializeByte(Uint8* pData,UINT nSizeToRead,const char* pszLable = "Bytes");

	void SkipByte(UINT nSizeToSkip);

	bool Open(const char* pszFilename);

	void Close();

	

};

class BinaryMemoryInputArchive : public Serializer
{
	const Uint8* m_pData;
	Uint32 m_nDataSize;
	Uint32 m_nCurPos;

public:

	BinaryMemoryInputArchive();

	~BinaryMemoryInputArchive();

	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	bool IsReading()const;

	UINT Tell();

	void Seek(UINT nPos);

	bool SerializeByte(Uint8* pData,UINT nSizeToRead,const char* pszLable = "Bytes");

	void SkipByte(UINT nSizeToSkip);

	bool Open(const Uint8* pData,Uint32 nSizeInByte);

	void Close();


};

}

#endif// __BinaryInputArchive_H__