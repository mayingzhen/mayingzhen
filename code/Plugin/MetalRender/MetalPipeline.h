#pragma once


namespace ma
{
    class MetalTechnique;
    
	class MetalPipeline : public Referenced
	{
	public:

		MetalPipeline();

		~MetalPipeline();
        
        virtual void RT_StreamComplete(MetalTechnique* pTech);
        
	public:
        
        id <MTLRenderPipelineState> m_pipelineState;
         MTLRenderPipelineReflection* m_pReflection;
    };

    RefPtr<MetalPipeline> CreateMetalPipeline(MetalTechnique* pTech);
    
}


