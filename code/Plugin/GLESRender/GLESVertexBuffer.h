#ifndef  _GLESVertexBuffer__H__
#define  _GLESVertexBuffer__H__

namespace ma
{

	class GLESRENDER_API GLESVertexBuffer : public VertexBuffer
	{

	public:
		GLESVertexBuffer(void* pData = NULL, int nsize = 0, int nStride = 0, USAGE Usgae = USAGE_NO);

		~GLESVertexBuffer();

		virtual void*	Lock(int iOffsetBytes, int iLockSize, LOCK LockFlags);

		virtual void	Unlock();

		virtual void	Active();

		GLuint			GetVertexBuffer(); 
		
	private:
		GLuint			m_hVertexBufffer;
	};

}

#endif