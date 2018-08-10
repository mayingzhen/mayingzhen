#include "Samples/Render/SampleComputeShader.h"
#include <random>

namespace ma
{
	// SSBO particle declaration
	struct ParticleVertex
	{
// 		Vector2 pos;								// Particle position
// 		Vector2 vel;								// Particle velocity
// 		Vector4 gradientPos;						// Texture coordiantes for the gradient ramp map
		Vector4 p;
	};

#define PARTICLE_COUNT 256 * 1024

	class ParticleComponent : public RenderComponent
	{

	public:
		DECL_OBJECT(ParticleComponent)

		ParticleComponent()
		{
			AABB aabb;
			aabb.setInfinite();
			this->SetAABB(aabb);

			m_pRenderable = new Renderable();

			RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
			
			ShaderCreateInfo info;
			info.m_strVSFile = "particle.hlsl:vs_main";
			info.m_strPSFile = "particle.hlsl:ps_main";
			RefPtr<Technique> pTech = CreateTechnique("Particle", info);

			pSubMaterial->SetShadingTechnqiue(pTech.get());
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

		RefPtr<VertexBuffer> m_pInitVelBuffer;
		RefPtr<VertexBuffer> m_pBirthTimeBuffer;

		RefPtr<VertexBuffer> m_pPosBuffer;
		RefPtr<VertexBuffer> m_pVelBuffer;
	};

	SampleComputeShader::SampleComputeShader()
	{

	}

	void SampleComputeShader::Load()
	{
		GetCamera()->LookAt(Vector3(5, 3, -5), Vector3(0, 0, 0));
		GetCameraControll()->Init();

		SceneNode* pBox = m_pScene->CreateSceneNode();
		//ParticleComponent* pParticle = pBox->CreateComponent<ParticleComponent>();


		std::default_random_engine rndEngine(0);
		std::uniform_real_distribution<float> rndDist(-1.0f, 1.0f);

		int max_num_particles_ = 65536;

		// Initial particle positions
		std::vector<ParticleVertex> particleBuffer(max_num_particles_);
		for (auto& particle : particleBuffer)
		{
			float const angel = rndDist(rndEngine) / 0.05f * 3.14;
			float const r = rndDist(rndEngine) * 3.0;

			particle.p = Vector4(r * cos(angel), 0.2f + abs(rndDist(rndEngine)) * 3, r * sin(angel), 0);
		}
		m_pInitVelBuffer = GetRenderSystem()->CreateVertexBuffer((uint8_t*)&particleBuffer[0], particleBuffer.size(), sizeof(ParticleVertex));

		float freq = 256.0f;
		float inv_emit_freq_ = 1.0f / freq;
		float time = 0;
		std::vector<float> time_v(256 * 256);
		for (size_t i = 0; i < time_v.size(); ++i)
		{
			time_v[i] = float(time);
			time += inv_emit_freq_;
		}
		m_pBirthTimeBuffer = GetRenderSystem()->CreateVertexBuffer((uint8_t*)&time_v[0], time_v.size(), sizeof(float));

		std::vector<Vector4> p(max_num_particles_);
		for (size_t i = 0; i < p.size(); ++i)
		{
			p[i] = Vector4(0, 0, 0, -1);
		}
		RefPtr<VertexBuffer> m_pPosBuffer = GetRenderSystem()->CreateVertexBuffer((uint8_t*)&p[0], p.size(), sizeof(Vector4));
		RefPtr<VertexBuffer> m_pVelBuffer = GetRenderSystem()->CreateVertexBuffer((uint8_t*)&p[0], p.size(), sizeof(Vector4));

		RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();

		ShaderCreateInfo info;
		info.m_strCSFile = "particle.hlsl:cs_main";
		info.m_pVertexDecl = CreateVertexDeclaration();
		m_pCSTech = CreateTechnique("ParticleCS", info);

	}

	void SampleComputeShader::PreRender()
	{
		ComputeCommad* pComputeCmd = GetRenderSystem()->GetComputeCommad();

		pComputeCmd->Begin();

		pComputeCmd->SetTechnique(m_pCSTech.get());

		int max_num_particles_ = 65536;
		pComputeCmd->Dispatch((max_num_particles_ + 255) / 256, 1, 1);

		pComputeCmd->End();
	}
}

