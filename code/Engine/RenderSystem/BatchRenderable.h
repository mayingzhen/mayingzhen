#pragma once

namespace ma
{
	class BatchRenderable
	{
	public:
		BatchRenderable();

		virtual void	AddRenderObj(Renderable* pRenderObj);

		virtual void	PrepareRender(RenderPassType eRPType);

		virtual void	Render(RenderPassType eRPType, RenderListType eRLType);

		virtual void	Clear();

	protected:
		typedef std::vector<Renderable*> VEC_RENDERABLE;
		VEC_RENDERABLE	m_arrRenderList;
	};
}

