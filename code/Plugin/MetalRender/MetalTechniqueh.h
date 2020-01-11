#pragma once


namespace ma
{
    class MetalPipeline;
    
    class MetalTechnique : public Technique
	{
	public:

		MetalTechnique();

		~MetalTechnique();
        
        virtual void        RT_SetSampler(Uniform* pUniform, SamplerState* pSampler);

		virtual void		RT_StreamComplete();

        virtual void        RT_SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer);
        
        SamplerState*       GetActiveSampler(Uniform* pUniform);
        
        HardwareBuffer*     GetActiveStorageBuffer(Uniform* pUniform);

	public:
        RefPtr<MetalPipeline> m_pPipline;
        
        std::map<Uniform*, RefPtr<SamplerState> > m_activeSamplers;
        
        std::map<Uniform*, RefPtr<HardwareBuffer> > m_activeStorageBuffers;
	};

}


