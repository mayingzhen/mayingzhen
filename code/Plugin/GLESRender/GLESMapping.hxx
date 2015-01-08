#include "GLESMapping.h"

namespace ma
{

	GLenum GLESMapping::GetGLESUSage(USAGE Usage)
	{
		switch(Usage)
		{
		case USAGE_NO:
			return GL_STATIC_DRAW;
		case USAGE_DYNAMIC:
			return GL_STREAM_DRAW;
		default:
			ASSERT(false);
			return GL_STATIC_DRAW;
		};
	}

	GLenum GLESMapping::GetGLESPrimitiveType(PRIMITIVE_TYPE type)
	{
		switch (type)
		{
		case PRIM_POINTLIST:
			return GL_POINTS;

		case PRIM_LINELIST:
			return GL_LINES;

		case PRIM_LINESTRIP:
			return GL_LINE_STRIP;

		case PRIM_TRIANGLELIST:
			return GL_TRIANGLES;

		case PRIM_TRIANGLESTRIP:
			return GL_TRIANGLE_STRIP;

		case PRIM_TRIANGLEFAN:
			return GL_TRIANGLE_FAN;

		default:
			ASSERT(0 && "Invalid Declaration Usage.");
			return GL_TRIANGLES;
		} 
	}

	GLenum GLESMapping::GetGLESIndexType(INDEX_TYPE type)
	{
		if (type == INDEX_TYPE_U16)
		{
			return GL_UNSIGNED_SHORT;
		}
		else if (type == INDEX_TYPE_U32)
		{
			return GL_UNSIGNED_INT;
		}
		else
		{
			ASSERT(0);
			return GL_UNSIGNED_SHORT;
		}
	}

	void	GLESMapping::GetGLESDeclType(DECL_USAGE Usage,BYTE nUsageIndex, GLenum& type, 
		GLint& typeCount,GLboolean& normalized,std::string& name)
	{
		if (Usage == DU_POSITION)
		{
			typeCount = 3;
			type = GL_FLOAT;
			normalized = false;
			name = VERTEX_ATTRIBUTE_POSITION_NAME;
		}
		else if (Usage == DU_TEXCOORD)
		{
			typeCount = 2;
			type = GL_FLOAT;
			normalized = false;
			if (nUsageIndex == 0)
				name = VERTEX_ATTRIBUTE_TEXCOORD0_NAME;
			else if (nUsageIndex == 1)
				name = VERTEX_ATTRIBUTE_TEXCOORD1_NAME;
			else
				ASSERT(false);
		}
		else if (Usage == DU_COLOR)
		{
			typeCount = 4;
			type = GL_FLOAT;
			normalized = false;
			if (nUsageIndex == 0)
				name = VERTEX_ATTRIBUTE_COLOR0_NAME;
			else if (nUsageIndex == 1)
				name = VERTEX_ATTRIBUTE_COLOR1_NAME;
			else
				ASSERT(false);
		}
		else if (Usage == DU_NORMAL)
		{
			typeCount = 3;
			type = GL_FLOAT;
			normalized = false;
			name = VERTEX_ATTRIBUTE_NORMAL_NAME;
		}
		else if (Usage == DU_BLENDINDICES)
		{
			typeCount = 4;
			type = GL_UNSIGNED_BYTE;
			normalized = false;
			name = VERTEX_ATTRIBUTE_BLENDINDICES_NAME;
		}
		else if (Usage == DU_BLENDWEIGHT)
		{
			typeCount = 4;
			type = GL_UNSIGNED_BYTE;
			normalized = true;
			name = VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME;
		}

	}

	void GLESMapping::GetGLESFilter(FilterOptions type,GLenum& minFilter,GLenum& magFilter)
	{
		if (type == TFO_POINT)
		{
			minFilter = GL_NEAREST;
			magFilter = GL_NEAREST;
		}
		else if (type == TFO_BILINEAR)
		{
			minFilter = GL_LINEAR;
			magFilter = GL_LINEAR;
		}
		else if (type == TFO_TRILINEAR)
		{
			minFilter = GL_NEAREST_MIPMAP_LINEAR;
			magFilter = GL_LINEAR;
		}
		else if (type == TFO_ANISOTROPIC)
		{
			minFilter = GL_LINEAR_MIPMAP_LINEAR;
			magFilter = GL_LINEAR;
		}
		else
		{
			minFilter = GL_NEAREST;
			magFilter = GL_NEAREST;
			ASSERT(false);
		}
	}

	GLenum GLESMapping::GetGLESWrap(Wrap type)
	{
		if (type == REPEAT)
		{
			return GL_REPEAT;
		}
		else if (type == CLAMP)
		{
			return GL_CLAMP_TO_EDGE;
		}
		else 
		{
			ASSERT(false);
		}

		return GL_REPEAT;
	}

	GLenum GLESMapping::GetGLESDataType(PixelFormat mFormat)
	{
		switch (mFormat)
		{
		case PF_A8:
		case PF_L8:
		case PF_L16:
		case PF_R8G8B8:
		case PF_B8G8R8:
		case PF_BYTE_LA:
			return GL_UNSIGNED_BYTE;
		case PF_R5G6B5:
		case PF_B5G6R5:
			return GL_UNSIGNED_SHORT_5_6_5;
		case PF_A4R4G4B4:
		case PF_R4G4B4A4:
			return GL_UNSIGNED_SHORT_4_4_4_4;
		case PF_A1R5G5B5:
			return GL_UNSIGNED_SHORT_5_5_5_1;

#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
		case PF_X8B8G8R8:
		case PF_A8B8G8R8:
			return GL_UNSIGNED_INT_8_8_8_8_REV;
		case PF_X8R8G8B8:
		case PF_A8B8G8R8:
		case PF_A8R8G8B8:
			return GL_UNSIGNED_INT_8_8_8_8_REV;
		case PF_B8G8R8A8:
			return GL_UNSIGNED_BYTE;
		case PF_R8G8B8A8:
			return GL_UNSIGNED_BYTE;
#else
		case PF_X8B8G8R8:
		case PF_A8B8G8R8:
		case PF_X8R8G8B8:
		case PF_A8R8G8B8:
		case PF_B8G8R8A8:
		case PF_R8G8B8A8:
			return GL_UNSIGNED_BYTE;
#endif
		case PF_FLOAT16_R:
		case PF_FLOAT16_GR:
		case PF_FLOAT16_RGB:
		case PF_FLOAT16_RGBA:
#if GL_OES_texture_half_float
			return GL_HALF_FLOAT_OES;
#else
			return 0;
#endif
#if GL_EXT_texture_rg
		case PF_R8:
		case PF_RG8:
			return GL_UNSIGNED_BYTE;
#endif
		case PF_FLOAT32_R:
		case PF_FLOAT32_GR:
		case PF_FLOAT32_RGB:
		case PF_FLOAT32_RGBA:
			return GL_FLOAT;
		case PF_DXT1:
		case PF_DXT3:
		case PF_DXT5:
		case PF_R3G3B2:
		case PF_A2R10G10B10:
		case PF_A2B10G10R10:
		case PF_SHORT_RGBA:
		case PF_SHORT_RGB:
		case PF_SHORT_GR:
			// TODO not supported
		default:
			return 0;
		}
	}

	GLenum GLESMapping::GetGLESFormat(PixelFormat format)
	{
		switch (format)
		{
		case PF_L8:
		case PF_L16:
			return GL_LUMINANCE;

		case PF_A8:
			return GL_ALPHA;

		case PF_BYTE_LA:
			return GL_LUMINANCE_ALPHA;

#if GL_IMG_texture_compression_pvrtc
		case PF_PVRTC_RGB2:
			return GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
		case PF_PVRTC_RGB4:
			return GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
		case PF_PVRTC_RGBA2:
			return GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
		case PF_PVRTC_RGBA4:
			return GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
#endif
#if GL_IMG_texture_compression_pvrtc2
		case PF_PVRTC2_2BPP:
			return GL_COMPRESSED_PVRTC_2BPPV2_IMG;
		case PF_PVRTC2_4BPP:
			return GL_COMPRESSED_PVRTC_4BPPV2_IMG;
#endif        
#if GL_OES_compressed_ETC1_RGB8_texture
		case PF_ETC1_RGB8:
			return GL_ETC1_RGB8_OES;
#endif
		case PF_X8B8G8R8:
		case PF_X8R8G8B8:
		case PF_A8B8G8R8:
		case PF_A8R8G8B8:
		case PF_B8G8R8A8:
		case PF_A1R5G5B5:
		case PF_A4R4G4B4:
		case PF_R4G4B4A4:
			return GL_RGBA;
		case PF_R5G6B5:
		case PF_B5G6R5:
		case PF_R8G8B8:
		case PF_B8G8R8:
			return GL_RGB;
		case PF_A4L4:
		case PF_R3G3B2:
		case PF_A2R10G10B10:
		case PF_A2B10G10R10:
		case PF_FLOAT16_RGBA:
		case PF_FLOAT32_RGB:
		case PF_FLOAT32_RGBA:
		case PF_SHORT_RGBA:
		case PF_SHORT_RGB:
		case PF_SHORT_GR:
		case PF_DXT1:
#if GL_EXT_texture_compression_dxt1
		return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
#endif
		case PF_DXT3:
#if GL_EXT_texture_compression_s3tc
		return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
#endif
		case PF_DXT5:
#if GL_EXT_texture_compression_s3tc
		return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
#endif
#if GL_EXT_texture_rg
		case PF_FLOAT16_R:
		case PF_FLOAT32_R:
		case PF_R8:
			return GL_RED_EXT;
		case PF_FLOAT16_GR:
		case PF_FLOAT32_GR:
		case PF_RG8:
			return GL_RG_EXT;
#endif
		default:
			return 0;
		}
	}

	//-----------------------------------------------------------------------------
	PixelFormat GLESMapping::GetClosestFormat(GLenum fmt, GLenum dataType)
	{
		switch (fmt)
		{
#if GL_IMG_texture_compression_pvrtc
			case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
				return PF_PVRTC_RGB2;
			case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
				return PF_PVRTC_RGBA2;
			case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
				return PF_PVRTC_RGB4;
			case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
				return PF_PVRTC_RGBA4;
#endif
#if GL_IMG_texture_compression_pvrtc2
			case GL_COMPRESSED_PVRTC_2BPPV2_IMG:
				return PF_PVRTC2_2BPP;
			case GL_COMPRESSED_PVRTC_4BPPV2_IMG:
				return PF_PVRTC2_4BPP;
#endif
#if GL_OES_compressed_ETC1_RGB8_texture
			case GL_ETC1_RGB8_OES:
				return PF_ETC1_RGB8;
#endif
			case GL_LUMINANCE:
				return PF_L8;
			case GL_ALPHA:
				return PF_A8;
			case GL_LUMINANCE_ALPHA:
				return PF_BYTE_LA;

			case GL_RGB:
				switch(dataType)
				{
				case GL_UNSIGNED_SHORT_5_6_5:
					return PF_R5G6B5;
				default:
					//#if (OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS)
					//					return PF_R8G8B8;
					//#else
					return PF_B8G8R8;
					//#endif
				};
			case GL_RGBA:
				switch(dataType)
				{
				case GL_UNSIGNED_SHORT_5_5_5_1:
					return PF_A1R5G5B5;
				case GL_UNSIGNED_SHORT_4_4_4_4:
					//#if (OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS)
					//                    return PF_AR4G4B4;
					//#else
					return PF_R4G4B4A4;
					//#endif
				default:
					//#if (OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS)
					//                    return PF_A8R8G8B8;
					//#else
					return PF_A8B8G8R8;
					//#endif
				}
#ifdef GL_BGRA
			case GL_BGRA:
				return PF_A8B8G8R8;
#endif

#if GL_EXT_texture_compression_dxt1
			case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
			case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
				return PF_DXT1;
#endif
#if GL_EXT_texture_compression_s3tc
			case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
				return PF_DXT3;
			case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
				return PF_DXT5;
#endif

#if GL_EXT_texture_rg
			case GL_R8_EXT:
				return PF_R8;
			case GL_RG8_EXT:
				return PF_RG8;
#endif
			default:
				//TODO: not supported
				return PF_A8R8G8B8;
		};
	}

}


