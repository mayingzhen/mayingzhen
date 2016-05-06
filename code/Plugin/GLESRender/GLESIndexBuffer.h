#ifndef  _GLESIndexBuffer__H__
#define  _GLESIndexBuffer__H__


namespace ma
{

	class GLESIndexBuffer : public IndexBuffer
	{

	public:
		GLESIndexBuffer();
		
		~GLESIndexBuffer();

		virtual void*	LockImpl(int iOffsetBytes, int uiLockSize, LOCK LockFlags);
		
		virtual void	UnlockImpl();

		virtual void	RT_StreamComplete();

		GLuint			GetIndexBuffer();

	protected:
		GLuint			m_hIndexBuffer;
	};

}

#endif