#ifndef  _GLESIndexBuffer__H__
#define  _GLESIndexBuffer__H__


#include "GLESMapping.h"
#include "Engine/RenderSystem/IndexBuffer.h"

namespace ma
{

	class GLESRENDER_API GLESIndexBuffer : public IndexBuffer
	{

	public:
		GLESIndexBuffer();
		~GLESIndexBuffer();

		void* Lock(int iOffsetBytes, int uiLockSize, int LockFlags);
		void Unlock();

		virtual void	Bind(void* Data, int size, INDEX_TYPE eIndexType, USAGE Usgae = USAGE_STATIC);

		GLuint	GetIndexBuffer(){return m_hIndexBuffer;}
	protected:
		GLuint m_hIndexBuffer;
	};

}

#endif