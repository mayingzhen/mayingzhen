#ifndef __BinaryOutputArchive_H__
#define __BinaryOutputArchive_H__

#include "Serializer.h"

namespace ma
{

	class ENGINE_API BinaryOutputArchive : public Serializer
	{
	
	public:

		BinaryOutputArchive();

		~BinaryOutputArchive();

		bool IsReading() const;

		bool SerializeByte(Uint8* &pData,UINT nSizeToRead,const char* pszLable = "Bytes");	

		bool Open(const char* pszFilename);

		void Close();

	private:
		std::ofstream m_file;

	};

}

#endif// __BinaryOutputArchive_H__