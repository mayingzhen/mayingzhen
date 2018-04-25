#pragma once

namespace ma
{
	class Scene;

	struct RenderSurface
	{
		RefPtr<Texture> m_pTexture;
		uint32_t m_nMip;
		uint32_t m_nFace;

		ColourValue m_cClearColor;
	};

	class RenderPass : public Referenced
	{
	public:

		void AttachColor(int index, Texture* pColor, int level, int face) 
		{
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
		RefPtr<Texture> m_pDepthStencil;

		float m_fClearDepth = 1.0;

		uint32_t m_nClearStencil = 0;
		
		RenderSurface m_arrColor[MAX_RENDERTARGETS];

		Rectangle m_viewPort;
	};
}

