#pragma once

namespace ma
{
// 	class RenderTarget : public Referenced
// 	{
// 	public:
// 		RenderTarget();
// 		virtual ~RenderTarget();
// 
// 		virtual void RT_CreateRenderTarget() = 0;
// 	};
// 
// 	class DepthStencil : public Referenced
// 	{
// 	public:
// 		DepthStencil();
// 		virtual ~DepthStencil();
// 
// 		virtual void RT_CreateDepthStencil() = 0;
// 	};


	class FrameBuffer : public Referenced
	{
	public:
		//FrameBuffer();
		//virtual ~FrameBuffer();

		void AttachColor(int index,Texture* pColor) {m_arrColor[index] = pColor;}
		void AttachDepthStencil(Texture* pDepthStencil) {m_pDepthStencil = pDepthStencil;}

	public:
		RefPtr<Texture> m_pDepthStencil;
		RefPtr<Texture> m_arrColor[MAX_RENDERTARGETS];

	};
}

