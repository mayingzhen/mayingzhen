#pragma once


namespace ma
{

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer();

		~VulkanVertexBuffer();

		//IVulkanBuffer* GetD3DVertexBuffer();

	private:

		virtual void*	LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlags);
		
		virtual void	UnlockImpl();

		virtual void	RT_StreamComplete();

	public:
		vks::Buffer vertexBuffer;
	};

}
