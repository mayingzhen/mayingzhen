#pragma once

#import <Metal/Metal.h>

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
        
        virtual RenderCommand* GetThreadCommand(uint32_t nIndex);
        
	public:

		MTLRenderPassDescriptor*  m_pass_desc;

        id<MTLCommandBuffer> m_curCommand = 0;
        
        id<MTLParallelRenderCommandEncoder> m_parallelRCE;
        
        std::vector< RefPtr<MetalRenderCommand> > m_arrRenderCommand;
	};

} 




