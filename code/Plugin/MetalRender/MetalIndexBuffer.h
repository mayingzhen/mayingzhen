#pragma once


namespace ma
{

	class MetalIndexBuffer : public IndexBuffer
	{
	public:
		MetalIndexBuffer();

		~MetalIndexBuffer();
	
		id<MTLBuffer> GetMetalIndexBuffer() ;

	private:
		virtual void*	LockImpl(int iOffsetBytes, int uiLockSize, LOCK LockFlags);
		
		virtual void	UnlockImpl();

		virtual void	RT_StreamComplete();

	protected:
        
		id<MTLBuffer>   mMetalIndexBuffer;
	};

}

