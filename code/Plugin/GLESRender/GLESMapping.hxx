#include "GLESMapping.h"

namespace ma
{

	GLenum GLESMapping::GetGLESUSage(USAGE Usage)
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

	void	GLESMapping::GetGLESDeclType(DECL_USAGE Usage, GLenum& type, 
		GLint& typeCount,GLboolean& normalized,std::string& name)
	{
		if (Usage == DU_POSITION)
		{
			typeCount = 3;
			type = GL_FLOAT;
			normalized = false;
			name = VERTEX_ATTRIBUTE_POSITION_NAME;
		}
		else if (Usage == DU_TEXCOORD0)
		{
			typeCount = 2;
			type = GL_FLOAT;
			normalized = false;
			name = VERTEX_ATTRIBUTE_TEXCOORD0_NAME;
		}
		else if (Usage == DU_TEXCOORD1)
		{
			typeCount = 2;
			type = GL_FLOAT;
			normalized = false;
			name = VERTEX_ATTRIBUTE_TEXCOORD1_NAME;
		}
		else if (Usage == DU_COLOR)
		{
			typeCount = 4;
			type = GL_FLOAT;
			normalized = false;
			name = VERTEX_ATTRIBUTE_COLOR_NAME;
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

	GLenum GLESMapping::GetGLESFormat(FORMAT format)
	{
		switch (format)
		{
		case FMT_R8G8B8:
		case FMT_R5G6B5:
		case FMT_R3G3B2:
			return GL_RGB;

		case FMT_A8R8G8B8:
		case FMT_X8R8G8B8:
			return GL_RGBA;

		case FMT_A8:
			return GL_ALPHA;

		default:
			ASSERT(0 && "Invalid Format.");
			return GL_RGB;
		}
	}

}


