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

		void AttachColor(int index, Texture* pColor, int level = 0, int face = 0, ColourValue cClearColor = ColourValue::Black)
		{
			m_arrColor.resize(index + 1);

			m_arrColor[index].m_pTexture = pColor;
			m_arrColor[index].m_nMip = level;
			m_arrColor[index].m_nFace = face;
		}

		void AttachDepthStencil(Texture* pDepthStencil, float fClearDepth = 1.0f, uint32_t nClearStencil = 0) 
		{
			m_pDepthStencil = pDepthStencil; 
			m_fClearDepth = 1.0f;
			m_nClearStencil = nClearStencil;
		}

		void SetViewPort(const Rectangle& viewPort) { m_viewPort = viewPort; }
		const Rectangle&  GetViewPort() const { return m_viewPort; }

		virtual void Create() = 0;

		virtual void Begine() = 0;

		virtual void End() = 0;

		virtual RenderCommand* GetThreadCommand(uint32_t nIndex, int stage) = 0;

	public:
		RefPtr<Texture>				m_pDepthStencil;

		float						m_fClearDepth = 1.0;

		uint32_t					m_nClearStencil = 0;
		
		std::vector<RenderSurface>	m_arrColor;

		Rectangle					m_viewPort;
	};

}

