
#include "VulkanConstantBuffer.h"


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

		m_shadowData.clear();
	}

	bool ConstantBuffer::SetSize(unsigned size)
	{
//		Release();

// 		if (!size)
// 		{
// 			LogError("Can not create zero-sized constant buffer");
// 			return false;
// 		}
// 
// 		// Round up to next 16 bytes
// 		size += 15;
// 		size &= 0xfffffff0;
// 
// 		m_bDirty = false;
// 		m_shadowData.resize(size);// = new unsigned char[size_];
// 		memset(&m_shadowData[0], 0, size);
// 
// 		//if (graphics_)
// 		{
// 			Vulkan_BUFFER_DESC bufferDesc;
// 			memset(&bufferDesc, 0, sizeof bufferDesc);
// 
// 			bufferDesc.ByteWidth = size;
// 			bufferDesc.BindFlags = Vulkan_BIND_CONSTANT_BUFFER;
// 			bufferDesc.CPUAccessFlags = 0;
// 			bufferDesc.Usage = Vulkan_USAGE_DEFAULT;
// 
// 			GetVulkanDxDevive()->CreateBuffer(&bufferDesc, 0, &m_pVulkanBuffer);
// 			ASSERT(m_pVulkanBuffer);
// 			if (!m_pVulkanBuffer)
// 			{
// 				LogError("Failed to create constant buffer");
// 				return false;
// 			}
// 		}

		return true;
	}

	void ConstantBuffer::SetParameter(unsigned offset, unsigned size, const void* data)
	{
		if (offset + size > m_shadowData.size())
			return; // Would overflow the buffer

		memcpy(&m_shadowData[offset], data, size);
		m_bDirty = true;
	}

	void ConstantBuffer::SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data)
	{
		if (offset + rows * 4 * sizeof(float) > m_shadowData.size())
			return; // Would overflow the buffer

		float* dest = (float*)&m_shadowData[offset];
		const float* src = (const float*)data;

		while (rows--)
		{
			*dest++ = *src++;
			*dest++ = *src++;
			*dest++ = *src++;
			++dest; // Skip over the w coordinate
		}

		m_bDirty = true;
	}

	void ConstantBuffer::Apply()
	{
// 		if (m_bDirty && m_pVulkanBuffer)
// 		{
// 			GetVulkanDxDeviveContext()->UpdateSubresource(m_pVulkanBuffer, 0, 0, &m_shadowData[0], 0, 0);
// 			m_bDirty = false;
// 		}
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
