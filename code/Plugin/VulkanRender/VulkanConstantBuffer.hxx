
#include "VulkanConstantBuffer.h"
#include "VulkanRenderDevice.h"


namespace ma
{
	VulkanConstantBuffer::VulkanConstantBuffer() 
	{
	}

	VulkanConstantBuffer::~VulkanConstantBuffer()
	{
		Release();
	}

	void VulkanConstantBuffer::Release()
	{
// 		vks::VulkanDevice* vulkanDevice = GetVulkanDevice();
// 		vkDestroyBuffer(vulkanDevice->logicalDevice, m_buf, nullptr);
// 
// 		vkFreeMemory(vulkanDevice->logicalDevice, m_mem, nullptr);
	}


	bool VulkanConstantBuffer::Create()
	{
		vks::VulkanDevice* vulkanDevice = GetVulkanDevice();

		/* VULKAN_KEY_START */
		VkBufferCreateInfo buf_info = {};
		buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buf_info.pNext = NULL;
		buf_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		buf_info.size = this->GetSize();
		buf_info.queueFamilyIndexCount = 0;
		buf_info.pQueueFamilyIndices = NULL;
		buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		buf_info.flags = 0;
		VkResult res = vkCreateBuffer(vulkanDevice->logicalDevice, &buf_info, NULL, &m_buf);
		assert(res == VK_SUCCESS);

		VkMemoryRequirements mem_reqs;
		vkGetBufferMemoryRequirements(vulkanDevice->logicalDevice, m_buf, &mem_reqs);

		VkMemoryAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.pNext = NULL;
		alloc_info.memoryTypeIndex = 0;

		alloc_info.allocationSize = mem_reqs.size;
		alloc_info.memoryTypeIndex = vulkanDevice->getMemoryType(mem_reqs.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		res = vkAllocateMemory(vulkanDevice->logicalDevice, &alloc_info, NULL, &m_mem);
		assert(res == VK_SUCCESS);

		res = vkBindBufferMemory(vulkanDevice->logicalDevice, m_buf, m_mem, 0);
		assert(res == VK_SUCCESS);

		VK_CHECK_RESULT(vkMapMemory(vulkanDevice->logicalDevice, m_mem, 0, this->GetSize(), 0, (void **)&m_mapped));

		memset(m_mapped, 0, this->GetSize());

		m_descriptor.offset = 0;
		m_descriptor.buffer = m_buf;
		m_descriptor.range = this->GetSize();

		return true;
	}

	void VulkanConstantBuffer::SetParameter(unsigned offset, unsigned size, const void* data)
	{
		vks::VulkanDevice* vulkanDevice = GetVulkanDevice();

		memcpy(m_mapped + offset, data, size);
	}

	void VulkanConstantBuffer::Clear()
	{
		//g_mapConstantBufferPool.clear();
	}

	RefPtr<VulkanConstantBuffer> CreateConstantBuffer(ShaderType type, unsigned index, unsigned size)
	{
// 		// Ensure that different shader types and index slots get unique buffers, even if the size is same
// 		unsigned key = type | (index << 1) | (size << 4);
// 		map<unsigned, RefPtr<ConstantBuffer> >::iterator i = g_mapConstantBufferPool.find(key);
// 		if (i != g_mapConstantBufferPool.end())
// 		{
// 			return i->second.get();
// 		}
// 		else
// 		{
			RefPtr<VulkanConstantBuffer> newConstantBuffer(new VulkanConstantBuffer());
			newConstantBuffer->SetSize(size);
			newConstantBuffer->SetBound(index);
			newConstantBuffer->Create();
			return newConstantBuffer;
// 		}
		return NULL;
	}

}
