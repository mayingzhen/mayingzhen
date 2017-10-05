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

	void VulkanVertexDeclaration::RT_StreamComplete()
	{
		m_bindingDescriptions.resize(1);

		//VkVertexInputBindingDescription bindingDesc;
		m_bindingDescriptions[0].binding = 0/*VERTEX_BUFFER_BIND_ID*/;
		m_bindingDescriptions[0].stride = this->GetStreanmStride(0);
		m_bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		m_attributeDescriptions.resize(this->GetElementCount(0));

		for (UINT i = 0; i < this->GetElementCount(0); ++i)
		{
			const VertexElement& element = this->GetElement(0,i);
			m_attributeDescriptions[i].location = i/*VulkanMapping::GetD3DDeclUsage(element.Usage)*/;
			m_attributeDescriptions[i].binding = 0/*element.UsageIndex*/;
			m_attributeDescriptions[i].format = VulkanMapping::GetDeclType(element.Type);
			m_attributeDescriptions[i].offset = element.Offset;
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


