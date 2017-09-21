#pragma once

namespace ma
{
	class FrameBuffer : public Referenced
	{
	public:
		//FrameBuffer();
		//virtual ~FrameBuffer();

		void AttachColor(int index,Texture* pColor) {m_arrColor[index] = pColor;}
		void AttachDepthStencil(Texture* pDepthStencil) {m_pDepthStencil = pDepthStencil;}

		virtual void Create(){}

	public:
		RefPtr<Texture> m_pDepthStencil;
		RefPtr<Texture> m_arrColor[MAX_RENDERTARGETS];

	};
}


