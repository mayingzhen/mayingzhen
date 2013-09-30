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

	void ParticlComponent::BuildRenderItem()
	{
		if (m_pParticleEmit == NULL || m_pGameObject == NULL)
			return;	

		GetParticleSystem()->AddParticleEmitter(m_pParticleEmit);

		m_pParticleEmit->SetWorldMatrix( m_pGameObject->GetSceneNode()->GetWorldMatrix() );

		GetRenderQueue()->AddRenderObj(RL_Trans,m_pParticleEmit);
	}


	void ParticlComponent::Load(const char* pszPath)
	{
		m_pParticleEmit = GetParticleSystem()->Create(pszPath); // ParticleEmitter::create(pszPath);
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

