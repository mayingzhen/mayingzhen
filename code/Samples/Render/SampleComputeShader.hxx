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
		//DECL_OBJECT(ParticleComponent)

		ParticleComponent()
		{
			AABB aabb;
			aabb.setInfinite();
			this->SetAABB(aabb);

			m_pRenderable = new Renderable();

			RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
			
			VertexElement element[1];
			element[0] = VertexElement(0, 0, DT_FLOAT4, DU_POSITION, 0);
			RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element, 1);

			RefPtr<BlendState> pBlendState = CreateBlendState();
			pBlendState->m_blendDesc[0].BlendEnable = true;

			RefPtr<RasterizerState> pRastate = CreateRasterizerState();
			pRastate->m_eCullMode = CULL_FACE_SIDE_NONE;

			ShaderCreateInfo info;
			info.m_strVSFile = "particle.hlsl:vs_main";
			info.m_strGSFile = "particle.hlsl:gs_main";
			info.m_strPSFile = "particle.hlsl:ps_main";
			info.m_strCSFile = "particle.hlsl:cs_main";
			info.m_pVertexDecl = pDeclaration.get();
			info.m_pBlendState = pBlendState;
			info.m_pRSState = pRastate;
			info.m_ePrimitiveType = PRIM_POINTLIST;
			RefPtr<Technique> pTech = CreateTechnique("Particle", info);

			pSubMaterial->SetShadingTechnqiue(pTech.get());

			m_pRenderable->m_pSubMaterial = pSubMaterial;
			m_pRenderable->m_ePrimitiveType = PRIM_POINTLIST;

		}

		virtual	void		Update()
		{
			RenderComponent::Update();
		}

		virtual void		Render(RenderQueue* pRenderQueue)
		{
			pRenderQueue->AddRenderObj(RL_Mesh, m_pRenderable.get());
		}

	public:
		RefPtr<Renderable> m_pRenderable;

// 		RefPtr<VertexBuffer> m_pInitVelBuffer;
// 		RefPtr<VertexBuffer> m_pBirthTimeBuffer;
// 
// 		RefPtr<VertexBuffer> m_pPosBuffer;
// 		RefPtr<VertexBuffer> m_pVelBuffer;
	};

	SampleComputeShader::SampleComputeShader()
	{

	}

	void SampleComputeShader::Load()
	{
		GetCamera()->LookAt(Vector3(5, 3, -5), Vector3(0, 0, 0));
		GetCameraControll()->Init();

		m_pScene->SetAmbientColor(Vector3(0.5, 0.5, 0.5));
		m_pScene->GetMainDirLight()->GetSceneNode()->LookAt(Vector3(5, 3, -5), Vector3(0, 0, 0));

		if (0)
		{
			SceneNode* pBox = m_pScene->CreateSceneNode();
			MeshComponent* pBoxMesh = pBox->CreateComponent<MeshComponent>();
			pBoxMesh->SetSuportInstance(true);
			pBoxMesh->Load("Fbx/Box.skn", "Fbx/Box.mtl");
		}

		SceneNode* pBox = m_pScene->CreateSceneNode();
		ParticleComponent* pParticle = new ParticleComponent();
		pBox->AddComponent(pParticle);


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
		m_pInitVelBuffer = GetRenderSystem()->CreateVertexBuffer(
			(uint8_t*)&particleBuffer[0], particleBuffer.size(), sizeof(ParticleVertex), HBU_STORGE_BUFFER);

		float freq = 256.0f;
		float inv_emit_freq_ = 1.0f / freq;
		float time = 0;
		std::vector<float> time_v(256 * 256);
		for (size_t i = 0; i < time_v.size(); ++i)
		{
			time_v[i] = float(time);
			time += inv_emit_freq_;
		}
		m_pBirthTimeBuffer = GetRenderSystem()->CreateVertexBuffer(
			(uint8_t*)&time_v[0], time_v.size(), sizeof(float), HBU_STORGE_BUFFER);

		std::vector<Vector4> p(max_num_particles_);
		for (size_t i = 0; i < p.size(); ++i)
		{
			p[i] = Vector4(0, 0, 0, -1);
		}
		m_pPosBuffer = GetRenderSystem()->CreateVertexBuffer(
			(uint8_t*)&p[0], p.size(), sizeof(Vector4), HBU_STORGE_VECTOR_BUFFER);
		m_pVelBuffer = GetRenderSystem()->CreateVertexBuffer(
			(uint8_t*)&p[0], p.size(), sizeof(Vector4), HBU_STORGE_VECTOR_BUFFER);

		m_pMaterial = pParticle->m_pRenderable->m_pSubMaterial;

		//ShaderCreateInfo info;
		//info.m_strCSFile = "particle.hlsl:cs_main";
		//info.m_pVertexDecl = CreateVertexDeclaration();
		//m_pCSTech = CreateTechnique("ParticleCS", info);
		//m_pCSMaterial = CreateSubMaterial();
		//m_pCSMaterial->SetShadingTechnqiue(m_pCSTech.get());
		m_pMaterial->SetParameter("particle_pos_rw_stru_buff", Any(m_pPosBuffer));
		m_pMaterial->SetParameter("particle_vel_rw_stru_buff", Any(m_pVelBuffer));
		m_pMaterial->SetParameter("particle_init_vel_buff", Any(m_pInitVelBuffer));
		m_pMaterial->SetParameter("particle_birth_time_buff", Any(m_pBirthTimeBuffer));

		Matrix4 matWorld(Matrix4::IDENTITY);
		m_pMaterial->SetParameter("ps_model_mat", Any(matWorld));
		
		Vector4 initPosLife = Vector4(0, 0, 0, 8);
		m_pMaterial->SetParameter("init_pos_life", Any(initPosLife));

		float pointRadius = 0.2f;
		m_pMaterial->SetParameter("point_radius", Any(pointRadius));

		pParticle->m_pRenderable->m_pVertexBuffer = m_pPosBuffer;
	}

	void SampleComputeShader::PreRender()
	{
		float freq = 256.0f;
		float inv_emit_freq_ = 1.0f / freq;
		int max_num_particles_ = 65536;

		float elapsed_time = GetTimer()->GetFrameDeltaTime();
		accumulate_time_ += elapsed_time;
		if (accumulate_time_ >= max_num_particles_ * inv_emit_freq_)
		{
			accumulate_time_ = 0;
		}

		m_pMaterial->SetParameter("elapse_time", Any(elapsed_time));
		m_pMaterial->SetParameter("accumulate_time", Any(accumulate_time_));

		//return;
		ComputeCommad* pComputeCmd = GetRenderSystem()->GetComputeCommad();

		pComputeCmd->SetStorgeBuffer(m_pPosBuffer.get());

		GetRenderSystem()->BegineCompute();

		pComputeCmd->Begin();

		Technique* pTech = m_pMaterial->GetShadingTechnqiue();

		pTech->BindCompute(NULL);

		pComputeCmd->SetTechnique(pTech);

		pComputeCmd->Dispatch((max_num_particles_ + 255) / 256, 1, 1);

		pComputeCmd->End();

		GetRenderSystem()->EndCompute();
	}
}

