#ifndef __BinaryInputArchive_H__
#define __BinaryInputArchive_H__

#include "Serializer.h"

namespace ma
{

	class ENGINE_API BinaryInputArchive : public Serializer
	{
		
	public:

		BinaryInputArchive();

		~BinaryInputArchive();

		bool IsReading() const;

		bool SerializeByte(Uint8* &pData,UINT nSizeToRead,const char* pszLable = "Bytes");

		bool Open(const char* pszFilename);

		bool Open(Stream* pStream);

		void Close();

	private:
		StreamPtr			m_pStream;
	};
}

#endif// __BinaryInputArchive_H__