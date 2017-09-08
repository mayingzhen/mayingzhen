#ifndef _ImageData_h__
#define _ImageData_h__


namespace ma
{


/** Codec return class for images. Has information about the size and the
            pixel format of the image. */
class CImageCodec
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

#endif