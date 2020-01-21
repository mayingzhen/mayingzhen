#pragma once

namespace ma
{
	class Scene;

	enum LoadOp 
	{
		LOAD_OP_LOAD,
		LOAD_OP_CLEAR,
		LOAD_OP_DONT_CARE,
	};

	enum StoreOp 
	{
		STORE_OP_STORE = 0,
		STORE_OP_DONT_CARE = 1,
	};

	struct RenderSurface
	{
		RenderSurface(const RefPtr<Texture>& tex = nullptr)
		{
			m_pTexture = tex;
		}

		RefPtr<Texture>		m_pTexture;
		uint32_t			m_nMip = 0;
		uint32_t			m_nFace = 0;
		ColourValue			m_cClearColor;
		float				m_fClearDepth = 1.0;
		uint32_t			m_nClearStencil = 0;
		LoadOp				m_eLoadOp = LOAD_OP_CLEAR;
		StoreOp				m_eStoreOp = STORE_OP_STORE;
	};

	class RenderPass : public Referenced
	{
	public:

		void AttachColor(int index, const RenderSurface& surface)
		{
			m_arrColor.resize(index + 1);

			m_arrColor[index] = surface;
		}

		void AttachDepthStencil(const RenderSurface& surface)
		{
			m_depthStencil = surface;
		}

		virtual void Create() = 0;

		virtual void Begine() = 0;

		virtual void End() = 0;

		virtual RenderCommand* GetThreadCommand(uint32_t nIndex, int stage) = 0;

	public:
		RenderSurface				m_depthStencil;
		
		std::vector<RenderSurface>	m_arrColor;
	};

}

