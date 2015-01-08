#ifndef __OgreDDSCodec_H__
#define __OgreDDSCodec_H__

#include "ImageCodec.h"

namespace ma
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Image
	*  @{
	*/

	// Forward declarations
	struct DXTColourBlock;
	struct DXTExplicitAlphaBlock;
	struct DXTInterpolatedAlphaBlock;

    /** Codec specialized in loading DDS (Direct Draw Surface) images.
	@remarks
		We implement our own codec here since we need to be able to keep DXT
		data compressed if the card supports it.
    */
    class ENGINE_API CDDSCodec : public CImageCodec
    {
    private:
    	void flipEndian(void * pData, uint32 size, uint32 count) const;
	    void flipEndian(void * pData, uint32 size) const;

		PixelFormat convertFourCCFormat(uint32 fourcc) const;
		PixelFormat convertPixelFormat(uint32 rgbBits, uint32 rMask, 
			uint32 gMask, uint32 bMask, uint32 aMask) const;

		/// Unpack DXT colours into array of 16 colour values
		void unpackDXTColour(PixelFormat pf, const DXTColourBlock& block, ColourValue* pCol) const;
		/// Unpack DXT alphas into array of 16 colour values
		void unpackDXTAlpha(const DXTExplicitAlphaBlock& block, ColourValue* pCol) const;
		/// Unpack DXT alphas into array of 16 colour values
		void unpackDXTAlpha(const DXTInterpolatedAlphaBlock& block, ColourValue* pCol) const;

		/// Single registered codec instance
		static CDDSCodec* msInstance;
	public:
        CDDSCodec();
        virtual ~CDDSCodec() { }

        /*/// @copydoc Codec::code
        DataStreamPtr code(MemoryDataStreamPtr& input, CodecDataPtr& pData) const;
        /// @copydoc Codec::codeToFile
        void codeToFile(MemoryDataStreamPtr& input, const string& outFileName, CodecDataPtr& pData) const;
        /// @copydoc Codec::decode
        DecodeResult decode(DataStreamPtr& input) const;*/

		bool decode(const char* pszName, void* pMemory, uint32 nNumBytes, IN OUT ImageData& imageData) const;

		/// @copydoc Codec::magicNumberToFileExt
		string magicNumberToFileExt(const char *magicNumberPtr, uint32 maxbytes) const;       
    };
	/** @} */
	/** @} */

} // namespace

#endif

