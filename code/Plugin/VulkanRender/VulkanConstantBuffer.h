
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
		VkBuffer m_buf = VK_NULL_HANDLE;
		VkDeviceMemory m_mem = VK_NULL_HANDLE;
		VkDescriptorBufferInfo m_descriptor;

		uint8_t* m_mapped = nullptr;
	};

}

