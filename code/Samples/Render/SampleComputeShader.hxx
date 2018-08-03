#include "Samples/Render/SampleComputeShader.h"
#include <random>

namespace ma
{
	// SSBO particle declaration
	struct ParticleVertex
	{
		Vector2 pos;								// Particle position
		Vector2 vel;								// Particle velocity
		Vector4 gradientPos;						// Texture coordiantes for the gradient ramp map
	};

#define PARTICLE_COUNT 256 * 1024

	class ParticleComponent : public RenderComponent
	{
	private:
		ParticleComponent()
		{
			AABB aabb;
			aabb.setInfinite();
			this->SetAABB(aabb);

			m_pRenderable = new Renderable();

			std::default_random_engine rndEngine(0);
			std::uniform_real_distribution<float> rndDist(-1.0f, 1.0f);

			// Initial particle positions
			std::vector<ParticleVertex> particleBuffer(PARTICLE_COUNT);
			for (auto& particle : particleBuffer)
			{
				particle.pos = Vector2(rndDist(rndEngine), rndDist(rndEngine));
				particle.vel = Vector2(0.0f);
				particle.gradientPos.x = particle.pos.x / 2.0f;
			}
			m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer((uint8_t*)&particleBuffer[0], particleBuffer.size(), sizeof(ParticleVertex));

			m_pRenderable->m_pVertexBuffer = m_pVertexBuffer;
		
		
			RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
			
// 			ShaderCreateInfo info;
// 			info.m_strVSFile = "part"
// 			RefPtr<Technique> pTech;

		}

		virtual	void		Update()
		{
			RenderComponent::Update();
		}

		virtual void		Render(RenderQueue* pRenderQueue)
		{
			pRenderQueue->AddRenderObj(RL_Mesh, m_pRenderable.get());
		}

	private:
		RefPtr<Renderable> m_pRenderable;

		RefPtr<VertexBuffer> m_pVertexBuffer;
	};

	SampleComputeShader::SampleComputeShader()
	{
	}




	void SampleComputeShader::Load()
	{

		GetCamera()->LookAt(Vector3(5, 3, -5), Vector3(0, 0, 0));
		GetCameraControll()->Init();



		SceneNode* pBox = m_pScene->CreateSceneNode();
		ParticleComponent* pParticle = pBox->CreateComponent<ParticleComponent>();

		//VkDeviceSize storageBufferSize = particleBuffer.size() * sizeof(Particle);

		if (1)
		{
			CreateMeshMaterial("FBX/Box.tga", "FBX/Box.mtl");

			CreateMeshMaterial("magician/magician/body.tga", "magician/magician/Body.mtl", "SKIN;LIGHT;BRDF;SPEC");
		}
		
		if (1)
		{
			for (uint32_t i = 1; i < 4; ++i)
			{
				SceneNode* pBox = m_pScene->CreateSceneNode();
				MeshComponent* pBoxMesh = pBox->CreateComponent<MeshComponent>();
				pBoxMesh->SetSuportInstance(true);
				pBoxMesh->Load("Fbx/Box.skn", "Fbx/Box.mtl");
				//pBoxMesh->SetShadowCaster(true);
                float x = (float)i + 2.0f;//Math::RangeRandom(0, 150);
                float z = (float)i + 2.0f;//Math::RangeRandom(0, 150);
				pBox->SetPos(Vector3(x, 0, z));
			}
		}

	}

}



