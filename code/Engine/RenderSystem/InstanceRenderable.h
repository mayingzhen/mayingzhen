#pragma once

namespace ma
{
	class InstanceRenderable : public Renderable
	{
	public:
		InstanceRenderable(Technique* pTech);

		virtual void	PreRender(Technique* pTech, SceneContext* sc) override;

		virtual	void	Render(Technique* pTechnique, RenderCommand* pRenderCommand) override;

		void			AddRenderable(Renderable* pRenderObj);

		virtual void	PrepareRender(VertexBuffer* pInstanceBuffer, uint32_t& nOffset);

	public:
		typedef std::vector<Renderable*> VEC_RENDERABLE;
		VEC_RENDERABLE		m_arrRenderList;

		struct InstaceData
		{
			Matrix3x4 m_world;
		};

		std::vector<InstaceData> m_arrInstanceData;

		RefPtr<VertexBuffer> m_pInstanceBuffer;
		uint32_t m_nInstaceOffset = 0;
	};
}



