#include "Samples/Render/SampleComputeShader.h"
#include <random>

namespace ma
{
	SampleComputeShader::SampleComputeShader()
	{

	}

	void SampleComputeShader::Load()
	{
		GetCamera()->LookAt(Vector3(5, 3, -5), Vector3(0, 0, 0));

		m_pScene->SetAmbientColor(ColourValue(0.5, 0.5, 0.5));
		m_pScene->GetMainDirLight()->GetSceneNode()->LookAt(Vector3(5, 3, -5), Vector3(0, 0, 0));

		if (1)
		{
			SceneNode* pBox = m_pScene->CreateSceneNode();
			MeshComponent* pBoxMesh = pBox->CreateComponent<MeshComponent>();
			//pBoxMesh->SetSuportInstance(true);
			pBoxMesh->Load("Fbx/Box.skn", "Fbx/Box.mtl");
		}

		SceneNode* pParticleNode = m_pScene->CreateSceneNode();
		ParticleComponent* pParticle = pParticleNode->CreateComponent<ParticleComponent>();
		pParticle->SetTexture("explosion.png");

	}

	void SampleComputeShader::PreRender()
	{

	}
}

