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

	void	GLESMapping::GetGLESDeclType(DECL_USAGE Usage, GLenum type, GLint& typeCount,GLboolean& normalized)
	{
		if (Usage == DU_POSITION)
		{
			typeCount = 3;
			type = GL_FLOAT;
			normalized = false;
		}
		else if (Usage == DU_TEXCOORD)
		{
			typeCount = 2;
			type = GL_FLOAT;
			normalized = false;
		}
		else if (Usage == DU_COLOR)
		{
			typeCount = 4;
			type = GL_FLOAT;
			normalized = false;
		}
		else if (Usage == DU_NORMAL)
		{
			typeCount = 3;
			type = GL_FLOAT;
			normalized = false;
		}
		else if (Usage == DU_BLENDINDICES)
		{
			typeCount = 4;
			type = GL_UNSIGNED_BYTE;
			normalized = false;
		}
		else if (Usage == DU_BLENDWEIGHT)
		{
			typeCount = 4;
			type = GL_UNSIGNED_BYTE;
			normalized = true;
		}

	}


}
