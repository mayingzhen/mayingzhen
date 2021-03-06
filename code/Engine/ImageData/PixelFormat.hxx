#include "PixelFormat.h"
#include "PixelConversions.h"

namespace ma
{

    //-----------------------------------------------------------------------
    /** Pixel format database */
    PixelFormatDescription _pixelFormats[PF_COUNT] = {
	//-----------------------------------------------------------------------
        {"PF_UNKNOWN",
        /* Bytes per element */
        0,
        /* Flags */
        0,
        /* Component type and count */
        PCT_BYTE, 0,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_L8",
        /* Bytes per element */
        1,
        /* Flags */
        PFF_LUMINANCE | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 1,
        /* rbits, gbits, bbits, abits */
        8, 0, 0, 0,
        /* Masks and shifts */
        0xFF, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_L16",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_LUMINANCE | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SHORT, 1,
        /* rbits, gbits, bbits, abits */
        16, 0, 0, 0,
        /* Masks and shifts */
        0xFFFF, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_A8",
        /* Bytes per element */
        1,
        /* Flags */
        PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 1,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 8,
        /* Masks and shifts */
        0, 0, 0, 0xFF, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_A4L4",
        /* Bytes per element */
        1,
        /* Flags */
        PFF_HASALPHA | PFF_LUMINANCE | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 2,
        /* rbits, gbits, bbits, abits */
        4, 0, 0, 4,
        /* Masks and shifts */
        0x0F, 0, 0, 0xF0, 0, 0, 0, 4
        },
	//-----------------------------------------------------------------------
        {"PF_BYTE_LA",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_HASALPHA | PFF_LUMINANCE,
        /* Component type and count */
        PCT_BYTE, 2,
        /* rbits, gbits, bbits, abits */
        8, 0, 0, 8,
        /* Masks and shifts */
        //    0, 0, 0, 0, //Deprecated. See PF_RG8
        //    0, 0, 0, 0
        0x000000FF, 0, 0, 0x0000FF00,
        0, 0, 0, 8
        },
	//-----------------------------------------------------------------------
        {"PF_R5G6B5",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        5, 6, 5, 0,
        /* Masks and shifts */
        0xF800, 0x07E0, 0x001F, 0,
        11, 5, 0, 0
        },
	//-----------------------------------------------------------------------
		{"PF_B5G6R5",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        5, 6, 5, 0,
        /* Masks and shifts */
        0x001F, 0x07E0, 0xF800, 0,
        0, 5, 11, 0
        },
	//-----------------------------------------------------------------------
        {"PF_A4R4G4B4",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        4, 4, 4, 4,
        /* Masks and shifts */
        0x0F00, 0x00F0, 0x000F, 0xF000,
        8, 4, 0, 12
        },
	//-----------------------------------------------------------------------
        {"PF_A1R5G5B5",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        5, 5, 5, 1,
        /* Masks and shifts */
        0x7C00, 0x03E0, 0x001F, 0x8000,
        10, 5, 0, 15,
        },
	//-----------------------------------------------------------------------
        {"PF_R8G8B8",
        /* Bytes per element */
        3,  // 24 bit integer -- special
        /* Flags */
        PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 0,
        /* Masks and shifts */
        0xFF0000, 0x00FF00, 0x0000FF, 0,
        16, 8, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_B8G8R8",
        /* Bytes per element */
        3,  // 24 bit integer -- special
        /* Flags */
        PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 0,
        /* Masks and shifts */
        0x0000FF, 0x00FF00, 0xFF0000, 0,
        0, 8, 16, 0
        },
	//-----------------------------------------------------------------------
        {"PF_A8R8G8B8",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 8,
        /* Masks and shifts */
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000,
        16, 8, 0, 24
        },
	//-----------------------------------------------------------------------
        {"PF_A8B8G8R8",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 8,
        /* Masks and shifts */
        0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000,
        0, 8, 16, 24,
        },
	//-----------------------------------------------------------------------
        {"PF_B8G8R8A8",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 8,
        /* Masks and shifts */
        0x0000FF00, 0x00FF0000, 0xFF000000, 0x000000FF,
        8, 16, 24, 0
        },
	//-----------------------------------------------------------------------
        {"PF_A2R10G10B10",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        10, 10, 10, 2,
        /* Masks and shifts */
        0x3FF00000, 0x000FFC00, 0x000003FF, 0xC0000000,
        20, 10, 0, 30
        },
	//-----------------------------------------------------------------------
        {"PF_A2B10G10R10",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        10, 10, 10, 2,
        /* Masks and shifts */
        0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000,
        0, 10, 20, 30
        },
	//-----------------------------------------------------------------------
        {"PF_DXT1",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 3, // No alpha
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_DXT2",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_DXT3",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_DXT4",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_DXT5",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_FLOAT16_RGB",
        /* Bytes per element */
        6,
        /* Flags */
        PFF_FLOAT,
        /* Component type and count */
        PCT_FLOAT16, 3,
        /* rbits, gbits, bbits, abits */
        16, 16, 16, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_FLOAT16_RGBA",
        /* Bytes per element */
        8,
        /* Flags */
        PFF_FLOAT | PFF_HASALPHA,
        /* Component type and count */
        PCT_FLOAT16, 4,
        /* rbits, gbits, bbits, abits */
        16, 16, 16, 16,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_FLOAT32_RGB",
        /* Bytes per element */
        12,
        /* Flags */
        PFF_FLOAT,
        /* Component type and count */
        PCT_FLOAT32, 3,
        /* rbits, gbits, bbits, abits */
        32, 32, 32, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_FLOAT32_RGBA",
        /* Bytes per element */
        16,
        /* Flags */
        PFF_FLOAT | PFF_HASALPHA,
        /* Component type and count */
        PCT_FLOAT32, 4,
        /* rbits, gbits, bbits, abits */
        32, 32, 32, 32,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_X8R8G8B8",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 0,
        /* Masks and shifts */
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000,
        16, 8, 0, 24
        },
	//-----------------------------------------------------------------------
        {"PF_X8B8G8R8",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 0,
        /* Masks and shifts */
        0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000,
        0, 8, 16, 24
        },
	//-----------------------------------------------------------------------
        {"PF_R8G8B8A8",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 8,
        /* Masks and shifts */
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF,
        24, 16, 8, 0
        },
	//-----------------------------------------------------------------------
		{"PF_DEPTH",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_DEPTH,
        /* Component type and count */
            PCT_FLOAT16, 1, // ?
        /* rbits, gbits, bbits, abits */
            16, 0, 0, 0,
        /* Masks and shifts */
		0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
		{"PF_SHORT_RGBA",
		/* Bytes per element */
        8,
        /* Flags */
        PFF_HASALPHA,
        /* Component type and count */
        PCT_SHORT, 4,
        /* rbits, gbits, bbits, abits */
        16, 16, 16, 16,
        /* Masks and shifts */
		0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_R3G3B2",
        /* Bytes per element */
        1,
        /* Flags */
        PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        3, 3, 2, 0,
        /* Masks and shifts */
        0xE0, 0x1C, 0x03, 0,
        5, 2, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_FLOAT16_R",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_FLOAT,
        /* Component type and count */
        PCT_FLOAT16, 1,
        /* rbits, gbits, bbits, abits */
        16, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_FLOAT32_R",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_FLOAT,
        /* Component type and count */
        PCT_FLOAT32, 1,
        /* rbits, gbits, bbits, abits */
        32, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_SHORT_GR",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SHORT, 2,
        /* rbits, gbits, bbits, abits */
        16, 16, 0, 0,
        /* Masks and shifts */
        0x0000FFFF, 0xFFFF0000, 0, 0, 
		0, 16, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_FLOAT16_GR",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_FLOAT,
        /* Component type and count */
        PCT_FLOAT16, 2,
        /* rbits, gbits, bbits, abits */
        16, 16, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
        {"PF_FLOAT32_GR",
        /* Bytes per element */
        8,
        /* Flags */
        PFF_FLOAT,
        /* Component type and count */
        PCT_FLOAT32, 2,
        /* rbits, gbits, bbits, abits */
        32, 32, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
		{"PF_SHORT_RGB",
		/* Bytes per element */
        6,
        /* Flags */
        0,
        /* Component type and count */
        PCT_SHORT, 3,
        /* rbits, gbits, bbits, abits */
        16, 16, 16, 0,
        /* Masks and shifts */
		0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
		{"PF_PVRTC_RGB2",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
		{"PF_PVRTC_RGBA2",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
		{"PF_PVRTC_RGB4",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
		{"PF_PVRTC_RGBA4",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_PVRTC2_2BPP",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_PVRTC2_4BPP",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R11G11B10_FLOAT",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_FLOAT | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_FLOAT32, 1,
        /* rbits, gbits, bbits, abits */
        11, 11, 10, 0,
        /* Masks and shifts */
        0xFFC00000, 0x03FF800, 0x000007FF, 0,
        24, 16, 8, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8_UINT",
        /* Bytes per element */
        1,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 1,
        /* rbits, gbits, bbits, abits */
        8, 0, 0, 0,
        /* Masks and shifts */
        0xFF, 0, 0, 0,
        0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8G8_UINT",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 2,
        /* rbits, gbits, bbits, abits */
        8, 8, 0, 0,
        /* Masks and shifts */
            0x000000FF, 0x0000FF00, 0, 0,
            0, 8, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8G8B8_UINT",
        /* Bytes per element */
        3,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 3,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 0,
        /* Masks and shifts */
        0xFF0000, 0x00FF00, 0x0000FF, 0,
        16, 8, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8G8B8A8_UINT",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_INTEGER | PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 4,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 8,
        /* Masks and shifts */
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF,
        24, 16, 8, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16_UINT",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 1,
        /* rbits, gbits, bbits, abits */
        16, 0, 0, 0,
        /* Masks and shifts */
        0xFFFF, 0, 0, 0,
        0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16G16_UINT",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 2,
        /* rbits, gbits, bbits, abits */
        16, 16, 0, 0,
        /* Masks and shifts */
        0xFFFF0000, 0x0000FFFF, 0, 0,
        16, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16G16B16_UINT",
        /* Bytes per element */
        6,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 3,
        /* rbits, gbits, bbits, abits */
        16, 16, 16, 0,
        /* Masks and shifts */
        0xFFFF00000000, 0x0000FFFF0000, 0x00000000FFFF, 0,
        32, 16, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16G16B16A16_UINT",
        /* Bytes per element */
        8,
        /* Flags */
        PFF_INTEGER | PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 4,
        /* rbits, gbits, bbits, abits */
        16, 16, 16, 16,
        /* Masks and shifts */
        0xFFFF000000000000, 0x0000FFFF00000000, 0x00000000FFFF0000, 0x000000000000FFFF,
        48, 32, 16, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R32_UINT",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 1,
        /* rbits, gbits, bbits, abits */
        32, 0, 0, 0,
        /* Masks and shifts */
        0xFFFFFFFF, 0, 0, 0,
        0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R32G32_UINT",
        /* Bytes per element */
        8,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 2,
        /* rbits, gbits, bbits, abits */
        32, 32, 0, 0,
        /* Masks and shifts */
        0xFFFFFFFF00000000, 0xFFFFFFFF, 0, 0,
        32, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R32G32B32_UINT",
        /* Bytes per element */
        12,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 3,
        /* rbits, gbits, bbits, abits */
        32, 32, 32, 0,
        /* Masks and shifts */
        0, 0, 0, 0,
        64, 32, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R32G32B32A32_UINT",
        /* Bytes per element */
        16,
        /* Flags */
        PFF_INTEGER | PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_UINT, 4,
        /* rbits, gbits, bbits, abits */
        32, 32, 32, 32,
        /* Masks and shifts */
        0, 0, 0, 0,
        96, 64, 32, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8_SINT",
        /* Bytes per element */
        1,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 1,
        /* rbits, gbits, bbits, abits */
        8, 0, 0, 0,
        /* Masks and shifts */
        0xFF, 0, 0, 0,
        0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8G8_SINT",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 2,
        /* rbits, gbits, bbits, abits */
        8, 8, 0, 0,
        /* Masks and shifts */
            0x000000FF, 0x0000FF00, 0, 0,
            0, 8, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8G8B8_SINT",
        /* Bytes per element */
        3,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 3,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 0,
        /* Masks and shifts */
        0xFF0000, 0x00FF00, 0x0000FF, 0,
        16, 8, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8G8B8A8_SINT",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_INTEGER | PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 4,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 8,
        /* Masks and shifts */
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF,
        24, 16, 8, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16_SINT",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 1,
        /* rbits, gbits, bbits, abits */
        16, 0, 0, 0,
        /* Masks and shifts */
        0xFFFF, 0, 0, 0,
        0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16G16_SINT",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 2,
        /* rbits, gbits, bbits, abits */
        16, 16, 0, 0,
        /* Masks and shifts */
        0xFFFF0000, 0x0000FFFF, 0, 0,
        16, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16G16B16_SINT",
        /* Bytes per element */
        6,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 3,
        /* rbits, gbits, bbits, abits */
        16, 16, 16, 0,
        /* Masks and shifts */
        0xFFFF00000000, 0x0000FFFF0000, 0x00000000FFFF, 0,
        32, 16, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16G16B16A16_SINT",
        /* Bytes per element */
        8,
        /* Flags */
        PFF_INTEGER | PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 4,
        /* rbits, gbits, bbits, abits */
        16, 16, 16, 16,
        /* Masks and shifts */
        0xFFFF000000000000, 0x0000FFFF00000000, 0x00000000FFFF0000, 0x000000000000FFFF,
        48, 32, 16, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R32_SINT",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 1,
        /* rbits, gbits, bbits, abits */
        32, 0, 0, 0,
        /* Masks and shifts */
        0xFFFFFFFF, 0, 0, 0,
        0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R32G32_SINT",
        /* Bytes per element */
        8,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 2,
        /* rbits, gbits, bbits, abits */
        32, 32, 0, 0,
        /* Masks and shifts */
        0xFFFFFFFF00000000, 0xFFFFFFFF, 0, 0,
        32, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R32G32B32_SINT",
        /* Bytes per element */
        12,
        /* Flags */
        PFF_INTEGER | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 3,
        /* rbits, gbits, bbits, abits */
        32, 32, 32, 0,
        /* Masks and shifts */
        0, 0, 0, 0,
        64, 32, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R32G32B32A32_SINT",
        /* Bytes per element */
        16,
        /* Flags */
        PFF_INTEGER | PFF_HASALPHA | PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_SINT, 4,
        /* rbits, gbits, bbits, abits */
        32, 32, 32, 32,
        /* Masks and shifts */
        0, 0, 0, 0,
        96, 64, 32, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R9G9B9E5_SHAREDEXP",
        /* Bytes per element */
        4,
        /* Flags */
        PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        9, 9, 9, 0,
        /* Masks and shifts */
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF,
        24, 16, 8, 0
        },
    //-----------------------------------------------------------------------
        {"PF_BC4_UNORM",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED,
        /* Component type and count */
        PCT_BYTE, 1, // Red only
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_BC4_SNORM",
        /* Bytes per element */
        0,
        /* Flags */
            PFF_COMPRESSED | PFF_SIGNED | PFF_SIGNED,
        /* Component type and count */
        PCT_BYTE, 1, // Red only
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_BC5_UNORM",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED,
        /* Component type and count */
        PCT_BYTE, 2, // Red-Green only
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_BC5_SNORM",
        /* Bytes per element */
        0,
        /* Flags */
            PFF_COMPRESSED | PFF_SIGNED | PFF_SIGNED,
        /* Component type and count */
        PCT_BYTE, 2, // Red-Green only
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_BC6H_UF16",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_BC6H_SF16",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_BC7_UNORM",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_BC7_UNORM_SRGB",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8",
        /* Bytes per element */
        1,
        /* Flags */
        PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 1,
        /* rbits, gbits, bbits, abits */
        8, 0, 0, 0,
        /* Masks and shifts */
        0xFF, 0, 0, 0,
        0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_RG8",
        /* Bytes per element */
        2,
        /* Flags */
        PFF_NATIVEENDIAN,
        /* Component type and count */
        PCT_BYTE, 2,
        /* rbits, gbits, bbits, abits */
        8, 8, 0, 0,
        /* Masks and shifts */
            0x000000FF, 0, 0, 0x0000FF00, //Interpret legacy BYTE_LA formats as RG8
            0, 0, 0, 8
            //0x000000FF, 0x0000FF00, 0, 0,
            //0, 8, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8_SNORM",
        /* Bytes per element */
        1,
        /* Flags */
            PFF_NATIVEENDIAN | PFF_SIGNED,
        /* Component type and count */
        PCT_BYTE, 1,
        /* rbits, gbits, bbits, abits */
        8, 0, 0, 0,
        /* Masks and shifts */
        0xFF, 0, 0, 0,
        0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8G8_SNORM",
        /* Bytes per element */
        2,
        /* Flags */
            PFF_NATIVEENDIAN | PFF_SIGNED,
        /* Component type and count */
        PCT_BYTE, 2,
        /* rbits, gbits, bbits, abits */
        8, 8, 0, 0,
        /* Masks and shifts */
            0x000000FF, 0x0000FF00, 0, 0,
            0, 8, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8G8B8_SNORM",
        /* Bytes per element */
        3,
        /* Flags */
            PFF_NATIVEENDIAN | PFF_SIGNED,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 0,
        /* Masks and shifts */
        0xFF0000, 0x00FF00, 0x0000FF, 0,
        16, 8, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R8G8B8A8_SNORM",
        /* Bytes per element */
        4,
        /* Flags */
            PFF_HASALPHA | PFF_NATIVEENDIAN | PFF_SIGNED,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        8, 8, 8, 8,
        /* Masks and shifts */
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF,
        24, 16, 8, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16_SNORM",
        /* Bytes per element */
        2,
        /* Flags */
            PFF_NATIVEENDIAN | PFF_SIGNED,
        /* Component type and count */
        PCT_BYTE, 1,
        /* rbits, gbits, bbits, abits */
        16, 0, 0, 0,
        /* Masks and shifts */
        0xFFFF, 0, 0, 0,
        0, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16G16_SNORM",
        /* Bytes per element */
        4,
        /* Flags */
            PFF_NATIVEENDIAN | PFF_SIGNED,
        /* Component type and count */
        PCT_BYTE, 2,
        /* rbits, gbits, bbits, abits */
        16, 16, 0, 0,
        /* Masks and shifts */
        0xFFFF0000, 0x0000FFFF, 0, 0,
        16, 0, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16G16B16_SNORM",
        /* Bytes per element */
        6,
        /* Flags */
            PFF_NATIVEENDIAN | PFF_SIGNED,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        16, 16, 16, 0,
        /* Masks and shifts */
        0xFFFF00000000, 0x0000FFFF0000, 0x00000000FFFF, 0,
        32, 16, 0, 0
        },
    //-----------------------------------------------------------------------
        {"PF_R16G16B16A16_SNORM",
        /* Bytes per element */
        8,
        /* Flags */
            PFF_HASALPHA | PFF_NATIVEENDIAN | PFF_SIGNED,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        16, 16, 16, 16,
        /* Masks and shifts */
        0xFFFF000000000000, 0x0000FFFF00000000, 0x00000000FFFF0000, 0x000000000000FFFF,
        48, 32, 16, 0
        },

    //-----------------------------------------------------------------------
		{"PF_ETC1_RGB8",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
        //-----------------------------------------------------------------------
        {"PF_ETC2_RGB8",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
        //-----------------------------------------------------------------------
        {"PF_ETC2_RGBA8",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
        //-----------------------------------------------------------------------
        {"PF_ETC2_RGB8A1",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
        //-----------------------------------------------------------------------
        {"PF_ATC_RGB",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED,
        /* Component type and count */
        PCT_BYTE, 3,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
        //-----------------------------------------------------------------------
        {"PF_ATC_RGBA_EXPLICIT_ALPHA",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
        //-----------------------------------------------------------------------
        {"PF_ATC_RGBA_INTERPOLATED_ALPHA",
        /* Bytes per element */
        0,
        /* Flags */
        PFF_COMPRESSED | PFF_HASALPHA,
        /* Component type and count */
        PCT_BYTE, 4,
        /* rbits, gbits, bbits, abits */
        0, 0, 0, 0,
        /* Masks and shifts */
        0, 0, 0, 0, 0, 0, 0, 0
        },
	//-----------------------------------------------------------------------
		{"PF_R4G4B4A4",
		/* Bytes per element */
		2,
		/* Flags */
		PFF_HASALPHA | PFF_NATIVEENDIAN,
		/* Component type and count */
		PCT_BYTE, 4,
		/* rbits, gbits, bbits, abits */
		4, 4, 4, 4,
		/* Masks and shifts */
		0xF000, 0x0F00, 0x00F0, 0x000F,
		12, 8, 4, 0
		},

		//-----------------------------------------------------------------------
		{"PF_D24S8",
		/* Bytes per element */
		4,
		/* Flags */
		PFF_DEPTH,
		/* Component type and count */
		PCT_BYTE, 0,
		/* rbits, gbits, bbits, abits */
		0, 0, 0, 0,
		/* Masks and shifts */
		0, 0, 0, 0,
		0, 0, 0, 0
		},

        //-----------------------------------------------------------------------
        {"PF_D16F",
            /* Bytes per element */
            2,
            /* Flags */
            PFF_DEPTH,
            /* Component type and count */
            PCT_BYTE, 0,
            /* rbits, gbits, bbits, abits */
            0, 0, 0, 0,
            /* Masks and shifts */
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        
		//-----------------------------------------------------------------------
		{"PF_NULL",
		/* Bytes per element */
		0,
		/* Flags */
		0,
		/* Component type and count */
		PCT_BYTE, 0,
		/* rbits, gbits, bbits, abits */
		0, 0, 0, 0,
		/* Masks and shifts */
		0, 0, 0, 0,
		0, 0, 0, 0
		},

		//-----------------------------------------------------------------------
		{"PF_INTZ",
		/* Bytes per element */
		4,
		/* Flags */
		PFF_DEPTH,
		/* Component type and count */
		PCT_BYTE, 0,
		/* rbits, gbits, bbits, abits */
		0, 0, 0, 0,
		/* Masks and shifts */
		0, 0, 0, 0,
		0, 0, 0, 0
		},
    };
    //-----------------------------------------------------------------------
	size_t PixelBox::getConsecutiveSize() const
	{
		return PixelUtil::getMemorySize(getWidth(), getHeight(), getDepth(), format);
	}
	PixelBox PixelBox::getSubVolume(const Box &def) const
	{
		if(PixelUtil::isCompressed(format))
		{
			if(def.left == left && def.top == top && def.front == front &&
			   def.right == right && def.bottom == bottom && def.back == back)
			{
				// Entire buffer is being queried
				return *this;
			}
			
			LogError("Cannot return subvolume of compressed PixelBuffer PixelBox::getSubVolume");
		}
		if(!contains(def))
			LogError("Bounds out of range PixelBox::getSubVolume");

		const size_t elemSize = PixelUtil::getNumElemBytes(format);
		// Calculate new data origin
		// Notice how we do not propagate left/top/front from the incoming box, since
		// the returned pointer is already offset
		PixelBox rval(def.getWidth(), def.getHeight(), def.getDepth(), format, 
			((uint8_t*)data) + ((def.left-left)*elemSize)
			+ ((def.top-top)*rowPitch*elemSize)
			+ ((def.front-front)*slicePitch*elemSize)
		);

		rval.rowPitch = rowPitch;
		rval.slicePitch = slicePitch;
		rval.format = format;

		return rval;
	}
    //-----------------------------------------------------------------------
    /**
    * Directly get the description record for provided pixel format. For debug builds,
    * this checks the bounds of fmt with an assertion.
    */
    const PixelFormatDescription& PixelUtil::getDescriptionFor(const PixelFormat fmt)
    {
        const int ord = (int)fmt;
        ASSERT(ord>=0 && ord<PF_COUNT);

        return _pixelFormats[ord];
    }
 //-----------------------------------------------------------------------
    size_t PixelUtil::getNumElemBytes( PixelFormat format )
    {
        return getDescriptionFor(format).elemBytes;
    }
	//-----------------------------------------------------------------------
	size_t PixelUtil::getMemorySize(uint32_t width, uint32_t height, uint32_t depth, PixelFormat format)
	{
		if(isCompressed(format))
		{
			switch(format)
			{
				// DXT formats work by dividing the image into 4x4 blocks, then encoding each
				// 4x4 block with a certain number of bytes. 
				case PF_DXT1:
					return ((width+3)/4)*((height+3)/4)*8 * depth;
				case PF_DXT2:
				case PF_DXT3:
				case PF_DXT4:
				case PF_DXT5:
					return ((width+3)/4)*((height+3)/4)*16 * depth;
                case PF_BC4_SNORM:
                case PF_BC4_UNORM:
                    return ((width+3)/4)*((height+3)/4)*8 * depth;
                case PF_BC5_SNORM:
                case PF_BC5_UNORM:
                case PF_BC6H_SF16:
                case PF_BC6H_UF16:
                case PF_BC7_UNORM:
                case PF_BC7_UNORM_SRGB:
                    return ((width+3)/4)*((height+3)/4)*16 * depth;

                // Size calculations from the PVRTC OpenGL extension spec
                // http://www.khronos.org/registry/gles/extensions/IMG/IMG_texture_compression_pvrtc.txt
                // Basically, 32 bytes is the minimum texture size.  Smaller textures are padded up to 32 bytes
                case PF_PVRTC_RGB2:
                case PF_PVRTC_RGBA2:
                case PF_PVRTC2_2BPP:
					ASSERT(depth == 1);
                    return (max((int)width, 16) * max((int)height, 8) * 2 + 7) / 8;
                case PF_PVRTC_RGB4:
                case PF_PVRTC_RGBA4:
                case PF_PVRTC2_4BPP:
					ASSERT(depth == 1);
                    return (max((int)width, 8) * max((int)height, 8) * 4 + 7) / 8;
                    
                case PF_ETC1_RGB8:
                case PF_ETC2_RGB8:
                case PF_ETC2_RGBA8:
                case PF_ETC2_RGB8A1:
                    return (max((int)width, 4) * max((int)height, 4))>>1;//max(8, (int)(width * height) >> 1);
                case PF_ATC_RGB:
                    return ((width + 3) / 4) * ((height + 3) / 4) * 8;
                case PF_ATC_RGBA_EXPLICIT_ALPHA:
                case PF_ATC_RGBA_INTERPOLATED_ALPHA:
                    return ((width + 3) / 4) * ((height + 3) / 4) * 16;
				default:
					{
						LogError("Invalid compressed pixel format PixelUtil::getMemorySize");
						return 0;
					}
					
			}
		}
		else
		{
			return width*height*depth*getNumElemBytes(format);
		}
    }
    //-----------------------------------------------------------------------
    uint32_t PixelUtil::getCompressedBlockWidth( PixelFormat format, bool apiStrict )
    {
        switch(format)
        {
            // These formats work by dividing the image into 4x4 blocks, then encoding each
            // 4x4 block with a certain number of bytes.
            case PF_DXT1:
            case PF_DXT2:
            case PF_DXT3:
            case PF_DXT4:
            case PF_DXT5:
            case PF_BC4_SNORM:
            case PF_BC4_UNORM:
            case PF_BC5_SNORM:
            case PF_BC5_UNORM:
            case PF_BC6H_SF16:
            case PF_BC6H_UF16:
            case PF_BC7_UNORM:
            case PF_BC7_UNORM_SRGB:
            case PF_ETC2_RGB8:
            case PF_ETC2_RGBA8:
            case PF_ETC2_RGB8A1:
            case PF_ATC_RGB:
            case PF_ATC_RGBA_EXPLICIT_ALPHA:
            case PF_ATC_RGBA_INTERPOLATED_ALPHA:
                return 4;

            case PF_ETC1_RGB8:
                return apiStrict ? 0 : 4;

            // Size calculations from the PVRTC OpenGL extension spec
            // http://www.khronos.org/registry/gles/extensions/IMG/IMG_texture_compression_pvrtc.txt
            //  "Sub-images are not supportable because the PVRTC
            //  algorithm uses significant adjacency information, so there is
            //  no discrete block of texels that can be decoded as a standalone
            //  sub-unit, and so it follows that no stand alone sub-unit of
            //  data can be loaded without changing the decoding of surrounding
            //  texels."
            // In other words, if the user wants atlas, they can't be automatic
            case PF_PVRTC_RGB2:
            case PF_PVRTC_RGBA2:
            case PF_PVRTC2_2BPP:
            case PF_PVRTC_RGB4:
            case PF_PVRTC_RGBA4:
            case PF_PVRTC2_4BPP:
                return 0;

            default:
                assert( !isCompressed( format ) );
                return 1;
        }
    }
    //-----------------------------------------------------------------------
    uint32_t PixelUtil::getCompressedBlockHeight( PixelFormat format, bool apiStrict )
    {
        return getCompressedBlockWidth( format, apiStrict );
    }
    //-----------------------------------------------------------------------
    size_t PixelUtil::getNumElemBits( PixelFormat format )
    {
        return getDescriptionFor(format).elemBytes * 8;
    }
    //-----------------------------------------------------------------------
    unsigned int PixelUtil::getFlags( PixelFormat format )
    {
        return getDescriptionFor(format).flags;
    }
    //-----------------------------------------------------------------------
    bool PixelUtil::hasAlpha(PixelFormat format)
    {
        return (PixelUtil::getFlags(format) & PFF_HASALPHA) > 0;
    }
    //-----------------------------------------------------------------------
    bool PixelUtil::isFloatingPoint(PixelFormat format)
    {
        return (PixelUtil::getFlags(format) & PFF_FLOAT) > 0;
    }
    //-----------------------------------------------------------------------
    bool PixelUtil::isInteger(PixelFormat format)
    {
        return (PixelUtil::getFlags(format) & PFF_INTEGER) > 0;
    }
    //-----------------------------------------------------------------------
    bool PixelUtil::isCompressed(PixelFormat format)
    {
        return (PixelUtil::getFlags(format) & PFF_COMPRESSED) > 0;
    }
    //-----------------------------------------------------------------------
    bool PixelUtil::isDepth(PixelFormat format)
    {
        return (PixelUtil::getFlags(format) & PFF_DEPTH) > 0;
    }
    //-----------------------------------------------------------------------
    bool PixelUtil::isNativeEndian(PixelFormat format)
    {
        return (PixelUtil::getFlags(format) & PFF_NATIVEENDIAN) > 0;
    }
    //-----------------------------------------------------------------------
    bool PixelUtil::isLuminance(PixelFormat format)
    {
        return (PixelUtil::getFlags(format) & PFF_LUMINANCE) > 0;
    }
    //-----------------------------------------------------------------------
	bool PixelUtil::isValidExtent(size_t width, size_t height, size_t depth, PixelFormat format)
	{
		if(isCompressed(format))
		{
			switch(format)
			{
				case PF_DXT1:
				case PF_DXT2:
				case PF_DXT3:
				case PF_DXT4:
				case PF_DXT5:
                case PF_BC4_SNORM:
                case PF_BC4_UNORM:
                case PF_BC5_SNORM:
                case PF_BC5_UNORM:
                case PF_BC6H_SF16:
                case PF_BC6H_UF16:
                case PF_BC7_UNORM:
                case PF_BC7_UNORM_SRGB:
					return ((width&3)==0 && (height&3)==0 && depth==1);
				default:
					return true;
			}
		}
		else
		{
			return true;
		}
	}
	//-----------------------------------------------------------------------
    void PixelUtil::getBitDepths(PixelFormat format, int rgba[4])
    {
        const PixelFormatDescription &des = getDescriptionFor(format);
        rgba[0] = des.rbits;
        rgba[1] = des.gbits;
        rgba[2] = des.bbits;
        rgba[3] = des.abits;
    }
	//-----------------------------------------------------------------------
	void PixelUtil::getBitMasks(PixelFormat format, uint64_t rgba[4])
    {
        const PixelFormatDescription &des = getDescriptionFor(format);
        rgba[0] = des.rmask;
        rgba[1] = des.gmask;
        rgba[2] = des.bmask;
        rgba[3] = des.amask;
    }
	//---------------------------------------------------------------------
	void PixelUtil::getBitShifts(PixelFormat format, unsigned char rgba[4])
	{
		const PixelFormatDescription &des = getDescriptionFor(format);
		rgba[0] = des.rshift;
		rgba[1] = des.gshift;
		rgba[2] = des.bshift;
		rgba[3] = des.ashift;
	}
    //-----------------------------------------------------------------------
    string PixelUtil::getFormatName(PixelFormat srcformat)
    {
        return getDescriptionFor(srcformat).name;
    }
    //-----------------------------------------------------------------------
    bool PixelUtil::isAccessible(PixelFormat srcformat)
    {
        if (srcformat == PF_UNKNOWN)
            return false;
        unsigned int flags = getFlags(srcformat);
        return !((flags & PFF_COMPRESSED) || (flags & PFF_DEPTH));
    }
    //-----------------------------------------------------------------------
    PixelComponentType PixelUtil::getComponentType(PixelFormat fmt)
    {
        const PixelFormatDescription &des = getDescriptionFor(fmt);
        return des.componentType;
    }
    //-----------------------------------------------------------------------
    size_t PixelUtil::getComponentCount(PixelFormat fmt)
    {
        const PixelFormatDescription &des = getDescriptionFor(fmt);
        return des.componentCount;
    }
    //-----------------------------------------------------------------------
    PixelFormat PixelUtil::getFormatFromName(const string& name, bool accessibleOnly, bool caseSensitive)
    {
        string tmp = name;
        if (!caseSensitive)
        {
            // We are stored upper-case format names.
            StringUtil::toUpperCase(tmp);
        }

        for (int i = 0; i < PF_COUNT; ++i)
        {
            PixelFormat pf = static_cast<PixelFormat>(i);
            if (!accessibleOnly || isAccessible(pf))
            {
                if (tmp == getFormatName(pf))
                    return pf;
            }
        }
        return PF_UNKNOWN;
    }
    //-----------------------------------------------------------------------
    string PixelUtil::getBNFExpressionOfPixelFormats(bool accessibleOnly)
    {
        // Collect format names sorted by length, it's required by BNF compiler
        // that similar tokens need longer ones comes first.
        typedef multimap<string::size_type, string> FormatNameMap;
        FormatNameMap formatNames;
        for (size_t i = 0; i < PF_COUNT; ++i)
        {
            PixelFormat pf = static_cast<PixelFormat>(i);
            if (!accessibleOnly || isAccessible(pf))
            {
                string formatName = getFormatName(pf);
                formatNames.insert(std::make_pair(formatName.length(), formatName));
            }
        }

        // Populate the BNF expression in reverse order
        string result;
        // Note: Stupid M$ VC7.1 can't dealing operator!= with FormatNameMap::const_reverse_iterator.
        for (FormatNameMap::reverse_iterator j = formatNames.rbegin(); j != formatNames.rend(); ++j)
        {
            if (!result.empty())
                result += " | ";
            result += "'" + j->second + "'";
        }

        return result;
    }
    //-----------------------------------------------------------------------
    PixelFormat PixelUtil::getFormatForBitDepths(PixelFormat fmt, uint16_t integerBits, uint16_t floatBits)
    {
        switch (integerBits)
        {
        case 16:
            switch (fmt)
            {
            case PF_R8G8B8:
            case PF_X8R8G8B8:
                return PF_R5G6B5;

            case PF_B8G8R8:
            case PF_X8B8G8R8:
                return PF_B5G6R5;

            case PF_A8R8G8B8:
            case PF_R8G8B8A8:
				return PF_A4R4G4B4;
            case PF_A8B8G8R8:
            case PF_B8G8R8A8:
                return PF_R4G4B4A4;

            case PF_A2R10G10B10:
            case PF_A2B10G10R10:
                return PF_A1R5G5B5;

            default:
                // use original image format
                break;
            }
            break;

        case 32:
            switch (fmt)
            {
            case PF_R5G6B5:
                return PF_X8R8G8B8;

            case PF_B5G6R5:
                return PF_X8B8G8R8;

            case PF_A4R4G4B4:
                return PF_A8R8G8B8;
           
			case PF_R4G4B4A4:
				return PF_A8B8G8R8;

            case PF_A1R5G5B5:
                return PF_A2R10G10B10;

            default:
                // use original image format
                break;
            }
            break;

        default:
            // use original image format
            break;
        }

        switch (floatBits)
        {
        case 16:
            switch (fmt)
            {
            case PF_FLOAT32_R:
                return PF_FLOAT16_R;

            case PF_FLOAT32_RGB:
                return PF_FLOAT16_RGB;

            case PF_FLOAT32_RGBA:
                return PF_FLOAT16_RGBA;

            default:
                // use original image format
                break;
            }
            break;

        case 32:
            switch (fmt)
            {
            case PF_FLOAT16_R:
                return PF_FLOAT32_R;

            case PF_FLOAT16_RGB:
                return PF_FLOAT32_RGB;

            case PF_FLOAT16_RGBA:
                return PF_FLOAT32_RGBA;

            default:
                // use original image format
                break;
            }
            break;

        default:
            // use original image format
            break;
        }

        return fmt;
    }
    //-----------------------------------------------------------------------
    /*************************************************************************
    * Pixel packing/unpacking utilities
    */
    void PixelUtil::packColour(const ColourValue &colour, const PixelFormat pf,  void* dest)
    {
        packColour(colour.r, colour.g, colour.b, colour.a, pf, dest);
    }
    //-----------------------------------------------------------------------
    void PixelUtil::packColour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const PixelFormat pf,  void* dest)
    {
        const PixelFormatDescription &des = getDescriptionFor(pf);
        if(des.flags & PFF_NATIVEENDIAN) {
            // Shortcut for integer formats packing
            unsigned int value = ((Bitwise::fixedToFixed(r, 8, des.rbits)<<des.rshift) & des.rmask) |
                ((Bitwise::fixedToFixed(g, 8, des.gbits)<<des.gshift) & des.gmask) |
                ((Bitwise::fixedToFixed(b, 8, des.bbits)<<des.bshift) & des.bmask) |
                ((Bitwise::fixedToFixed(a, 8, des.abits)<<des.ashift) & des.amask);
            // And write to memory
            Bitwise::intWrite(dest, des.elemBytes, value);
        } else {
            // Convert to float
            packColour((float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)a/255.0f, pf, dest);
        }
    }
    //-----------------------------------------------------------------------
    void PixelUtil::packColour(const float r, const float g, const float b, const float a, const PixelFormat pf,  void* dest)
    {
        // Catch-it-all here
        const PixelFormatDescription &des = getDescriptionFor(pf);
        if(des.flags & PFF_NATIVEENDIAN) {
            // Do the packing
            //std::cerr << dest << " " << r << " " << g <<  " " << b << " " << a << std::endl;
            const unsigned int value = ((Bitwise::floatToFixed(r, des.rbits)<<des.rshift) & des.rmask) |
                ((Bitwise::floatToFixed(g, des.gbits)<<des.gshift) & des.gmask) |
                ((Bitwise::floatToFixed(b, des.bbits)<<des.bshift) & des.bmask) |
                ((Bitwise::floatToFixed(a, des.abits)<<des.ashift) & des.amask);
            // And write to memory
            Bitwise::intWrite(dest, des.elemBytes, value);
        } else {
            switch(pf)
            {
            case PF_FLOAT32_R:
                ((float*)dest)[0] = r;
                break;
			case PF_FLOAT32_GR:
				((float*)dest)[0] = g;
				((float*)dest)[1] = r;
				break;
            case PF_FLOAT32_RGB:
                ((float*)dest)[0] = r;
                ((float*)dest)[1] = g;
                ((float*)dest)[2] = b;
                break;
            case PF_FLOAT32_RGBA:
                ((float*)dest)[0] = r;
                ((float*)dest)[1] = g;
                ((float*)dest)[2] = b;
                ((float*)dest)[3] = a;
                break;
            case PF_FLOAT16_R:
                ((uint16_t*)dest)[0] = Bitwise::floatToHalf(r);
                break;
			case PF_FLOAT16_GR:
				((uint16_t*)dest)[0] = Bitwise::floatToHalf(g);
				((uint16_t*)dest)[1] = Bitwise::floatToHalf(r);
				break;
            case PF_FLOAT16_RGB:
                ((uint16_t*)dest)[0] = Bitwise::floatToHalf(r);
                ((uint16_t*)dest)[1] = Bitwise::floatToHalf(g);
                ((uint16_t*)dest)[2] = Bitwise::floatToHalf(b);
                break;
            case PF_FLOAT16_RGBA:
                ((uint16_t*)dest)[0] = Bitwise::floatToHalf(r);
                ((uint16_t*)dest)[1] = Bitwise::floatToHalf(g);
                ((uint16_t*)dest)[2] = Bitwise::floatToHalf(b);
                ((uint16_t*)dest)[3] = Bitwise::floatToHalf(a);
                break;
            case PF_SHORT_RGB:
				((uint16_t*)dest)[0] = (uint16_t)Bitwise::floatToFixed(r, 16);
                ((uint16_t*)dest)[1] = (uint16_t)Bitwise::floatToFixed(g, 16);
                ((uint16_t*)dest)[2] = (uint16_t)Bitwise::floatToFixed(b, 16);
                break;
			case PF_SHORT_RGBA:
				((uint16_t*)dest)[0] = (uint16_t)Bitwise::floatToFixed(r, 16);
                ((uint16_t*)dest)[1] = (uint16_t)Bitwise::floatToFixed(g, 16);
                ((uint16_t*)dest)[2] = (uint16_t)Bitwise::floatToFixed(b, 16);
                ((uint16_t*)dest)[3] = (uint16_t)Bitwise::floatToFixed(a, 16);
				break;
			case PF_BYTE_LA:
				((uint8_t*)dest)[0] = (uint8_t)Bitwise::floatToFixed(r, 8);
                ((uint8_t*)dest)[1] = (uint8_t)Bitwise::floatToFixed(a, 8);
				break;
            default:
                // Not yet supported
                LogError("pack to getFormatName(pf) not implemented PixelUtil::packColour");
                break;
            }
        }
    }
    //-----------------------------------------------------------------------
    void PixelUtil::unpackColour(ColourValue *colour, PixelFormat pf,  const void* src)
    {
        unpackColour(&colour->r, &colour->g, &colour->b, &colour->a, pf, src);
    }
    //-----------------------------------------------------------------------
    void PixelUtil::unpackColour(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a, PixelFormat pf,  const void* src)
    {
        const PixelFormatDescription &des = getDescriptionFor(pf);
        if(des.flags & PFF_NATIVEENDIAN) {
            // Shortcut for integer formats unpacking
            const unsigned int value = Bitwise::intRead(src, des.elemBytes);
            if(des.flags & PFF_LUMINANCE)
            {
                // Luminance format -- only rbits used
                *r = *g = *b = (uint8_t)Bitwise::fixedToFixed(
                    (value & des.rmask)>>des.rshift, des.rbits, 8);
            }
            else
            {
                *r = (uint8_t)Bitwise::fixedToFixed((value & des.rmask)>>des.rshift, des.rbits, 8);
                *g = (uint8_t)Bitwise::fixedToFixed((value & des.gmask)>>des.gshift, des.gbits, 8);
                *b = (uint8_t)Bitwise::fixedToFixed((value & des.bmask)>>des.bshift, des.bbits, 8);
            }
            if(des.flags & PFF_HASALPHA)
            {
                *a = (uint8_t)Bitwise::fixedToFixed((value & des.amask)>>des.ashift, des.abits, 8);
            }
            else
            {
                *a = 255; // No alpha, default a component to full
            }
        } else {
            // Do the operation with the more generic floating point
            float rr = 0, gg = 0, bb = 0, aa = 0;
            unpackColour(&rr,&gg,&bb,&aa, pf, src);
            *r = (uint8_t)Bitwise::floatToFixed(rr, 8);
            *g = (uint8_t)Bitwise::floatToFixed(gg, 8);
            *b = (uint8_t)Bitwise::floatToFixed(bb, 8);
            *a = (uint8_t)Bitwise::floatToFixed(aa, 8);
        }
    }
    //-----------------------------------------------------------------------
    void PixelUtil::unpackColour(float *r, float *g, float *b, float *a,
        PixelFormat pf,  const void* src)
    {
        const PixelFormatDescription &des = getDescriptionFor(pf);
        if(des.flags & PFF_NATIVEENDIAN) {
            // Shortcut for integer formats unpacking
            const unsigned int value = Bitwise::intRead(src, des.elemBytes);
            if(des.flags & PFF_LUMINANCE)
            {
                // Luminance format -- only rbits used
                *r = *g = *b = Bitwise::fixedToFloat(
                    (value & des.rmask)>>des.rshift, des.rbits);
            }
            else
            {
                *r = Bitwise::fixedToFloat((value & des.rmask)>>des.rshift, des.rbits);
                *g = Bitwise::fixedToFloat((value & des.gmask)>>des.gshift, des.gbits);
                *b = Bitwise::fixedToFloat((value & des.bmask)>>des.bshift, des.bbits);
            }
            if(des.flags & PFF_HASALPHA)
            {
                *a = Bitwise::fixedToFloat((value & des.amask)>>des.ashift, des.abits);
            }
            else
            {
                *a = 1.0f; // No alpha, default a component to full
            }
        } else {
            switch(pf)
            {
            case PF_FLOAT32_R:
                *r = *g = *b = ((float*)src)[0];
                *a = 1.0f;
                break;
			case PF_FLOAT32_GR:
				*g = ((float*)src)[0];
				*r = *b = ((float*)src)[1];
				*a = 1.0f;
				break;
            case PF_FLOAT32_RGB:
                *r = ((float*)src)[0];
                *g = ((float*)src)[1];
                *b = ((float*)src)[2];
                *a = 1.0f;
                break;
            case PF_FLOAT32_RGBA:
                *r = ((float*)src)[0];
                *g = ((float*)src)[1];
                *b = ((float*)src)[2];
                *a = ((float*)src)[3];
                break;
            case PF_FLOAT16_R:
                *r = *g = *b = Bitwise::halfToFloat(((uint16_t*)src)[0]);
                *a = 1.0f;
                break;
			case PF_FLOAT16_GR:
				*g = Bitwise::halfToFloat(((uint16_t*)src)[0]);
				*r = *b = Bitwise::halfToFloat(((uint16_t*)src)[1]);
				*a = 1.0f;
				break;
            case PF_FLOAT16_RGB:
                *r = Bitwise::halfToFloat(((uint16_t*)src)[0]);
                *g = Bitwise::halfToFloat(((uint16_t*)src)[1]);
                *b = Bitwise::halfToFloat(((uint16_t*)src)[2]);
                *a = 1.0f;
                break;
            case PF_FLOAT16_RGBA:
                *r = Bitwise::halfToFloat(((uint16_t*)src)[0]);
                *g = Bitwise::halfToFloat(((uint16_t*)src)[1]);
                *b = Bitwise::halfToFloat(((uint16_t*)src)[2]);
                *a = Bitwise::halfToFloat(((uint16_t*)src)[3]);
                break;
			case PF_SHORT_RGB:
				*r = Bitwise::fixedToFloat(((uint16_t*)src)[0], 16);
                *g = Bitwise::fixedToFloat(((uint16_t*)src)[1], 16);
				*b = Bitwise::fixedToFloat(((uint16_t*)src)[2], 16);
				*a = 1.0f;
				break;
			case PF_SHORT_RGBA:
				*r = Bitwise::fixedToFloat(((uint16_t*)src)[0], 16);
                *g = Bitwise::fixedToFloat(((uint16_t*)src)[1], 16);
				*b = Bitwise::fixedToFloat(((uint16_t*)src)[2], 16);
				*a = Bitwise::fixedToFloat(((uint16_t*)src)[3], 16);
				break;
			case PF_BYTE_LA:
				*r = *g = *b = Bitwise::fixedToFloat(((uint8_t*)src)[0], 8);
				*a = Bitwise::fixedToFloat(((uint8_t*)src)[1], 8);
				break;
            default:
                // Not yet supported
                LogError("unpack from not implemented PixelUtil::unpackColour");
                break;
            }
        }
    }
    //-----------------------------------------------------------------------
    /* Convert pixels from one format to another */
    void PixelUtil::bulkPixelConversion(void *srcp, PixelFormat srcFormat,
        void *destp, PixelFormat dstFormat, unsigned int count)
    {
        PixelBox src(count, 1, 1, srcFormat, srcp),
				 dst(count, 1, 1, dstFormat, destp);

        bulkPixelConversion(src, dst);
    }
    //-----------------------------------------------------------------------
    void PixelUtil::bulkCompressedSubregion( const PixelBox &src, const PixelBox &dst,
                                             const Box &dstRegion )
    {
        assert(src.getWidth()  == dstRegion.getWidth() &&
               src.getHeight() == dstRegion.getHeight() &&
               src.getDepth()  == dstRegion.getDepth());

        assert( dst.contains( dstRegion ) );
        assert( dst.format == src.format );
        assert( src.isConsecutive() && dst.isConsecutive() );

        if( src.getWidth()  == dst.getWidth() &&
            src.getHeight() == dst.getHeight() &&
            src.getDepth()  == dst.getDepth() )
        {
            bulkPixelConversion( src, dst );
            return;
        }

        uint32_t blockWidth  = PixelUtil::getCompressedBlockWidth( dst.format, false );
        uint32_t blockHeight = PixelUtil::getCompressedBlockHeight( dst.format, false );
        uint32_t blockResolution = blockWidth * blockHeight;
        if( !blockWidth || !blockHeight )
        {
            LogError("Cannot transfer subregions of the image when compressed by format %s . You must update the entire image, PixelUtil::bulkCompressedSubregion",
                PixelUtil::getFormatName( dst.format ).c_str());
            return;
        }

        if( dstRegion.left % blockWidth || dstRegion.right % blockWidth ||
            dstRegion.top % blockHeight || dstRegion.bottom % blockHeight )
        {
            LogError("Image transfers for the compressed format %s  requires subregions to be aligned to %d x %d blocks, PixelUtil::bulkPixelConversion",
                         PixelUtil::getFormatName( dst.format ).c_str(), blockWidth, blockHeight);
            return;
        }

        size_t blockSize = PixelUtil::getMemorySize( blockWidth, blockHeight, 1, dst.format );

        for( size_t z=dstRegion.front; z<dstRegion.back; ++z )
        {
            size_t dstZ = z * ( (dst.getWidth() * dst.getHeight()) / blockResolution );
            size_t srcZ = z * ( (src.getWidth() * src.getHeight()) / blockResolution );
            for( size_t y=dstRegion.top; y<dstRegion.bottom; y += blockHeight )
            {
                size_t dstY = ((y - dst.top) * dst.getWidth()) / blockResolution;
                size_t srcY = (y * src.getWidth()) / blockResolution;
                memcpy( (uint8_t*)(dst.data) + ( (dstZ + dstY + dstRegion.left / blockWidth) * blockSize ),
                        (uint8_t*)(src.data) + ( (srcZ + srcY ) * blockSize ),
                        (dstRegion.getWidth() / blockWidth) * blockSize );
            }
        }
    }
    //-----------------------------------------------------------------------
    void PixelUtil::bulkPixelConversion(const PixelBox &src, const PixelBox &dst)
    {
        ASSERT(src.getWidth() == dst.getWidth() &&
			   src.getHeight() == dst.getHeight() &&
			   src.getDepth() == dst.getDepth());

		// Check for compressed formats, we don't support decompression, compression or recoding
		if(PixelUtil::isCompressed(src.format) || PixelUtil::isCompressed(dst.format))
		{
			if(src.format == dst.format)
			{
                if( src.getConsecutiveSize() && dst.isConsecutive() )
				    memcpy(dst.data, src.data, src.getConsecutiveSize());
                else
                {
                    ASSERT(false);
                    const size_t rowSize = PixelUtil::getMemorySize( src.getWidth(), 1, 1, src.format );
                    const uint32_t blockWidth  = PixelUtil::getCompressedBlockWidth( dst.format, false );
                    const uint32_t blockHeight = PixelUtil::getCompressedBlockHeight( dst.format, false );

                    if( blockWidth == 0 || blockHeight == 0 )
                    {
                        LogError("This format should be consecutive!, PixelUtil::bulkPixelConversion");
                        return;
                    }

                    uint8_t *srcptr = static_cast<uint8_t*>(src.data)
                        + (src.left + blockWidth - 1) / blockWidth +
                            (src.top + blockHeight - 1) / blockHeight * src.rowPitch +
                            src.front * src.slicePitch;
                    uint8_t *dstptr = static_cast<uint8_t*>(dst.data)
                        + (dst.left + blockWidth - 1) / blockWidth +
                            (dst.top + blockHeight - 1) / blockHeight * dst.rowPitch +
                            dst.front * dst.slicePitch;

                    // Calculate pitches+skips in bytes
                    const size_t srcRowPitchBytes = src.rowPitch;
                    const size_t srcSliceSkipBytes = src.getSliceSkip();

                    const size_t dstRowPitchBytes   = dst.rowPitch;
                    const size_t dstSliceSkipBytes  = dst.getSliceSkip();

                    const size_t compressedSrcTop = (src.top + blockHeight - 1) / blockHeight;
                    const size_t compressedSrcBottom = (src.bottom + blockHeight - 1) / blockHeight;

                    for( size_t z=src.front; z<src.back; ++z )
                    {
                        for( size_t y=compressedSrcTop; y<compressedSrcBottom; ++y )
                        {
                            memcpy( dstptr, srcptr, rowSize );
                            srcptr += srcRowPitchBytes;
                            dstptr += dstRowPitchBytes;
                        }

                        srcptr += srcSliceSkipBytes;
                        dstptr += dstSliceSkipBytes;
                    }
                }
                return;
            }
            else
			{
				LogError("This method can not be used to compress or decompress images PixelUtil::bulkPixelConversion");
                return;
			}
		}

        // The easy case
        if(src.format == dst.format) {
            // Everything consecutive?
            if(src.isConsecutive() && dst.isConsecutive())
            {
				memcpy(dst.data, src.data, src.getConsecutiveSize());
                return;
            }

            const size_t srcPixelSize = PixelUtil::getNumElemBytes(src.format);
            const size_t dstPixelSize = PixelUtil::getNumElemBytes(dst.format);
            uint8_t *srcptr = static_cast<uint8_t*>(src.data)
                + (src.left + src.top * src.rowPitch + src.front * src.slicePitch) * srcPixelSize;
            uint8_t *dstptr = static_cast<uint8_t*>(dst.data)
				+ (dst.left + dst.top * dst.rowPitch + dst.front * dst.slicePitch) * dstPixelSize;

            // Calculate pitches+skips in bytes
            const size_t srcRowPitchBytes = src.rowPitch*srcPixelSize;
            //const size_t srcRowSkipBytes = src.getRowSkip()*srcPixelSize;
            const size_t srcSliceSkipBytes = src.getSliceSkip()*srcPixelSize;

            const size_t dstRowPitchBytes = dst.rowPitch*dstPixelSize;
            //const size_t dstRowSkipBytes = dst.getRowSkip()*dstPixelSize;
            const size_t dstSliceSkipBytes = dst.getSliceSkip()*dstPixelSize;

            // Otherwise, copy per row
            const size_t rowSize = src.getWidth()*srcPixelSize;
            for(size_t z=src.front; z<src.back; z++)
            {
                for(size_t y=src.top; y<src.bottom; y++)
                {
					memcpy(dstptr, srcptr, rowSize);
                    srcptr += srcRowPitchBytes;
                    dstptr += dstRowPitchBytes;
                }
                srcptr += srcSliceSkipBytes;
                dstptr += dstSliceSkipBytes;
            }
            return;
        }
		// Converting to PF_X8R8G8B8 is exactly the same as converting to
		// PF_A8R8G8B8. (same with PF_X8B8G8R8 and PF_A8B8G8R8)
		if(dst.format == PF_X8R8G8B8 || dst.format == PF_X8B8G8R8)
		{
			// Do the same conversion, with PF_A8R8G8B8, which has a lot of
			// optimized conversions
			PixelBox tempdst = dst;
			tempdst.format = dst.format==PF_X8R8G8B8?PF_A8R8G8B8:PF_A8B8G8R8;
			bulkPixelConversion(src, tempdst);
			return;
		}
		// Converting from PF_X8R8G8B8 is exactly the same as converting from
		// PF_A8R8G8B8, given that the destination format does not have alpha.
		if((src.format == PF_X8R8G8B8||src.format == PF_X8B8G8R8) && !hasAlpha(dst.format))
		{
			// Do the same conversion, with PF_A8R8G8B8, which has a lot of
			// optimized conversions
			PixelBox tempsrc = src;
			tempsrc.format = src.format==PF_X8R8G8B8?PF_A8R8G8B8:PF_A8B8G8R8;
			bulkPixelConversion(tempsrc, dst);
			return;
		}

        // Is there a specialized, inlined, conversion?
        if(doOptimizedConversion(src, dst))
        {
            // If so, good
            return;
        }

        const size_t srcPixelSize = PixelUtil::getNumElemBytes(src.format);
        const size_t dstPixelSize = PixelUtil::getNumElemBytes(dst.format);
        uint8_t *srcptr = static_cast<uint8_t*>(src.data)
            + (src.left + src.top * src.rowPitch + src.front * src.slicePitch) * srcPixelSize;
        uint8_t *dstptr = static_cast<uint8_t*>(dst.data)
            + (dst.left + dst.top * dst.rowPitch + dst.front * dst.slicePitch) * dstPixelSize;
		
		// Old way, not taking into account box dimensions
		//uint8_t *srcptr = static_cast<uint8_t*>(src.data), *dstptr = static_cast<uint8_t*>(dst.data);

        // Calculate pitches+skips in bytes
        const size_t srcRowSkipBytes = src.getRowSkip()*srcPixelSize;
        const size_t srcSliceSkipBytes = src.getSliceSkip()*srcPixelSize;
        const size_t dstRowSkipBytes = dst.getRowSkip()*dstPixelSize;
        const size_t dstSliceSkipBytes = dst.getSliceSkip()*dstPixelSize;

        // The brute force fallback
        float r = 0, g = 0, b = 0, a = 1;
        for(size_t z=src.front; z<src.back; z++)
        {
            for(size_t y=src.top; y<src.bottom; y++)
            {
                for(size_t x=src.left; x<src.right; x++)
                {
                    unpackColour(&r, &g, &b, &a, src.format, srcptr);
                    packColour(r, g, b, a, dst.format, dstptr);
                    srcptr += srcPixelSize;
                    dstptr += dstPixelSize;
                } 
                srcptr += srcRowSkipBytes;
                dstptr += dstRowSkipBytes;
            }
            srcptr += srcSliceSkipBytes;
            dstptr += dstSliceSkipBytes;
        }
    }
    //-----------------------------------------------------------------------
    void PixelUtil::convertForNormalMapping(const PixelBox &src, const PixelBox &dst)
    {
        assert(src.getWidth() == dst.getWidth() &&
               src.getHeight() == dst.getHeight() &&
               src.getDepth() == dst.getDepth() &&
               (dst.format == PF_R8G8_SNORM || dst.format == PF_RG8 || dst.format == PF_BYTE_LA)  );

        const PixelFormatDescription &srcDesc = getDescriptionFor( src.format );

        const size_t srcPixelSize = PixelUtil::getNumElemBytes(src.format);
        const size_t dstPixelSize = PixelUtil::getNumElemBytes(dst.format);
        uint8_t *srcPtr = static_cast<uint8_t*>(src.data)
            + (src.left + src.top * src.rowPitch + src.front * src.slicePitch) * srcPixelSize;
        int8_t *dstPtr = static_cast<int8_t*>(dst.data)
            + (dst.left + dst.top * dst.rowPitch + dst.front * dst.slicePitch) * dstPixelSize;

        // Calculate pitches+skips in bytes
        const size_t srcRowSkipBytes    = src.getRowSkip() * srcPixelSize;
        const size_t srcSliceSkipBytes  = src.getSliceSkip() * srcPixelSize;
        const size_t dstRowSkipBytes    = dst.getRowSkip() * dstPixelSize;
        const size_t dstSliceSkipBytes  = dst.getSliceSkip() * dstPixelSize;

        uint8_t notLuminanceMask  = srcDesc.flags & PFF_LUMINANCE ? 0x00 : 0xFF;
        uint8_t luminanceMask     = srcDesc.flags & PFF_LUMINANCE ? 0xFF : 0x00;

        if( srcDesc.flags & PFF_FLOAT )
        {
            LogError("Floating point formats for normal maps is not implemented yet, PixelUtil::convertForNormalMapping" );
            return;
        }

        if( srcDesc.flags & PFF_SIGNED )
        {
            LogError("Signed format origins for normal maps is not tested, PixelUtil::convertForNormalMapping" );
            return;
        }

        uint8_t shiftOffset = 0x7F;

        if( dst.format == PF_BYTE_LA || dst.format == PF_RG8 )
            shiftOffset = 0x00;

        uint8_t r, g, b, a;
        for(size_t z=src.front; z<src.back; z++)
        {
            for(size_t y=src.top; y<src.bottom; y++)
            {
                for(size_t x=src.left; x<src.right; x++)
                {
                    unpackColour( &r, &g, &b, &a, src.format, srcPtr );

                    g = (g & notLuminanceMask) | (a & luminanceMask);

                    *dstPtr++ = r - shiftOffset;
                    *dstPtr++ = g - shiftOffset;

                    srcPtr += srcPixelSize;
                }

                srcPtr += srcRowSkipBytes;
                dstPtr += dstRowSkipBytes;
            }

            srcPtr += srcSliceSkipBytes;
            dstPtr += dstSliceSkipBytes;
        }
    }
    //-----------------------------------------------------------------------
    void PixelUtil::bulkPixelVerticalFlip(const PixelBox &box)
    {
        // Check for compressed formats, we don't support decompression, compression or recoding
        if(PixelUtil::isCompressed(box.format))
        {
            LogError("This method can not be used for compressed formats, PixelUtil::bulkPixelVerticalFlip");
            return;
        }
        
        const size_t pixelSize = PixelUtil::getNumElemBytes(box.format);
        const size_t copySize = (box.right - box.left) * pixelSize;

        // Calculate pitches in bytes
        const size_t rowPitchBytes = box.rowPitch * pixelSize;
        const size_t slicePitchBytes = box.slicePitch * pixelSize;

        uint8_t *basesrcptr = static_cast<uint8_t*>(box.data)
            + (box.left + box.top * box.rowPitch + box.front * box.slicePitch) * pixelSize;
        uint8_t *basedstptr = basesrcptr + (box.bottom - box.top - 1) * rowPitchBytes;
        uint8_t* tmpptr = (uint8_t*)new uint8_t[copySize];
        
        // swap rows
        const size_t halfRowCount = (box.bottom - box.top) >> 1;
        for(size_t z = box.front; z < box.back; z++)
        {
            uint8_t* srcptr = basesrcptr;
            uint8_t* dstptr = basedstptr;
            for(size_t y = 0; y < halfRowCount; y++)
            {
                // swap rows
                memcpy(tmpptr, dstptr, copySize);
                memcpy(dstptr, srcptr, copySize);
                memcpy(srcptr, tmpptr, copySize);
                srcptr += rowPitchBytes;
                dstptr -= rowPitchBytes;
            }
            basesrcptr += slicePitchBytes;
            basedstptr += slicePitchBytes;
        }
        
        SAFE_DELETE_ARRAY(tmpptr);
    }

	static int computeLog(int value)
	{
		int i;

		i = 0;

		/* Error! */
		if (value == 0) return 0;

		for (;;)
		{
			if (value<= 1)
			{
				return i;
			}
			value = value >> 1;
			i++;
		}
	}

	int PixelUtil::getNumMipmaps( int nWidth, int nHeight )
	{
		int logW = computeLog(nWidth);
		int logH = computeLog(nHeight);
		int level = (logW > logH ? logW : logH);
		return level;
	}

    ColourValue PixelBox::getColourAt(size_t x, size_t y, size_t z)
    {
        ColourValue cv;

        size_t pixelSize = PixelUtil::getNumElemBytes(format);
        size_t pixelOffset = pixelSize * (z * slicePitch + y * rowPitch + x);
        PixelUtil::unpackColour(&cv, format, (unsigned char *)data + pixelOffset);

        return cv;
    }

    void PixelBox::setColourAt(ColourValue const &cv, size_t x, size_t y, size_t z)
    {
		size_t pixelSize = PixelUtil::getNumElemBytes(format);
        size_t pixelOffset = pixelSize * (z * slicePitch + y * rowPitch + x);
        PixelUtil::packColour(cv, format, (unsigned char *)data + pixelOffset);
    }

}
