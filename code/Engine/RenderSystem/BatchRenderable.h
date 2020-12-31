#pragma once

namespace ma
{
	class InstanceRenderable;
	class Renderable;
	class Technique;
	class RenderPass;
	class RenderCommand;
	class ComputeCommand;

	struct RenderItem
	{
		Renderable* m_renderable = nullptr;
		Technique* m_tech = nullptr;
	};

	class BatchRenderable
	{
	public:
		BatchRenderable();

		void InitRenderCommand(uint32_t nCount);

		virtual void	AddRenderObj(Renderable* pRenderObj, Technique* pTech);

		virtual void	PrepareRender();

		virtual void	Render(RenderPass* pPass, int stage);

		virtual void	Render(RenderCommand* pCommand, int stage);

		virtual void	Compute(ComputeCommand* pCommand);

		virtual void	Clear();

	protected:
		void			PrepareInstance();

		void			PrepareInstance(const std::vector<RenderItem>& batch);

		void			ParallelRender(RenderCommand* pCommand, RenderItem* pNodeStart, uint32_t nNodeCount);
		
	private:

		typedef std::vector<RenderItem> VEC_RENDERABLE;
		VEC_RENDERABLE		m_arrRenderList;

		struct InstRenderItem
		{
			RefPtr<InstanceRenderable> m_renderable;
			RefPtr<Technique> m_tech;
		};
		typedef std::vector<InstRenderItem> VEC_INSTRENDERABLE;
		VEC_INSTRENDERABLE	m_arrInsRenderList;

		VEC_RENDERABLE		m_arrNoInsRenderList;

		VEC_RENDERABLE		m_arrPrePareRenderList;

		VEC_RENDERABLE		m_batchTemp;

		typedef std::vector< RefPtr<RenderCommand> > VEC_COMMAND;
		VEC_COMMAND			m_vecCommand;
	};
}

