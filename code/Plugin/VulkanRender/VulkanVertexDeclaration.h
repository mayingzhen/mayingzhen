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

		static void Clear();

	public:
		//IVulkanInputLayout*	m_pImpl;
		VkPipelineVertexInputStateCreateInfo m_inputState;
		std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
	};

}

