#pragma once


namespace ma
{
    class MetalRenderCommand;
    
	class MetalRenderPass : public RenderPass
	{
	public:
		MetalRenderPass();

		~MetalRenderPass();

        virtual void Create();
        
        virtual void Begine();
        
        virtual void End();
        
        virtual RenderCommand* GetThreadCommand(uint32_t nIndex, RenderListType eRLType);
        
	public:

		MTLRenderPassDescriptor*  m_pass_desc;

        id<MTLCommandBuffer> m_curCommand = 0;
        
        id<MTLParallelRenderCommandEncoder> m_parallelRCE;
        
		//std::vector<VkClearValue> m_vecClearValues;
        std::vector< RefPtr<MetalRenderCommand> > m_arrRenderCommand;
	};

} 




