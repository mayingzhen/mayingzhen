#ifndef __BinaryInputSerializer_H__
#define __BinaryInputSerializer_H__

#include "BinarySerializer.h"

namespace ma
{

	class ENGINE_API BinaryInputSerializer : public BinarySerializer
	{
		
	public:

		BinaryInputSerializer();

		~BinaryInputSerializer();

		bool IsReading() const;

		void SerializeByte(uint8* &pData,UINT nSizeToRead,const char* pszLable);

		bool Open(const char* pszFilename);

		bool Open(Stream* pStream);

		void Close();
	};
}

#endif// __BinaryInputSerializer_H__