#ifndef __CETC1Codec_H__
#define __CETC1Codec_H__

#include "ImageCodec.h"

namespace ma
{

class CETC1Codec : public CImageCodec
{
public:
	CETC1Codec(void);
	~CETC1Codec(void);

protected:
	string mType;

	void flipEndian(void * pData, size_t size, size_t count) const;
	void flipEndian(void * pData, size_t size) const;

public:
	/*/// @copydoc Codec::code
	DataStreamPtr code(MemoryDataStreamPtr& input, CodecDataPtr& pData) const;
	/// @copydoc Codec::codeToFile
	void codeToFile(MemoryDataStreamPtr& input, const string& outFileName, CodecDataPtr& pData) const;
	/// @copydoc Codec::decode
	DecodeResult decode(DataStreamPtr& input) const;*/
	bool decode(const char* pszName, void* pMemory, uint32 nNumBytes, IN OUT ImageData& imgData) const;

	/// @copydoc Codec::magicNumberToFileExt
	string magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const;

	string getType() const;
};

}

#endif