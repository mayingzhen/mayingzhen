#include "GLESIndexBuffer.h"

namespace ma
{
	GLESIndexBuffer::GLESIndexBuffer(void* Data, int size, INDEX_TYPE eIndexType, USAGE Usgae)
		: IndexBuffer(Data,size,eIndexType,Usgae)
	{
		m_hIndexBuffer = -1;
		Active();
	}

	GLESIndexBuffer::~GLESIndexBuffer()
	{
	}

	void * GLESIndexBuffer::Lock(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_hIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_nSize,NULL,GL_STATIC_DRAW);
		return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_WRITE_ONLY);
		//return m_pData;
	}

	void GLESIndexBuffer::Unlock()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_hIndexBuffer);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);	
	}

	void GLESIndexBuffer::Active()
	{
		GLenum glUsage = GLESMapping::GetGLESUSage(m_eUsage);

		glGenBuffers(1,& m_hIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_hIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_nSize,m_pData,glUsage);
	}

	GLuint	GLESIndexBuffer::GetIndexBuffer()
	{
// 		if (m_hIndexBuffer < 0)
// 		{
// 			Active();
// 		}

		return m_hIndexBuffer;
	}

}

