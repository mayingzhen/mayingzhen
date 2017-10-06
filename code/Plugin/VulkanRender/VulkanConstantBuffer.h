
#pragma once

namespace ma
{

	/// Hardware constant buffer.
	class VulkanConstantBuffer : public ConstantBuffer
	{
	public:
		VulkanConstantBuffer();
		virtual ~VulkanConstantBuffer();

		/// Release buffer.
		virtual void Release();

		/// Set a generic parameter and mark buffer dirty.
		void SetParameter(unsigned offset, unsigned size, const void* data);

		bool Create();

	public:
		VkBuffer m_buf;
		VkDeviceMemory m_mem;
		VkDescriptorBufferInfo m_descriptor;

		BYTE* m_mapped = nullptr;
	};

}

