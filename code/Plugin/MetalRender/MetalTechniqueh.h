#pragma once


namespace ma
{
    class MetalPipeline;
    
    class MetalTechnique : public Technique
	{
	public:

		MetalTechnique();

		~MetalTechnique();

		virtual void		CommitChanges(RenderCommand* pCmd);

		void				RT_StreamComplete();

	private:
        RefPtr<MetalPipeline> m_pPipline;
	};

}


