#pragma once

namespace ma
{

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer();

		~VulkanIndexBuffer();

	private:
		virtual void*	LockImpl(int iOffsetBytes, int uiLockSize, LOCK LockFlags);
		
		virtual void	UnlockImpl();

		virtual void	RT_StreamComplete();

	public:

		vks::Buffer		m_indexBuffer;
	};

}

