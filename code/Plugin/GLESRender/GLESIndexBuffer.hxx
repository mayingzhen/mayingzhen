#include "GLESIndexBuffer.h"

namespace ma
{
	GLESIndexBuffer::GLESIndexBuffer()
	{
		m_hIndexBuffer = 0;
	}

	GLESIndexBuffer::~GLESIndexBuffer()
	{
	}

	void * GLESIndexBuffer::LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
// 		GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_hIndexBuffer) );
// 		GL_ASSERT( glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_Size,NULL,GL_STATIC_DRAW) );
// 		void* pLock = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_WRITE_ONLY) ; 
// 		ASSERT(glGetError() == GL_NO_ERROR); 
// 		return pLock;
		return NULL;
	}

	void GLESIndexBuffer::UnlockImpl()
	{
// 		GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_hIndexBuffer) );
// 		GL_ASSERT( glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) );	
	}

	void GLESIndexBuffer::RT_StreamComplete()
	{
		GLenum glUsage = GLESMapping::GetGLESBufferUSage(m_Usage);

		GL_ASSERT( glGenBuffers(1,& m_hIndexBuffer) );
		GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_hIndexBuffer) );
		GL_ASSERT( glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_Size,m_pData,glUsage) );

		if (!m_bShadowData)
		{
			FreeData();
		}
	}

	GLuint	GLESIndexBuffer::GetIndexBuffer()
	{
		return m_hIndexBuffer;
	}

}

