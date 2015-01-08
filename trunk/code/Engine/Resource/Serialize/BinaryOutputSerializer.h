#ifndef __BinaryOutputSerializer_H__
#define __BinaryOutputSerializer_H__

#include "Serializer.h"

namespace ma
{
	class ENGINE_API BinaryOutputSerializer : public BinarySerializer
	{
	
	public:

		BinaryOutputSerializer();

		~BinaryOutputSerializer();

		bool IsReading() const;

		void SerializeByte(uint8* &pData,UINT nSizeToWrite,const char* pszLable);	

		bool Open(const char* pszFilename);

		void Close();
	};

}

#endif// __BinaryOutputSerializer_H__