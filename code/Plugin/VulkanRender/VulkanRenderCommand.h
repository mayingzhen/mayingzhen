#pragma once


namespace ma
{

	class VulkanRenderCommand : public RenderCommand
	{
	public:

		VulkanRenderCommand();

		~VulkanRenderCommand();

		void Create(UINT queueFamilyIndex);

		virtual	void Begin();
		virtual void End();

		virtual void SetIndexBuffer(IndexBuffer* pIB);
		virtual	void SetVertexBuffer(int index, VertexBuffer* pVB);

		virtual void DrawRenderable(const Renderable* pRenderable, Technique* pTech);

	public:
		VkCommandBuffer m_vkCmdBuffer;
		VkCommandPool m_vkCmdPool;

		VulkanRenderPass* m_pRenderPass;
	};


}


