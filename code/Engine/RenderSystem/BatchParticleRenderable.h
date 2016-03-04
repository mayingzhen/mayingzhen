#pragma once

namespace ma
{

	class BatchParticleRenderable : public BatchRenderable
	{
	public:
		
		BatchParticleRenderable();

		virtual void	PrepareRender();
		
		virtual void	Render();

		virtual void	Clear();

	private:
		void MerageRenderable(VEC_RENDERABLE& vecRenderable);

	private:
		VEC_RENDERABLE m_arrMerageRenderList;
		uint32 m_nMerageCount;
	};
}

