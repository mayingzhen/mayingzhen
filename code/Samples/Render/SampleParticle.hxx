#include "SampleParticle.h"


namespace ma
{
	SampleParticle::SampleParticle()
	{
	}

	void SampleParticle::Load()
	{
		Vector3 vEyePos = Vector3(0, 6, 5);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->LookAt(vEyePos,VAtPos);

		RefPtr<SceneNode> pFire = CreateSceneNode();
		m_pScene->GetRootNode()->AddChild(pFire.get());

		SceneNode* pSceneNode = m_pScene->CreateSceneNode();
		RefPtr<CParticleSystem> pParticleSystem = CreateParticleSystem();
		pSceneNode->AddComponent(pParticleSystem.get());

		RefPtr<CParticleSubUVAffector> pSubUVAff = CreateSubUVAffector();
		pSubUVAff->SetTile(4,4);
		pSubUVAff->SetCycles(4);
		pSubUVAff->SetFixedTile(false);
		pParticleSystem->AddAffector(pSubUVAff.get());

		RefPtr<CParticlePointEmitter> pPointEmit = CreatePointEmitter();
		pPointEmit->SetDirection(Vector3::UNIT_Z);
		pPointEmit->SetMinLifeTime(5);
		pPointEmit->SetMaxLifeTime(5);
		pPointEmit->SetMaxAngleDegrees(Vec3i(30,30,30));
		pPointEmit->SetMinVelocity(2.0);
		pPointEmit->SetMaxVelocity(3.0);

		pParticleSystem->AddEmitter(pPointEmit.get());

		pParticleSystem->SaveToXML("particle/money.particle");

		{
			RefPtr<Material> pMaterial = CreateMaterial();

			RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
			pMaterial->AddSubMaterial(0,pSubMaterial.get());
			
			RefPtr<Technique> pShadingTech = CreateTechnique("ShadingTech","particle","particle","");
			RenderState rs = pShadingTech->GetRenderState();
			rs.SetBlendMode(BM_TRANSPARENT);
			rs.SetDepthWrite(false);
			rs.SetCullMode(CULL_FACE_SIDE_NONE);
			pShadingTech->SetRenderState(rs);
			pSubMaterial->SetShadingTechnqiue(pShadingTech.get());

			pSubMaterial->SetParameter("tDiff", Any( CreateTexture("particle/money.dds") ) );

			pMaterial->SaveToXML("particle/money.mtl");
		}


		pParticleSystem->SetMaterialSet( CreateMaterial("particle/money.mtl").get() );

		pParticleSystem->Play();
	}

	void SampleParticle::UnLoad()
	{
	}

	void SampleParticle::Update()
	{
	}


}


