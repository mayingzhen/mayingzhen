#pragma once

#include "PixelFormat.h"

namespace ma
{
	enum ImageFlags
	{
		IF_COMPRESSED = 0x00000001,
		IF_CUBEMAP = 0x00000002,
		IF_3D_TEXTURE = 0x00000004
	};


	class ImageData : public Referenced
	{
	public:
		ImageData();

		void bulkPixelConversion(PixelFormat toFormat);

		uint32_t getNumFaces(void) const;

		PixelBox GetPixelBox(uint32_t face, uint32_t mipmap) const;

		RefPtr<MemoryStream> m_pMemory;
		uint32_t m_nHeight;
		uint32_t m_nWidth;
		uint32_t m_nDepth;
		uint32_t m_nSize;

		uint16_t m_nNumMipmaps;
		uint32_t m_nFlags;

		PixelFormat m_eFormat;
	};

	RefPtr<ImageData> CreateImageData();

}

