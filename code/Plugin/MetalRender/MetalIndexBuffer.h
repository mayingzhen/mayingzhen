#pragma once

#import <Metal/Metal.h>

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
        
        virtual void    RT_UpdateData(uint32_t nOffset, uint8_t* pData, uint32_t nSize);

	protected:
        
		id<MTLBuffer>   mMetalIndexBuffer;
	};

}

