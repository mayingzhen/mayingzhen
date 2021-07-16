#pragma once

#include "Engine/Scene/RenderComponent.h"

namespace ma
{

	class ScreenRenderProxy : public RenderProxy
	{
	public:
		ScreenRenderProxy();

		virtual uint32_t GetRenderableCount() const { return 1; }
		virtual Renderable* GetRenderableByIndex(uint32_t index) const { return m_pRenderable.get(); }

		void SetTech(const char* pszTech);

	private:
		RefPtr<Renderable> m_pRenderable;
	};

	class ScreenRender : public RenderComponent
	{
	public:
		ScreenRender();

		DECL_OBJECT(ScreenRender)

		void SetTech(const char* pTech);
	};

	RefPtr<ScreenRender> CreateScreenRender();
}