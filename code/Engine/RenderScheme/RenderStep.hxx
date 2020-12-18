#include "RenderScheme.h"
#include "../RenderSystem/RenderCommand.h"
#include "../RenderSystem/RenderProfile.h"
#include "PostProcess.h"

namespace ma
{

	RenderStep::RenderStep()
	{
		m_pRenderQueue = new RenderQueue;
	}
	

	RenderStep::~RenderStep()
	{

	}

	void RenderStep::Render()
	{
		RenderQueue* cur_renderQueue = m_pRenderQueue.get();
		RenderPass*  cur_renderPass = m_pRenderPass.get();

		SetSceneContext(cur_renderQueue->GetSceneContext());

		cur_renderPass->Begine();

		cur_renderQueue->Render(cur_renderPass);

		cur_renderPass->End();
	}



}


