#include "ParticleComponent.h"

namespace ma
{
	IMPL_OBJECT(ParticlComponent,Component)

	ParticlComponent::ParticlComponent(GameObject* pGameObj)
	:Component(pGameObj)
	{
	}

	ParticlComponent::~ParticlComponent()
	{

	}

	void ParticlComponent::Render()
	{
		SceneNode* pSceneNode = m_pGameObject->GetSceneNode();
		Camera* pCamera = pSceneNode->GetSene()->GetCurCamera();
		Matrix4x4 matWorld = pSceneNode->GetWorldMatrix();
		//m_pParticleEmit->draw(pCamera,matWorld);

		//RenderQueue::AddRenderable(m_pParticleEmit->GetRenderable());
	}

	void ParticlComponent::Update()
	{
		//m_pParticleEmit->update(GetTimer()->GetFrameDeltaTime());
	}

	void ParticlComponent::Start()
	{

	}

	void ParticlComponent::Stop()
	{
	}

	void ParticlComponent::Load(const char* pszPath)
	{
		IRenderDevice* pRenderDevice = ma::GetRenderDevice();
		if (pRenderDevice == NULL)
			return;

		m_pParticleEmit = ParticleEmitter::create(pszPath);
		m_pParticleEmit->start();
	}	

	void ParticlComponent::GetBoundingAABB(Vector3& vMin,Vector3& vMax)
	{
// 		if (m_pRendMesh)
// 		{
// 			m_pRendMesh->GetBoundingAABB(vMin,vMax);
// 		}
	}

	void ParticlComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.EndSection();
	}

}

