#include "GLESMapping.h"

namespace ma
{

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

}
