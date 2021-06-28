#pragma once


namespace ma
{

	class VulkanRenderCommand : public RenderCommand
	{
	public:

		VulkanRenderCommand();

		~VulkanRenderCommand();

		void Create(uint32_t queueFamilyIndex);

		virtual	void Begin();

		virtual void End();

		virtual void SetIndexBuffer(IndexBuffer* pIB);

		virtual	void SetVertexBuffer(int index, VertexBuffer* pVB, uint32_t nOffSet);

		virtual void SetTechnique(Technique* pTech);

		virtual void SetViewPort(const Rectangle& viewPort);

		virtual void SetScissor(const Rectangle& viewPort);

		virtual	void SetDepthBounds(float fDepthNear, float fDepthFar);

		virtual void SetDepthBias(float fConstant, float fSlope);

		virtual void DrawIndex(uint32_t nIndexStart, uint32_t nIndexCount, uint32_t nVertexStart, uint32_t nInstanceCount);

		virtual void Draw(uint32_t nVertexStart, uint32_t nVertexCount, uint32_t nInstanceCount);

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
		uint32_t m_preVBOffset[MAX_VB] = { 0 };
		VkPipeline m_prePipeline;
	};

	class VulkanComputeCommand : public ComputeCommand
	{
	public:

		VulkanComputeCommand();

		~VulkanComputeCommand();

		void Create();

		virtual	void Begin();

		virtual void End();

		virtual void SetTechnique(Technique* pTech);

		virtual void SetStorgeBuffer(VertexBuffer* pBuffer);

		virtual void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

	private:
		VkCommandBuffer m_vkCmdBuffer;
		VkCommandPool m_vkCmdPool;

		VkPipeline m_prePipeline;

		vks::Buffer m_storageBuffer;
	};
}



