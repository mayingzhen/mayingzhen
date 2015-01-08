#include "GLESVertexDeclaration.h"

namespace ma
{
	GLESVertexDeclaration::GLESVertexDeclaration()
	{
	}

	GLESVertexDeclaration::~GLESVertexDeclaration()
	{
	}

	void GLESVertexDeclaration::RT_StreamComplete()
	{
		m_bActive = true;
	}

}
