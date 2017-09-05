
#include "VulkanConstantBuffer.h"
#include "VulkanRenderDevice.h"


namespace ma
{
	std::map<unsigned, RefPtr<ConstantBuffer> > g_mapConstantBufferPool;

	ConstantBuffer::ConstantBuffer() 
	{
		//m_pVulkanBuffer = NULL;
		m_bDirty = false;
	}

	ConstantBuffer::~ConstantBuffer()
	{
		Release();
	}

	void ConstantBuffer::Release()
	{
// 		if (m_pVulkanBuffer)
// 		{
// 			SAFE_RELEASE(m_pVulkanBuffer);
// 		}

		//m_shadowData.clear();
	}

// 	bool memory_type_from_properties(struct sample_info &info, uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex) {
// 		// Search memtypes to find first index with those properties
// 		for (uint32_t i = 0; i < info.memory_properties.memoryTypeCount; i++) {
// 			if ((typeBits & 1) == 1) {
// 				// Type is available, does it match user properties?
// 				if ((info.memory_properties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) {
// 					*typeIndex = i;
// 					return true;
// 				}
// 			}
// 			typeBits >>= 1;
// 		}
// 		// No memory types matched, return failure
// 		return false;
//	}

	bool ConstantBuffer::SetSize(unsigned size)
	{
		vks::VulkanDevice* vulkanDevice = GetVulkanDevice();

		// Round up to next 16 bytes
		size += 15;
		size &= 0xfffffff0;

		m_bDirty = false;

		/* VULKAN_KEY_START */
		VkBufferCreateInfo buf_info = {};
		buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buf_info.pNext = NULL;
		buf_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		buf_info.size = size;
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
		//bool pass = memory_type_from_properties(info, mem_reqs.memoryTypeBits,
		//	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		//	&alloc_info.memoryTypeIndex);
		//assert(pass && "No mappable, coherent memory");

		res = vkAllocateMemory(vulkanDevice->logicalDevice, &alloc_info, NULL, &m_mem);
		assert(res == VK_SUCCESS);

		return true;
	}

	void ConstantBuffer::SetParameter(unsigned offset, unsigned size, const void* data)
	{
		vks::VulkanDevice* vulkanDevice = GetVulkanDevice();

		uint8_t *pData;
		VkResult res = vkMapMemory(vulkanDevice->logicalDevice, m_mem, offset, size, 0, (void **)&pData);
		assert(res == VK_SUCCESS);

		memcpy(pData, data, size);

		vkUnmapMemory(vulkanDevice->logicalDevice, m_mem);

		m_bDirty = true;
	}

	void ConstantBuffer::SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data)
	{
		vks::VulkanDevice* vulkanDevice = GetVulkanDevice();

		uint8_t *pData;
		VkResult res = vkMapMemory(vulkanDevice->logicalDevice, m_mem, offset, rows * sizeof(Vector3), 0, (void **)&pData);
		assert(res == VK_SUCCESS);

		memcpy(pData, data, rows * sizeof(Vector3));

		vkUnmapMemory(vulkanDevice->logicalDevice, m_mem);

		m_bDirty = true;
	}

	void ConstantBuffer::Apply()
	{
		vks::VulkanDevice* vulkanDevice = GetVulkanDevice();

		if (m_bDirty)
		{
			VkResult res = vkBindBufferMemory(vulkanDevice->logicalDevice, m_buf, m_mem, 0);
			assert(res == VK_SUCCESS);

			m_bDirty = false;
		}
	}

	void ConstantBuffer::Clear()
	{
		//g_mapConstantBufferPool.clear();
	}

	RefPtr<ConstantBuffer> CreateConstantBuffer(ShaderType type, unsigned index, unsigned size)
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
// 			RefPtr<ConstantBuffer> newConstantBuffer(new ConstantBuffer());
// 			newConstantBuffer->SetSize(size);
// 			g_mapConstantBufferPool[key] = newConstantBuffer;
// 			return newConstantBuffer;
// 		}
		return NULL;
	}

}
