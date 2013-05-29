#include "GLESVertexBuffer.h"

namespace ma
{
	GLESVertexBuffer::GLESVertexBuffer(void* pData, int nsize, int nStride, USAGE Usgae)
		:VertexBuffer(pData,nsize,nStride,Usgae)
	{
		m_hVertexBufffer = 0;
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

	void GLESVertexBuffer::Active()
	{
		GLenum glUsage = GLESMapping::GetGLESUSage(m_Usage);
		glGenBuffers(1,& m_hVertexBufffer);
		glBindBuffer(GL_ARRAY_BUFFER,m_hVertexBufffer);
		glBufferData(GL_ARRAY_BUFFER,m_Size,m_pData,glUsage);
	}

}
