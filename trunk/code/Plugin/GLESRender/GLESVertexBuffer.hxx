#include "GLESVertexBuffer.h"

namespace ma
{
	GLESVertexBuffer::GLESVertexBuffer(void* pData, int nsize, int nStride, USAGE Usgae)
		:VertexBuffer(pData,nsize,nStride,Usgae)
	{
		m_hVertexBufffer = -1;
		Active();
	}

	GLESVertexBuffer::~GLESVertexBuffer()
	{
	  
	}


	void * GLESVertexBuffer::Lock(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
		glBindBuffer(GL_ARRAY_BUFFER,m_hVertexBufffer);
		glBufferData(GL_ARRAY_BUFFER,m_Size,NULL,GL_STATIC_DRAW);
		return glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
		//return m_pData;
	}

	void GLESVertexBuffer::Unlock()
	{
		glBindBuffer(GL_ARRAY_BUFFER,m_hVertexBufffer);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void GLESVertexBuffer::Active()
	{
		GLenum glUsage = GLESMapping::GetGLESUSage(m_Usage);
		glGenBuffers(1,& m_hVertexBufffer);
		glBindBuffer(GL_ARRAY_BUFFER,m_hVertexBufffer);
		glBufferData(GL_ARRAY_BUFFER,m_Size,m_pData,glUsage);
	}

	GLuint GLESVertexBuffer::GetVertexBuffer()
	{
// 		if (m_hVertexBufffer < 0)
// 		{
// 			Active();
// 		}
		return m_hVertexBufffer;
	}
}
