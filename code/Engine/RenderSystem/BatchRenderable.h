#pragma once

namespace ma
{

	class BatchRenderable
	{
	public:
		BatchRenderable();

		virtual void	AddRenderObj(Renderable* pRenderObj);

		virtual void	PrepareRender();

		virtual void	Render();

		virtual void	Clear();

	protected:
		typedef std::vector<Renderable*> VEC_RENDERABLE;
		VEC_RENDERABLE	m_arrRenderList;
	};
}

