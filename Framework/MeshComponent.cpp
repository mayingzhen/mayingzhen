#include "MeshComponent.h"

MeshComponent::MeshComponent()
{

}

MeshComponent::~MeshComponent()
{

}

void MeshComponent::Render()
{
	IRender* pRender = GetRender();
	if (pRender == NULL)
		return;
	
	if (m_pMesh == NULL)
		return;

	pRender->RenderMesh( m_pMesh->GetRendMesh() );
}

void MeshComponent::Update()
{

}

void MeshComponent::Start()
{
	
}

void MeshComponent::Stop()
{

}
