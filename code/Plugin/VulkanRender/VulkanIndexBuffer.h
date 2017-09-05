#pragma once

namespace ma
{

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer();

		~VulkanIndexBuffer();
	
		//IVulkanBuffer * GetD3DIndexBuffer() ;

	private:
		virtual void*	LockImpl(int iOffsetBytes, int uiLockSize, LOCK LockFlags);
		
		virtual void	UnlockImpl();

		virtual void	RT_StreamComplete();

	public:
	
		//IVulkanBuffer*    mVulkanIndexBuffer;	
		vks::Buffer indexBuffer;
	};

}

