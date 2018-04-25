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

	public:
        RefPtr<MetalPipeline> m_pPipline;
	};

}


