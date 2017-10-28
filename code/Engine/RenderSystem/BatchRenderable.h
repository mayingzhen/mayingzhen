#pragma once

namespace ma
{
	class InstanceRenderable : public Renderable
	{
	public:
		virtual void	PreRender(Technique* pTech);

		virtual	void	Render(RenderCommand* pRenderCommand);

		void			AddRenderable(Renderable* pRenderObj);

		virtual void	Create();

	public:
		typedef std::vector<Renderable*> VEC_RENDERABLE;
		VEC_RENDERABLE		m_arrRenderList;

		struct InstaceData
		{
			// worldMatrix
			Matrix3x4 m_world;
		};

		std::vector<InstaceData> m_arrInstanceData;

		SubAllocVB m_subVB;
	};

	class BatchRenderable
	{
	public:
		BatchRenderable();

		virtual void	AddRenderObj(Renderable* pRenderObj);

		virtual void	PrepareRender(RenderPassType eRPType);

		virtual void	Render(RenderPass* pPass, RenderPassType eRPType, RenderListType eRLType);

		virtual void	Clear();

	private:
		void			PrepareInstance(RenderPassType eRPType);

		void			PrepareInstance(const std::vector<Renderable*>& batch,Technique* pTech);

	protected:
		typedef std::vector<Renderable*> VEC_RENDERABLE;
		VEC_RENDERABLE		m_arrRenderList;

		typedef std::vector< RefPtr<InstanceRenderable> > VEC_INSRENDERABLE;
		VEC_INSRENDERABLE	m_arrInsRenderList;

		VEC_RENDERABLE		m_arrNoInsRenderList;

		VEC_RENDERABLE		m_arrPrePareRenderList;

		VEC_RENDERABLE		m_batchTemp;
	};
}

