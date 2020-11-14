#pragma once


namespace ma
{

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer();

		~VulkanVertexBuffer();

	private:

		virtual void*	LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlags);
		
		virtual void	UnlockImpl();

		virtual void	RT_StreamComplete();

		virtual void	RT_UpdateData(uint32_t nOffset, uint8_t* pData, uint32_t nSize);

	public:
		vks::Buffer		m_vertexBuffer;
	};

}
