#include "GLESVertexDeclaration.h"
#include "Effect.h"

namespace ma
{
	#define OFFSET(x) ((char *)NULL + (x))

	GLESVertexDeclaration::GLESVertexDeclaration()
	{
	}

	GLESVertexDeclaration::~GLESVertexDeclaration()
	{
	}

	void GLESVertexDeclaration::Active(UINT nVertexStart,Effect* pEffect)
	{
		int vertexStartByte = nVertexStart * GetStreanmStride();

		int nSteam = GetElementCount();
		for (int i = 0; i < nSteam; ++i)
		{
			const VertexElement& ve = GetElement(i);

			VertexAttribute attrib;
			GLint typeCount = 0;
			GLenum type = GL_FLOAT;
			GLboolean normalized = false;
			if (ve.Usage == DU_POSITION)
			{
				attrib = pEffect->getVertexAttribute(VERTEX_ATTRIBUTE_POSITION_NAME);
				typeCount = 3;
				type = GL_FLOAT;
				normalized = false;
			}
			else if (ve.Usage == DU_TEXCOORD)
			{
				attrib = pEffect->getVertexAttribute(VERTEX_ATTRIBUTE_TEXCOORD0_NAME);
				typeCount = 2;
				type = GL_FLOAT;
				normalized = false;
			}
			else if (ve.Usage == DU_NORMAL)
			{
				typeCount = 3;
				type = GL_FLOAT;
				normalized = false;
			}
			else if (ve.Usage == DU_BLENDINDICES)
			{
				attrib = pEffect->getVertexAttribute(VERTEX_ATTRIBUTE_BLENDINDICES_NAME);
				typeCount = 4;
				type = GL_UNSIGNED_BYTE;
				normalized = false;
			}
			else if (ve.Usage == DU_BLENDWEIGHT)
			{
				attrib = pEffect->getVertexAttribute(VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME);
				typeCount = 4;
				type = GL_UNSIGNED_BYTE;
				normalized = true;
			}

			void* pBufferData = OFFSET( vertexStartByte + ve.Offset );		

			GL_ASSERT( glVertexAttribPointer(attrib,typeCount,type,normalized,GetStreanmStride(), pBufferData ) );
			//GL_CHECK_ERROR;

			GL_ASSERT( glEnableVertexAttribArray(attrib) );
			//GL_CHECK_ERROR;
		}
	}


}
