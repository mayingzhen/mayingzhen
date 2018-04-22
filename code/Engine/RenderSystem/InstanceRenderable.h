#pragma once

namespace ma
{
	class InstanceRenderable : public Renderable
	{
	public:
		virtual void	PreRender(Technique* pTech);

		virtual	void	Render(Technique* pTechnique, RenderCommand* pRenderCommand);

		void			AddRenderable(Renderable* pRenderObj);

		virtual void	Create();

	public:
		typedef std::vector<Renderable*> VEC_RENDERABLE;
		VEC_RENDERABLE		m_arrRenderList;

		struct InstaceData
		{
			Matrix3x4 m_world;
		};

		std::vector<InstaceData> m_arrInstanceData;

		SubAllocVB m_subVB;
	};
}



