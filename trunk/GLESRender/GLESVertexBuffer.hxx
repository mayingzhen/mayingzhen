#include "GLESVertexBuffer.h"

namespace ma
{
	GLESVertexBuffer::GLESVertexBuffer()
	{
	}

	GLESVertexBuffer::~GLESVertexBuffer()
	{
	  
	}


	void * GLESVertexBuffer::Lock(int iOffsetBytes, int iLockSize, int LockFlag)
	{
		void * pData = NULL;


		return pData;
	}

	void GLESVertexBuffer::Unlock()
	{

	}

	void GLESVertexBuffer::Bind(void* Data, int size, UINT nVertexType, USAGE Usgae)
	{
		glGenBuffers(1,& m_hVertexBufffer);
		glBindBuffer(GL_ARRAY_BUFFER,m_hVertexBufffer);
		glBufferData(GL_ARRAY_BUFFER,size,Data,GL_STATIC_DRAW);

		m_Usage = Usgae;
		m_Size = size;
	}

}
