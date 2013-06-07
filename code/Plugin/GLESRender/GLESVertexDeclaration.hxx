#include "GLESVertexDeclaration.h"

namespace ma
{
	//#define OFFSET(x) ((char *)NULL + (x))

	GLESVertexDeclaration::GLESVertexDeclaration()
	{
	}

	GLESVertexDeclaration::~GLESVertexDeclaration()
	{
	}

// 	void GLESVertexDeclaration::Bind(UINT nVertexStart/*,ShaderProgram* pEffect*/)
// 	{
// 		int vertexStartByte = nVertexStart * GetStreanmStride();
// 
// 		int nSteam = GetElementCount();
// 		for (int i = 0; i < nSteam; ++i)
// 		{
// 			const VertexElement& ve = GetElement(i);
// 
// 			GLint typeCount; 
// 			GLenum type;
// 			GLboolean normalized; = false;
// 			GLESMapping::GetGLESType(ve.Usage,type,typeCount,normalized);
// 
// 			void* pBufferData = OFFSET( vertexStartByte + ve.Offset );		
// 
// 			GL_ASSERT( glVertexAttribPointer( ve.Usage,typeCount,type,normalized, GetStreanmStride(), pBufferData ) );
// 
// 			GL_ASSERT( glEnableVertexAttribArray(attrib) );
// 		}
// 	}

// 	void GLESVertexDeclaration::UnBind()
// 	{
// 
// 	}


}
