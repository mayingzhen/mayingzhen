#include "ParticleComponent.h"
#include <random>

namespace ma
{

#define PARTICLE_COUNT 256 * 1024
#define max_num_particles_  65536

	void ParticleRenderable::PreRender(Technique* pTech)
	{
		pTech->Bind(this);
	}

	void ParticleRenderable::Render(Technique* pTechnique, RenderCommand* pRenderCommand)
	{
		Renderable::Render(pTechnique, pRenderCommand);
	}

	void ParticleRenderable::Compute(Technique* pTechnique, ComputeCommand* pComputeCmd)
	{
		//pComputeCmd->SetStorgeBuffer(m_pPosBuffer.get());

		//pComputeCmd->Begin();

		pTechnique->BindCompute(this);

		pComputeCmd->SetTechnique(pTechnique);

		pComputeCmd->Dispatch((max_num_particles_ + 255) / 256, 1, 1);

		//pComputeCmd->End();
	}


	ParticleComponent::ParticleComponent()
	{
		AABB aabb;
		aabb.setInfinite();
		this->SetAABB(aabb);

		m_pRenderable = new ParticleRenderable();

		RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();

		VertexElement element[2];
		element[0] = VertexElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		element[1] = VertexElement(0, 12, DT_FLOAT2, DU_TEXCOORD, 0);
		RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element, 1);

		RefPtr<BlendState> pBlendState = CreateBlendState();
		pBlendState->m_blendDesc[0].BlendEnable = true;

		RefPtr<RasterizerState> pRastate = CreateRasterizerState();
		pRastate->m_eCullMode = CULL_FACE_SIDE_NONE;

		RefPtr<DepthStencilState> pDS = CreateDepthStencilState();
		pDS->m_bDepthWrite = false;

		ShaderCreateInfo info;
		info.m_strVSFile = "particle.hlsl:vs_main";
		info.m_strGSFile = "particle.hlsl:gs_main";
		info.m_strPSFile = "particle.hlsl:ps_main";
		info.m_strCSFile = "particle.hlsl:cs_main";
		info.m_pVertexDecl = pDeclaration.get();
		info.m_pBlendState = pBlendState;
		info.m_pRSState = pRastate;
		info.m_pDSState = pDS;
		info.m_ePrimitiveType = PRIM_POINTLIST;

		if (GetRenderSystem()->GetDefferedLightRenderPass())
		{
			info.m_pRenderPass = GetRenderSystem()->GetDefferedLightRenderPass();
		}
		else
		{
			info.m_pRenderPass = GetRenderSystem()->GetBaseRenderPass();
		}

		RefPtr<Technique> pTech = CreateTechnique(info);

		pSubMaterial->SetShadingTechnqiue(pTech.get());

		m_pRenderable->m_pSubMaterial = pSubMaterial;
		m_pRenderable->m_ePrimitiveType = PRIM_POINTLIST;

		std::default_random_engine rndEngine(0);
		std::uniform_real_distribution<float> rndDist(-1.0f, 1.0f);

		// Initial particle positions
		std::vector<Vector4> particleBuffer(max_num_particles_);
		for (auto& particle : particleBuffer)
		{
			float const angel = rndDist(rndEngine) / 0.05f * 3.14f;
			float const r = rndDist(rndEngine) * 3.0f;

			particle = Vector4(r * cos(angel), 0.2f + abs(rndDist(rndEngine)) * 3.0f, r * sin(angel), 0.0f);
		}
		m_pInitVelBuffer = GetRenderSystem()->CreateVertexBuffer(
			(uint8_t*)&particleBuffer[0], particleBuffer.size(), sizeof(Vector4), HBU_STORGE_BUFFER);

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

		pSubMaterial->SetParameter("particle_pos_rw_stru_buff", Any(m_pPosBuffer));
		pSubMaterial->SetParameter("particle_vel_rw_stru_buff", Any(m_pVelBuffer));
		pSubMaterial->SetParameter("particle_init_vel_buff", Any(m_pInitVelBuffer));
		pSubMaterial->SetParameter("particle_birth_time_buff", Any(m_pBirthTimeBuffer));

		Matrix4 matWorld(Matrix4::IDENTITY);
		pSubMaterial->SetParameter("ps_model_mat", Any(matWorld));

		Vector4 initPosLife = Vector4(0, 0, 0, 8);
		pSubMaterial->SetParameter("init_pos_life", Any(initPosLife));

		float pointRadius = 0.2f;
		pSubMaterial->SetParameter("point_radius", Any(pointRadius));

		m_pRenderable->m_pVertexBuffer = m_pPosBuffer;

	}

	void ParticleComponent::RegisterAttribute()
	{
		REGISTER_OBJECT(ParticleComponent, CreateParticleComponent);
	}


	void ParticleComponent::Update()
	{
		RenderComponent::Update();

		float freq = 256.0f;
		float inv_emit_freq_ = 1.0f / freq;

		float elapsed_time = GetTimer()->GetFrameDeltaTime();
		accumulate_time_ += elapsed_time;
		if (accumulate_time_ >= max_num_particles_ * inv_emit_freq_)
		{
			accumulate_time_ = 0;
		}

		m_pRenderable->m_pSubMaterial->SetParameter("elapse_time", Any(elapsed_time));
		m_pRenderable->m_pSubMaterial->SetParameter("accumulate_time", Any(accumulate_time_));
	}


	void ParticleComponent::Render(RenderQueue* pRenderQueue)
	{
		Technique* pTech = m_pRenderable->GetMaterial()->GetShadingTechnqiue();

		pRenderQueue->AddComputeObj(m_pRenderable.get(), pTech);
		pRenderQueue->AddRenderObj(RL_MeshTrans, m_pRenderable.get(), pTech);
	}

	void ParticleComponent::SetTexture(const char* pszPath)
	{
		m_pSampler = CreateSamplerState(pszPath);

		m_pRenderable->m_pSubMaterial->SetParameter("particle_tex",Any(m_pSampler));
	}

	RefPtr<ParticleComponent> CreateParticleComponent()
	{
		return new ParticleComponent();
	}

}

