#include "PostProcess.h"

namespace ma
{
	void PostProcess::SetOutputTex(Texture* pOutput,int level/* = 0*/, int array_index/* = 0*/, int face/* = 0*/)
	{

	}

	void PostProcess::Render()
	{
// 		FrameBufferPtr const & fb = (0 == num_bind_output_) ? re.DefaultFrameBuffer() : frame_buffer_;
// 		re.BindFrameBuffer(fb);
// 		ViewportPtr const & vp = fb->GetViewport();
// 		if ((!technique_->Transparent()) && (0 == vp->left) && (0 == vp->top)
// 			&& (fb->Width() == static_cast<uint32_t>(vp->width))
// 			&& (fb->Height() == static_cast<uint32_t>(vp->height)))
// 		{
// 			fb->Discard(FrameBuffer::CBM_Color);
// 		}
// 		this->Render();

		FrameBuffer fb;
		fb.AttachColor(0,mTexSSAO.get());
		fb.AttachDepthStencil(GetRenderSystem()->GetDefaultDepthStencil().get());
		GetRenderSystem()->SetFrameBuffer(&fb);
		ScreenQuad::Render(m_pTech.get());
	}
}