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


	class ImageData
	{
	public:
		ImageData();

		void bulkPixelConversion(PixelFormat toFormat);

		uint32 getNumFaces(void) const;

		PixelBox GetPixelBox(uint32 face, uint32 mipmap) const;

		RefPtr<MemoryStream> m_pMemory;
		uint32 m_nHeight;
		uint32 m_nWidth;
		uint32 m_nDepth;
		uint32 m_nSize;

		uint16 m_nNumMipmaps;
		uint32 m_nFlags;

		PixelFormat m_eFormat;
	};

}

