#include "SkyBox.h"
#include "Engine/Renderable/SkyBoxRenderable.h"

namespace ma
{
	SkyBox::SkyBox()
	{
		AABB aabb;
		aabb.setInfinite();
		this->SetAABB(aabb);

		m_pRenderable = new SkyBoxRenderable();
		m_pRenderable->m_nRenderOrder = RL_SkyBox;
	}

	SkyBox::~SkyBox()
	{

	}

	void SkyBox::RegisterAttribute()
	{
		REGISTER_OBJECT(SkyBox, CreateSkyBox);

		ACCESSOR_ATTRIBUTE(SkyBox, "CubePath", GetCubeMap, SetCubeMap, const char*, NULL, AM_DEFAULT);
	}

	void SkyBox::Update()
	{
		RenderComponent::Update();
	}

	void SkyBox::Render(RenderQueue* pRenderQueue)
	{
		pRenderQueue->AddRenderObj(m_pRenderable.get(), m_pRenderable->GetMaterial()->GetShadingTechnqiue());
	}

	void SkyBox::SetCubeMap(const char* pszCubeMap)
	{
		m_strCube = pszCubeMap;

		RefPtr<SamplerState> pSampler = CreateSamplerState(pszCubeMap);

		m_pRenderable->GetMaterial()->SetParameter("tEnv", Any(pSampler));
	}

	const char* SkyBox::GetCubeMap() const
	{
		return m_strCube.c_str();
	}

	RefPtr<SkyBox> CreateSkyBox()
	{
		return new SkyBox();
	}

}

