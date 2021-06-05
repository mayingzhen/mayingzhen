#pragma once

#import <Metal/Metal.h>

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
        
        virtual void    RT_UpdateData(uint32_t nOffset, uint8_t* pData, uint32_t nSize);

	protected:
		id<MTLBuffer>	mMetalVertexBuffer;
	};

}
