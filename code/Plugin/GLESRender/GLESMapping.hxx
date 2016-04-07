#include "GLESMapping.h"

namespace ma
{

	GLenum GLESMapping::GetGLESTextureUSage(TEXTURE_USAGE Usage)
	{
		switch(Usage)
		{
		case USAGE_STATIC:
			return GL_STATIC_DRAW;
		case USAGE_DYNAMIC:
			return GL_STREAM_DRAW;
		default:
			ASSERT(false);
			return GL_STATIC_DRAW;
		};
	}

	GLenum GLESMapping::GetGLESBufferUSage(HBU_USAGE Usage)
	{
		switch(Usage)
		{
		case HBU_STATIC:
		case HBU_STATIC_WRITE_ONLY:
			return GL_STATIC_DRAW;
		case HBU_DYNAMIC:
		case HBU_DYNAMIC_WRITE_ONLY:
			return GL_DYNAMIC_DRAW;
		case HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE:
			return GL_STREAM_DRAW;
		default:
			ASSERT(false);
			return GL_DYNAMIC_DRAW;
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

	void	GLESMapping::GetGLESDeclType(DECL_USAGE Usage, BYTE nUsageIndex, DECL_TYPE eType, GLenum& type, 
		GLint& typeCount,GLboolean& normalized,std::string& name)
	{
		if (Usage == DU_POSITION)
		{
			name = VERTEX_ATTRIBUTE_POSITION_NAME;
		}
		else if (Usage == DU_TEXCOORD)
		{
			if (nUsageIndex == 0)
				name = VERTEX_ATTRIBUTE_TEXCOORD0_NAME;
			else if (nUsageIndex == 1)
				name = VERTEX_ATTRIBUTE_TEXCOORD1_NAME;
			else
				ASSERT(false);
		}
		else if (Usage == DU_COLOR)
		{
			if (nUsageIndex == 0)
				name = VERTEX_ATTRIBUTE_COLOR0_NAME;
			else if (nUsageIndex == 1)
				name = VERTEX_ATTRIBUTE_COLOR1_NAME;
			else
				ASSERT(false);
		}
		else if (Usage == DU_NORMAL)
		{
			normalized = false;
			name = VERTEX_ATTRIBUTE_NORMAL_NAME;
		}
		else if (Usage == DU_BLENDINDICES)
		{
			name = VERTEX_ATTRIBUTE_BLENDINDICES_NAME;
		}
		else if (Usage == DU_BLENDWEIGHT)
		{
			name = VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME;
		}

		if (DT_COLOR == eType)
		{
			type = GL_UNSIGNED_BYTE;
			typeCount = 1;
			normalized  = true;
		}
		else if (DT_FLOAT1 == eType)
		{
			type = GL_FLOAT;
			typeCount = 1;
			normalized = false;
		}
		else if (DT_FLOAT2 == eType)
		{
			type = GL_FLOAT;
			typeCount = 2;
			normalized = false;
		}
		else if (DT_FLOAT3 == eType)
		{
			type = GL_FLOAT;
			typeCount = 3;
			normalized = false;
		}
		else if (DT_FLOAT4 == eType)
		{
			type = GL_FLOAT;
			typeCount = 4;
			normalized = false;
		}
		else if (DT_SHORT2 == eType)
		{
			type = GL_SHORT;
			typeCount = 2;
			normalized = false;
		}
		else if (DT_SHORT4 == eType)
		{
			type = GL_SHORT;
			typeCount = 4;
			normalized = false;
		}
		else if (DT_UBYTE4 == eType)
		{
			type = GL_UNSIGNED_BYTE;
			typeCount = 4;
			normalized = false;
		}
		else if (DT_UBYTE4N == eType)
		{
			type = GL_UNSIGNED_BYTE;
			typeCount = 4;
			normalized = true;
		}
		else
		{
			ASSERT(false);
		}
	}

	void GLESMapping::GetGLESFilter(Filter type,GLenum& minFilter,GLenum& magFilter)
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


	GLint GLESMapping::convertCompareFunction( CompareFunction func )
	{
		switch(func)
		{
		case CMPF_ALWAYS_FAIL:
			return GL_NEVER;
		case CMPF_ALWAYS_PASS:
			return GL_ALWAYS;
		case CMPF_LESS:
			return GL_LESS;
		case CMPF_LESS_EQUAL:
			return GL_LEQUAL;
		case CMPF_EQUAL:
			return GL_EQUAL;
		case CMPF_NOT_EQUAL:
			return GL_NOTEQUAL;
		case CMPF_GREATER_EQUAL:
			return GL_GEQUAL;
		case CMPF_GREATER:
			return GL_GREATER;
		};
		// To keep compiler happy
		return GL_ALWAYS;
	}

	GLint GLESMapping::convertStencilOp( StencilOperation op, bool invert /*= false*/ )
	{
		switch(op)
		{
		case SOP_KEEP:
			return GL_KEEP;
		case SOP_ZERO:
			return GL_ZERO;
		case SOP_REPLACE:
			return GL_REPLACE;
		case SOP_INCREMENT:
			return invert ? GL_DECR : GL_INCR;
		case SOP_DECREMENT:
			return invert ? GL_INCR : GL_DECR;
		case SOP_INCREMENT_WRAP:
			return invert ? GL_DECR_WRAP : GL_INCR_WRAP;
		case SOP_DECREMENT_WRAP:
			return invert ? GL_INCR_WRAP : GL_DECR_WRAP;
		case SOP_INVERT:
			return GL_INVERT;
		};
		// to keep compiler happy
		return SOP_KEEP;
	}


}


