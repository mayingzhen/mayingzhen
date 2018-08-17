#pragma once

namespace ma
{

	class ParticleRenderable : public Renderable
	{
		virtual void	PreRender(Technique* pTech);

		virtual void	Render(Technique* pTechnique, RenderCommand* pRenderCommand);

		void			Compute(Technique* pTechnique, ComputeCommad* pComputeCmd);
	};

	class ParticleComponent : public RenderComponent
	{

	public:
		DECL_OBJECT(ParticleComponent)

		ParticleComponent();

		static void			RegisterAttribute();

		virtual	void		Update();

		virtual void		Render(RenderQueue* pRenderQueue);

	private:
		RefPtr<ParticleRenderable>	m_pRenderable;

		RefPtr<VertexBuffer>		m_pInitVelBuffer;
		RefPtr<VertexBuffer>		m_pBirthTimeBuffer;

		RefPtr<VertexBuffer>		m_pPosBuffer;
		RefPtr<VertexBuffer>		m_pVelBuffer;

		float accumulate_time_ = 0;
	};

	RefPtr<ParticleComponent> CreateParticleComponent();
}


