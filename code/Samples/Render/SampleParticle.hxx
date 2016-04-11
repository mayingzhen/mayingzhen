#include "SampleParticle.h"


namespace ma
{
	SampleParticle::SampleParticle()
	{
	}

	void SampleParticle::Load()
	{
		Vector3 vEyePos = Vector3(0, 16, 15);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->LookAt(vEyePos,VAtPos);

		if (1)
		{
			RefPtr<Material> pMaterial = CreateMaterial();

			RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
			pMaterial->AddSubMaterial(0,pSubMaterial.get());

			RefPtr<Technique> pShadingTech = CreateTechnique("ShadingTech","particle","particle","");
			pShadingTech->m_eBlendMode = BM_TRANSPARENT;
			pShadingTech->m_bDepthWrite = false;
			pShadingTech->m_eCullMode = CULL_FACE_SIDE_NONE;
			pSubMaterial->SetShadingTechnqiue(pShadingTech.get());

			pSubMaterial->SetParameter("tDiff", Any( CreateSamplerState("particle/money.dds") ) );

			pMaterial->SaveToFile("particle/money.mtl");

		}

		if (1)
		{
			RefPtr<ParticleSystem> pParticleSystem = CreateParticleSystem();

			RefPtr<ParticleSubUVAffector> pSubUVAff = CreateSubUVAffector();
			pSubUVAff->SetTile(4,4);
			pSubUVAff->SetCycles(4);
			pSubUVAff->SetFixedTile(false);
			pParticleSystem->AddAffector(pSubUVAff.get());

			RefPtr<ParticlePointEmitter> pPointEmit = CreatePointEmitter();
			pPointEmit->SetDirection(Vector3::UNIT_Z);
			pPointEmit->SetMinLifeTime(5);
			pPointEmit->SetMaxLifeTime(5);
			pPointEmit->SetMaxAngleDegrees(Vec3i(30,30,30));
			pPointEmit->SetMinVelocity(2.0);
			pPointEmit->SetMaxVelocity(3.0);

			pParticleSystem->AddEmitter(pPointEmit.get());

			pParticleSystem->SetMaterialSet( CreateMaterial("particle/money.mtl").get() );

			pParticleSystem->SaveToXML("particle/money.particle");
		}

		for (uint32 i = 0; i < 5; ++i)
		{
			SceneNode* pSceneNode = m_pScene->CreateSceneNode();
			pSceneNode->SetPos(Vector3(i * 2,0,0));
			
			RefPtr<ParticleSystem> pParticleSystem = CreateParticleSystem();
			pSceneNode->AddComponent(pParticleSystem.get());

			pParticleSystem->LoadFromXML("particle/money.particle");

			pParticleSystem->Play();
		}
	}

	void SampleParticle::UnLoad()
	{
	}

	void SampleParticle::Update()
	{
	}


}


