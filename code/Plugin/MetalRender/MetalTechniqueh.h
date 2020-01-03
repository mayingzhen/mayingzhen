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

	public:
        RefPtr<MetalPipeline> m_pPipline;
	};

}


