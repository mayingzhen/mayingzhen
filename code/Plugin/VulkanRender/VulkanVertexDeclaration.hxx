#include "VulkanVertexDeclaration.h"
#include "VulkanShaderProgram.h"

namespace ma
{
	VulkanVertexDeclaration::VulkanVertexDeclaration()
	{

	}

	VulkanVertexDeclaration::~VulkanVertexDeclaration()
	{

	}

	#define VERTEX_BUFFER_BIND_ID 0
	#define INSTANCE_BUFFER_BIND_ID 1

	void VulkanVertexDeclaration::RT_StreamComplete()
	{
		VkVertexInputBindingDescription bindingDesc;
		bindingDesc.binding = VERTEX_BUFFER_BIND_ID;
		bindingDesc.stride = this->GetStreanmStride(VERTEX_BUFFER_BIND_ID);
		bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		m_bindingDescriptions.push_back(bindingDesc);

		for (UINT i = 0; i < this->GetElementCount(VERTEX_BUFFER_BIND_ID); ++i)
		{
			const VertexElement& element = this->GetElement(VERTEX_BUFFER_BIND_ID,i);

			VkVertexInputAttributeDescription arrtDesc;
			arrtDesc.location = i;
			arrtDesc.binding = VERTEX_BUFFER_BIND_ID;
			arrtDesc.format = VulkanMapping::GetDeclType(element.Type);
			arrtDesc.offset = element.Offset;

			m_attributeDescriptions.push_back(arrtDesc);
		}

		if (m_arrStreamElement.size() > 1)
		{
			VkVertexInputBindingDescription bindingDesc;
			bindingDesc.binding = INSTANCE_BUFFER_BIND_ID;
			bindingDesc.stride = this->GetStreanmStride(INSTANCE_BUFFER_BIND_ID);
			bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
			m_bindingDescriptions.push_back(bindingDesc);

			for (UINT i = 0; i < this->GetElementCount(INSTANCE_BUFFER_BIND_ID); ++i)
			{
				const VertexElement& element = this->GetElement(INSTANCE_BUFFER_BIND_ID, i);

				VkVertexInputAttributeDescription arrtDesc;
				arrtDesc.location = i + this->GetElementCount(VERTEX_BUFFER_BIND_ID);
				arrtDesc.binding = INSTANCE_BUFFER_BIND_ID;
				arrtDesc.format = VulkanMapping::GetDeclType(element.Type);
				arrtDesc.offset = element.Offset;

				m_attributeDescriptions.push_back(arrtDesc);
			}
		}

		m_inputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		m_inputState.pNext = NULL;
		m_inputState.flags = 0;
		m_inputState.vertexBindingDescriptionCount = static_cast<uint32_t>(m_bindingDescriptions.size());
		m_inputState.pVertexBindingDescriptions = m_bindingDescriptions.data();
		m_inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_attributeDescriptions.size());
		m_inputState.pVertexAttributeDescriptions = m_attributeDescriptions.data();

	}

}


