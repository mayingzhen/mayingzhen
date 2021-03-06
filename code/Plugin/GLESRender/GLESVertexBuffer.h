#ifndef  _GLESVertexBuffer__H__
#define  _GLESVertexBuffer__H__

namespace ma
{

	class GLESVertexBuffer : public VertexBuffer
	{

	public:
		GLESVertexBuffer();

		~GLESVertexBuffer();

		virtual void*	LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlags);

		virtual void	UnlockImpl();

		virtual void	RT_StreamComplete();

		GLuint			GetVertexBuffer(); 
		
	private:
		GLuint			m_hVertexBufffer;
	};

}

#endif