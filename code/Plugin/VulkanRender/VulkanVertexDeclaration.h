#pragma once

namespace ma
{
	class VulkanVertexDeclaration : public VertexDeclaration
	{

	public: 
		VulkanVertexDeclaration();
		
		~VulkanVertexDeclaration();

	public:

		virtual void RT_StreamComplete();

	public:
		VkPipelineVertexInputStateCreateInfo m_inputState;
		std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
	};

}

