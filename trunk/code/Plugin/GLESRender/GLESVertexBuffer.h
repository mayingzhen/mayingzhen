#ifndef  _GLESVertexBuffer__H__
#define  _GLESVertexBuffer__H__

#include "GLESMapping.h"
#include "Engine/RenderSystem/VertexBuffer.h"

namespace ma
{

	class GLESRENDER_API GLESVertexBuffer : public VertexBuffer
	{

	public:
		GLESVertexBuffer(void* pData = NULL, int nsize = 0, int nStride = 0, USAGE Usgae = USAGE_STATIC);

		~GLESVertexBuffer();

		virtual void*	Lock(int iOffsetBytes, int iLockSize, int LockFlags);

		virtual void	Unlock();

		virtual void	Active();

		GLuint	GetVertexBuffer() {return m_hVertexBufffer;}
		
	private:
		GLuint m_hVertexBufffer;
	};

}

#endif