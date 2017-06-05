#pragma once


namespace ma
{

	class MetalVertexBuffer : public VertexBuffer
	{
	public:
		MetalVertexBuffer();

		~MetalVertexBuffer();

		id<MTLBuffer> GetMetalVertexBuffer();

	private:

		virtual void*	LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlags);
		
		virtual void	UnlockImpl();

		virtual void	RT_StreamComplete();

	protected:
		id<MTLBuffer>	mMetalVertexBuffer;
	};

}
