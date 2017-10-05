#pragma once


namespace ma
{
    class MetalPipeline;
    
    class MetalTechnique : public Technique
	{
	public:

		MetalTechnique();

		~MetalTechnique();

		void				RT_StreamComplete();

	public:
        RefPtr<MetalPipeline> m_pPipline;
	};

}


