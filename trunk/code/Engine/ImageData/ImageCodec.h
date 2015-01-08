#pragma once
#include "PixelFormat.h"


namespace ma
{

enum ImageFlags
{
	IF_COMPRESSED = 0x00000001,
	IF_CUBEMAP    = 0x00000002,
	IF_3D_TEXTURE = 0x00000004
};

/** Codec return class for images. Has information about the size and the
            pixel format of the image. */
class  ImageData
{
public:
	ImageData():
		height(0), width(0), depth(1), size(0),
		num_mipmaps(0), flags(0), format(PF_UNKNOWN)
	{
	}
	RefPtr<MemoryStream> memory;
    uint32 height;
    uint32 width;
	uint32 depth;
    uint32 size;
    
    uint16 num_mipmaps;
    uint32 flags;

    PixelFormat format;

	uint32 getNumFaces(void) const
	{
		if ( flags &IF_CUBEMAP )
			return 6;
		return 1;
	}

	PixelBox GetPixelBox( uint32 face, uint32 mipmap ) const
	{
		// Image data is arranged as:
		// face 0, top level (mip 0)
		// face 0, mip 1
		// face 0, mip 2
		// face 1, top level (mip 0)
		// face 1, mip 1
		// face 1, mip 2
		// etc
		if(mipmap > num_mipmaps)
			LogError("Mipmap index out of range, Image::getPixelBox" ) ;
		if(face >= getNumFaces())
			LogError("Face index out of range, Image::getPixelBox");
		// Calculate mipmap offset and size
		uint8 *offset = const_cast<uint8*>(memory->GetPtr());
		// Base offset is number of full faces
		uint32 nwidth = width, nheight= height, ndepth=depth;
		uint32 numMips = num_mipmaps;

		// Figure out the offsets 
		uint32 fullFaceSize = 0;
		uint32 finalFaceSize = 0;
		uint32 finalWidth = 0, finalHeight = 0, finalDepth = 0;
		for( uint32 mip=0; mip <= numMips; ++mip )
		{
			if (mip == mipmap)
			{
				finalFaceSize = fullFaceSize;
				finalWidth = nwidth;
				finalHeight = nheight;
				finalDepth = ndepth;
			}
			fullFaceSize += PixelUtil::getMemorySize(nwidth, nheight, ndepth, format);

			/// Half size in each dimension
			if(nwidth!=1) nwidth /= 2;
			if(nheight!=1) nheight /= 2;
			if(ndepth!=1) ndepth /= 2;
		}
		// Advance pointer by number of full faces, plus mip offset into
		offset += face * fullFaceSize;
		offset += finalFaceSize;
		// Return subface as pixelbox
		PixelBox src(finalWidth, finalHeight, finalDepth, format, offset);
		return src;
	}
};

class ENGINE_API CImageCodec
{
public:
	CImageCodec(void);
	~CImageCodec(void);

	bool Load(const char* pszFile);

	// ---------------------------------------------------------------------
	// Static Function
	// ---------------------------------------------------------------------
public:
	/// Static method to startup FreeImage and register the FreeImage codecs
	static void Startup();

	/// Static method to shutdown FreeImage and unregister the FreeImage codecs
	static void Shutdown();

	bool decode(const char* pszName, void* pMemory, uint32 nNumBytes, IN OUT ImageData& imageData) const;

	enum Filter
	{
		FILTER_NEAREST,
		FILTER_LINEAR,
		FILTER_BILINEAR,
		FILTER_BOX,
		FILTER_TRIANGLE,
		FILTER_BICUBIC
	};
	/** Scale a 1D, 2D or 3D image volume. 
		@param 	src			PixelBox containing the source pointer, dimensions and format
		@param 	dst			PixelBox containing the destination pointer, dimensions and format
		@param 	filter		Which filter to use
		@remarks 	This function can do pixel format conversion in the process.
		@note	dst and src can point to the same PixelBox object without any problem
	*/
	static void scale(const PixelBox &src, const PixelBox &dst, Filter filter = FILTER_BILINEAR);

	// Static function to calculate size in bytes from the number of mipmaps, faces and the dimensions
	static uint32 calculateSize(uint32 mipmaps, uint32 faces, uint32 width, uint32 height, uint32 depth, PixelFormat format);
	
	/** Does gamma adjustment.
        @note
            Basic algo taken from Titan Engine, copyright (c) 2000 Ignacio 
            Castano Iguado
    */
	static void applyGamma( unsigned char *buffer, float gamma, uint32 size, uint8 bpp );
private:
	uint32 m_nWidth, m_nHeight;
};

}