#include "GLESIndexBuffer.h"

namespace ma
{
	GLESIndexBuffer::GLESIndexBuffer()
	{
	}

	GLESIndexBuffer::~GLESIndexBuffer()
	{
	}

	void * GLESIndexBuffer::Lock(int iOffsetBytes, int iLockSize, int LockFlag)
	{
		void * pData = NULL;


		return pData;
	}

	void GLESIndexBuffer::Unlock()
	{
	  
	}

	void GLESIndexBuffer::Bind(void* Data, int size, INDEX_TYPE eIndexType, USAGE Usgae)
	{
		glGenBuffers(1,&m_hIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_hIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,Data,GL_STATIC_DRAW);
		m_eType = eIndexType;
		m_eUsage = Usgae;
		m_nSize = size;
	}



}
