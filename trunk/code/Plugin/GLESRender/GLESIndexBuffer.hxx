#include "GLESIndexBuffer.h"

namespace ma
{
	GLESIndexBuffer::GLESIndexBuffer(void* Data, int size, INDEX_TYPE eIndexType, USAGE Usgae)
		: IndexBuffer(Data,size,eIndexType,Usgae)
	{
		m_hIndexBuffer = 0;
	}

	GLESIndexBuffer::~GLESIndexBuffer()
	{
	}

	void * GLESIndexBuffer::Lock(int iOffsetBytes, int iLockSize, int LockFlag)
	{
		//void * pData = NULL;


		//return pData;
		return m_pData;
	}

	void GLESIndexBuffer::Unlock()
	{
	  
	}

	void GLESIndexBuffer::Active()
	{
		GLenum glUsage = GLESMapping::GetGLESUSage(m_eUsage);

		glGenBuffers(1,& m_hIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_hIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_nSize,m_pData,glUsage);
	}

}

