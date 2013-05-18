#ifndef  _GLESVertexBuffer__H__
#define  _GLESVertexBuffer__H__

#include "GLESMapping.h"
#include "Engine/RenderSystem/VertexBuffer.h"

namespace ma
{

	class GLESRENDER_API GLESVertexBuffer : public VertexBuffer
	{

	public:
		GLESVertexBuffer();

		~GLESVertexBuffer();

		void*	Lock(int iOffsetBytes, int iLockSize, int LockFlags);

		void	Unlock();

		virtual void	Bind(void* Data, int size, UINT nVertexType, USAGE Usgae = USAGE_STATIC);

		GLuint	GetVertexBuffer() {return m_hVertexBufffer;}
		
	private:
		GLuint m_hVertexBufffer;
	};

}

#endif