#pragma once

namespace ma
{
	class Scene;

	class RenderPass : public Object
	{
	public:

		void AttachColor(int index, Texture* pColor) { m_arrColor[index] = pColor; }
		void AttachDepthStencil(Texture* pDepthStencil) { m_pDepthStencil = pDepthStencil; }

		void SetViewPort(const Rectangle& viewPort) { m_viewPort = viewPort; }
		const Rectangle&  GetViewPort() const { return m_viewPort; }

		virtual void Create() = 0;

		virtual void Begine() = 0;

		virtual void End() = 0;

		virtual RenderCommand* GetThreadCommand(UINT nIndex, RenderListType eRLType) = 0;

	public:
		RefPtr<Texture> m_pDepthStencil;

		RefPtr<Texture> m_arrColor[MAX_RENDERTARGETS];

		ColourValue m_arrClearColor[MAX_RENDERTARGETS];

		float m_fClearDepth = 1.0;
		
		UINT m_nClearStencil = 0;

		Rectangle m_viewPort;
	};
}

