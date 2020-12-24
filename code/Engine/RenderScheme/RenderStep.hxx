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

	void RenderStep::Render(RenderPass* prePass,RenderPass* nextPass)
	{
		RenderQueue* cur_renderQueue = m_pRenderQueue.get();
		RenderPass*  cur_renderPass = m_pRenderPass.get();

		SetSceneContext(cur_renderQueue->GetSceneContext());

		if (cur_renderPass != prePass)
		{
			cur_renderPass->Begine();
		}
		
		cur_renderQueue->Render(cur_renderPass);

		if (cur_renderPass != nextPass)
		{
			cur_renderPass->End();
		}
	}

	RefPtr<RenderStep> CreateRenderStep()
	{
		return new RenderStep();
	}

}


