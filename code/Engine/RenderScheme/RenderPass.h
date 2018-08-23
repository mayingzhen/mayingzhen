#pragma once

namespace ma
{
	class Scene;

	struct RenderSurface
	{
		RefPtr<Texture> m_pTexture;
		uint32_t m_nMip = 0;
		uint32_t m_nFace = 0;

		ColourValue m_cClearColor;
	};

	class RenderPass : public Referenced
	{
	public:

		void AttachColor(int index, Texture* pColor, int level, int face) 
		{
			m_arrColor.resize(index + 1);

			m_arrColor[index].m_pTexture = pColor;
			m_arrColor[index].m_nMip = level;
			m_arrColor[index].m_nFace = face;
		}

		void AttachDepthStencil(Texture* pDepthStencil) { m_pDepthStencil = pDepthStencil; }

		void SetViewPort(const Rectangle& viewPort) { m_viewPort = viewPort; }
		const Rectangle&  GetViewPort() const { return m_viewPort; }

		virtual void Create() = 0;

		virtual void Begine() = 0;

		virtual void End() = 0;

		virtual RenderCommand* GetThreadCommand(uint32_t nIndex, RenderListType eRLType) = 0;

	public:
		RefPtr<Texture>				m_pDepthStencil;

		float						m_fClearDepth = 1.0;

		uint32_t					m_nClearStencil = 0;
		
		std::vector<RenderSurface>	m_arrColor;

		Rectangle					m_viewPort;
	};

	class CommandBuffer
	{
	public:
		void AddRenderObj(int nBatchIndex, Renderable* pRenderObj)
		{

		}

		void Render()
		{
			m_pRenderPass->Begine();

// 			for (uint32_t i = 0; i < m_arrRenderList.size(); ++i)
// 			{
// 				m_arrRenderList[i]->PrepareRender(eRPType);
// 				m_arrRenderList[i]->Render(m_pRenderPass.get(), eRPType, eRLType);
// 			}

			m_pRenderPass->End();
		}

	private:
		RefPtr<RenderPass>	m_pRenderPass;

		//RenderQueue*		m_pRenderQueue[2];
		std::vector< RefPtr<BatchRenderable> > m_arrRenderList;
	};
}

