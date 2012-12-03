#ifndef __BinaryInputArchive_H__
#define __BinaryInputArchive_H__

#include <Serialize/Serialize/SerializeListener.h>


class BinaryInputArchive : public SerializeListener
{
	std::ifstream m_file;
public:

	BinaryInputArchive();

	~BinaryInputArchive();

	
	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	bool IsReading() const;

	xmUint Tell();

	void Seek(xmUint nPos);

	bool SerializeByte(xmUint8* pData,xmUint nSizeToRead,const char* pszLable = "Bytes");

	void SkipByte(xmUint nSizeToSkip);

	bool Open(const char* pszFilename);

	void Close();

	

};

class BinaryMemoryInputArchive : public SerializeListener
{
	const xmUint8* m_pData;
	xmUint32 m_nDataSize;
	xmUint32 m_nCurPos;

public:

	BinaryMemoryInputArchive();

	~BinaryMemoryInputArchive();

	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	bool IsReading()const;

	xmUint Tell();

	void Seek(xmUint nPos);

	bool SerializeByte(xmUint8* pData,xmUint nSizeToRead,const char* pszLable = "Bytes");

	void SkipByte(xmUint nSizeToSkip);

	bool Open(const xmUint8* pData,xmUint32 nSizeInByte);

	void Close();


};



#endif// __BinaryInputArchive_H__