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
		virtual	void SetVertexBuffer(int index, VertexBuffer* pVB, UINT nOffSet);

		virtual void SetTechnique(Technique* pTech);

		virtual void DrawIndex(UINT nIndexStart, UINT nIndexCount, UINT nInstanceCount, PRIMITIVE_TYPE ePrType);

	public:
		VkCommandBuffer m_vkCmdBuffer;
		VkCommandPool m_vkCmdPool;

		VulkanRenderPass* m_pRenderPass = NULL;

		enum 
		{
			MAX_VB = 6,
		};
	
		IndexBuffer* m_pPreIB = NULL;
		VertexBuffer* m_pPreVB[MAX_VB] = { NULL };
		UINT m_preVBOffset[MAX_VB] = { 0 };
		VulkanPipeline* m_pPrePipeline = NULL;
	};


}


