#include "GLESVertexBuffer.h"

namespace ma
{
	GLESVertexBuffer::GLESVertexBuffer()
	{
		m_hVertexBufffer = 0;
	}

	GLESVertexBuffer::~GLESVertexBuffer()
	{
	  
	}


	void * GLESVertexBuffer::Lock(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
		GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER,m_hVertexBufffer) );
		GL_ASSERT( glBufferData(GL_ARRAY_BUFFER,m_Size,NULL,GL_STATIC_DRAW) );
		void* pLock = glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
		ASSERT(glGetError() == GL_NO_ERROR); 
		return pLock;
	}

	void GLESVertexBuffer::Unlock()
	{
		GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER,m_hVertexBufffer) );
		GL_ASSERT( glUnmapBuffer(GL_ARRAY_BUFFER) );
	}

	void GLESVertexBuffer::RT_StreamComplete()
	{
		GLenum glUsage = GLESMapping::GetGLESUSage(m_Usage);
		GL_ASSERT( glGenBuffers(1,& m_hVertexBufffer) );
		GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER,m_hVertexBufffer) );
		GL_ASSERT( glBufferData(GL_ARRAY_BUFFER,m_Size,m_pData,glUsage) );

		if (m_bNeedFreeData)
		{
			SAFE_DELETE_ARRAY(m_pData);
		}
	}

	GLuint GLESVertexBuffer::GetVertexBuffer()
	{
		return m_hVertexBufffer;
	}
}
