#pragma once

namespace ma
{
	class ParticleProxy;

	class ParticleRenderable : public Renderable
	{
		virtual void	Compute(Technique* pTechnique, ComputeCommand* pComputeCmd, SceneContext* sc) override;
	};

	class ParticleComponent : public RenderComponent
	{

	public:
		DECL_OBJECT(ParticleComponent)

		ParticleComponent();

		static void			RegisterAttribute();

		virtual	void		Update() override;

		virtual void		Render(RenderQueue* pRenderQueue);

		void				SetTexture(const char* pszPath);

	private:
		ParticleProxy*		GetParticlePoxy();
	};

	class ParticleProxy : public RenderProxy
	{
	public:
		ParticleProxy();

		uint32_t					GetRenderableCount() const { return 1; }
		Renderable*					GetRenderableByIndex(uint32_t index) const { return m_pRenderable.get(); }

		void						Update();

		void						SetTexture(RefPtr<SamplerState> sampler);

	protected:
		RefPtr<ParticleRenderable>	m_pRenderable;

		RefPtr<VertexBuffer>		m_pInitVelBuffer;
		RefPtr<VertexBuffer>		m_pBirthTimeBuffer;

		RefPtr<VertexBuffer>		m_pPosBuffer;
		RefPtr<VertexBuffer>		m_pVelBuffer;

		RefPtr<SamplerState>		m_pSampler;

		float						m_fAccumulateTime = 0;
	};

	RefPtr<ParticleComponent> CreateParticleComponent();
}


