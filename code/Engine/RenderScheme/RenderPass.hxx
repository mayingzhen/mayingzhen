#include "RenderPass.h"
#include "../RenderSystem/RenderCommand.h"

namespace ma
{
	void RenderPass::AddRenderCommand(RenderCommand* pCommand)
	{
		m_vecCommand.push_back(pCommand);
	}

	void RenderPass::RemoveRenderCommand(RenderCommand* pCommand)
	{
		auto it = std::find(m_vecCommand.begin(), m_vecCommand.end(), pCommand);
		if (it != m_vecCommand.end())
		{
			m_vecCommand.erase(it);
		}
	}

	void RenderPass::ClearRenderCommand()
	{
		m_vecCommand.clear();
	}
}


