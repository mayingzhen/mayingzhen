#pragma once

#include "Engine/Scene/RenderComponent.h"

namespace ma
{
	class SkyBoxRenderable;

	class SkyBox : public RenderComponent
	{
	public:
		SkyBox();

		~SkyBox();

		DECL_OBJECT(SkyBox)

		static void			RegisterAttribute();

		void				SetCubeMap(const char* pszCubeMap);

		const char*			GetCubeMap() const;

		virtual	void		Update();

		virtual void		Render(RenderQueue* pRenderQueue);

	private:
		std::string			m_strCube;

		RefPtr<SkyBoxRenderable>	m_pRenderable;
	};

	RefPtr<SkyBox> CreateSkyBox();
}