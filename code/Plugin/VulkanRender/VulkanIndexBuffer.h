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

		virtual void	RT_UpdateData(uint32_t nOffset, uint8_t* pData, uint32_t nSize);

	public:

		vks::Buffer		m_indexBuffer;
	};

}

