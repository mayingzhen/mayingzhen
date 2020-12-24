#include "ImageData.h"
#include "ImageCodec.h"

namespace ma
{
	ImageData::ImageData():
		m_nHeight(0), m_nWidth(0), m_nDepth(1), m_nSize(0),
		m_nNumMipmaps(0), m_nFlags(0), m_eFormat(PF_UNKNOWN)
	{
	}

	void ImageData::bulkPixelConversion(PixelFormat toFormat)
	{
        uint32_t nNumFace = getNumFaces();
        uint32_t nMemSize = CImageCodec::calculateSize(m_nNumMipmaps, nNumFace, m_nWidth,
                                                     m_nHeight, m_nDepth, toFormat);
		RefPtr<MemoryStream> toMemory = CreateMemoryStream(nMemSize, false);
		uint32_t nMemOffset = 0;

		for (uint32_t iFace = 0; iFace < nNumFace; ++iFace)
		{
			for (uint32_t mip = 0; mip <= m_nNumMipmaps; ++mip)
			{
				PixelBox src = GetPixelBox(iFace, mip);
				PixelBox dst = PixelBox(src.getWidth(), src.getHeight(), src.getDepth(), toFormat, toMemory->GetPtr() + nMemOffset);
				PixelUtil::bulkPixelConversion(src, dst);
				nMemOffset += PixelUtil::getMemorySize(src.getWidth(), src.getHeight(), src.getDepth(), toFormat);
			}
		}

		m_pMemory = toMemory;
		m_eFormat = toFormat;
		m_nSize = m_pMemory->GetSize();
	}

	uint32_t ImageData::getNumFaces(void) const
	{
		if ( m_nFlags &IF_CUBEMAP )
			return 6;
		return 1;
	}

	PixelBox ImageData::GetPixelBox( uint32_t face, uint32_t mipmap ) const
	{
		// Image data is arranged as:
		// face 0, top level (mip 0)
		// face 0, mip 1
		// face 0, mip 2
		// face 1, top level (mip 0)
		// face 1, mip 1
		// face 1, mip 2
		// etc
		if(mipmap > m_nNumMipmaps)
			LogError("Mipmap index out of range, Image::getPixelBox" ) ;
		if(face >= getNumFaces())
			LogError("Face index out of range, Image::getPixelBox");
		// Calculate mipmap offset and size
		uint8_t *offset = const_cast<uint8_t*>(m_pMemory->GetPtr());
		// Base offset is number of full faces
		uint32_t nwidth = m_nWidth, nheight= m_nHeight, ndepth=m_nDepth;
		uint32_t numMips = m_nNumMipmaps;

		// Figure out the offsets 
		uint32_t fullFaceSize = 0;
		uint32_t finalFaceSize = 0;
		uint32_t finalWidth = 0, finalHeight = 0, finalDepth = 0;
		for( uint32_t mip=0; mip <= numMips; ++mip )
		{
			if (mip == mipmap)
			{
				finalFaceSize = fullFaceSize;
				finalWidth = nwidth;
				finalHeight = nheight;
				finalDepth = ndepth;
			}
			fullFaceSize += PixelUtil::getMemorySize(nwidth, nheight, ndepth, m_eFormat);

			/// Half size in each dimension
			if(nwidth!=1) nwidth /= 2;
			if(nheight!=1) nheight /= 2;
			if(ndepth!=1) ndepth /= 2;
		}
		// Advance pointer by number of full faces, plus mip offset into
		offset += face * fullFaceSize;
		offset += finalFaceSize;
		// Return subface as pixelbox
		PixelBox src(finalWidth, finalHeight, finalDepth, m_eFormat, offset);
		return src;
	}

	RefPtr<ImageData> CreateImageData()
	{
		return new ImageData();
	}
};
