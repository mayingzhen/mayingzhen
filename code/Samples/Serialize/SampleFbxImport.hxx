#include "Samples/Serialize/SampleFbxImport.h"
#include "Animation/Module.h"
#include "D3D9Render/Module.h"
#include "Plugin/FbxImport/Module.h"

namespace ma
{
	SampleFbxImport::SampleFbxImport()
	{
		m_pAnimtionPlay = NULL;
		m_pSkeleton = NULL;
		m_pRenderMesh = NULL;
		//m_pRendTexture = NULL;
		//m_pRenderMaterial = NULL;

		m_pStaticMesh = NULL;
		//m_pStatcMeshTexture = NULL;
		//m_pStaticMeshMaterial = NULL;

		m_pBoxMesh = NULL;
		//m_pBoxTexture = NULL;
		m_pBoxMaterial = NULL;

		m_pEmitter = NULL;
	}

	void SampleFbxImport::LoadSkelMesh(FBXImporter& fbxImpor)
	{
		MeshData* pMeshData = new MeshData;
		SkeletonData* pSkeData = new SkeletonData;
		AnimationData* pAnimData = new AnimationData;
// 		fbxImpor.LoadSkeletonMeshData("../../Data/Fbx/TestBull_anim.fbx",pMeshData,pSkeData);
// 		fbxImpor.LoadAnimationData("../../Data/Fbx/TestBull_anim.fbx",pAnimData,pSkeData);
// 
// 		// Save
// 		pMeshData->Save("../../Data/Fbx/TestBull.skn");
// 		pSkeData->Save("../../Data/Fbx/TestBull.ske");
// 		pAnimData->Save("../../Data/Fbx/TestBull.ska");
// 		SAFE_DELETE(pMeshData);
// 		SAFE_DELETE(pSkeData);
// 		SAFE_DELETE(pAnimData);


		m_pRenderMesh = new RenderMesh();
		m_pRenderMesh->Load("FBX/TestBull.skn","FBX/TestBull_DM.png");        

		pSkeData = static_cast<SkeletonData*>(ResourceManager::DeclareResource("FBX/TestBull.ske"));
		pSkeData->Load();

		m_pSkeleton = new Skeleton();
		m_pSkeleton->InitWithData(pSkeData);

		m_pAnimtionPlay = new AnimationPlay(m_pSkeleton);
		std::string strPath = std::string( FileSystem::getResourcePath() ) + "FBX/TestBull.ska";
		m_pAnimtionPlay->AddAction(strPath.c_str(),"TestAction");
		m_pAnimtionPlay->PlayAnimation("TestAction");
	}

	void SampleFbxImport::LoadSaticMesh(FBXImporter& fbxImpor)
	{
// 		MeshData* pMeshData = new MeshData;
// 		fbxImpor.LoadStaticMeshData("../../Data/Fbx/MovingPlatform.fbx",pMeshData);
// 		pMeshData->Save("../../Data/Fbx/MovingPlatform.skn");
// 		SAFE_DELETE(pMeshData);

		m_pStaticMesh = new RenderMesh();
		m_pStaticMesh->Load("Fbx/MovingPlatform.skn","Fbx/PlatformTexture.tga");
	}

	void SampleFbxImport::LoadParticles()
	{
		m_pEmitter = ParticleEmitter::create("Particle/fire.particle");
		m_pEmitter->start();
	}

	void SampleFbxImport::Init(const Platform* pPlatform)
	{
		Sample::Init(pPlatform);
		
		//Vector3 vEyePos = Vector3(0, 600, 800);
		Vector3 vEyePos = Vector3(0, 40, 40);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		m_pCamera->LookAt(vEyePos,VAtPos,vUp);

		FBXImporter fbxImpor;
		fbxImpor.Initialize();

		LoadSaticMesh(fbxImpor);

		LoadSkelMesh(fbxImpor);

		LoadBoxMesh(fbxImpor);

		LoadParticles();
		
		m_pDepthTex = GetRenderDevice()->CreateRenderTarget();
		m_pDepthTex->Create(-1,-1,FMT_R32F);

		m_pNormalTex = GetRenderDevice()->CreateRenderTarget();
		m_pNormalTex->Create(-1,-1,FMT_A8R8G8B8);

		m_pDiffuse = GetRenderDevice()->CreateRenderTarget();
		m_pDiffuse->Create(-1,-1,FMT_A8R8G8B8);

		m_pSpecular = GetRenderDevice()->CreateRenderTarget();
		m_pSpecular->Create(-1,-1,FMT_A8R8G8B8);

		m_pScreenQuad = new ScreenQuad();
		m_pScreenQuad->Init();

		m_pMaterDeferred = new Material("AMBIENT_LIGHT","DeferredLight");
		//m_pMaterDeferred->CreateTechnique("")
		
		Sampler* pDiffuse = Sampler::create(m_pDiffuse->GetTexture());
		Sampler* pSpeclar = Sampler::create(m_pSpecular->GetTexture());

		if (m_pRenderMesh)
		{
			m_pRenderMesh->GetMaterial()->CreateTechnique("gbuffer","gbuffer");
			
			m_pRenderMesh->GetMaterial()->GetParameter("u_textureLightDiffuse")->setSampler(pDiffuse);
			m_pRenderMesh->GetMaterial()->GetParameter("u_textureLightSpecular")->setSampler(pSpeclar);

			m_pRenderMesh->GetMaterial()->CreateTechnique("shading","default","DeferredLight");
		}

		if (m_pStaticMesh)
		{
			m_pStaticMesh->GetMaterial()->CreateTechnique("gbuffer","gbuffer");

			m_pStaticMesh->GetMaterial()->GetParameter("u_textureLightDiffuse")->setSampler(pDiffuse);
			m_pStaticMesh->GetMaterial()->GetParameter("u_textureLightSpecular")->setSampler(pSpeclar);

			m_pStaticMesh->GetMaterial()->CreateTechnique("shading","default","DeferredLight");
		}

		m_pLight = new Light();
	}

	void SampleFbxImport::LoadBoxMesh(FBXImporter& fbxImpor)
	{
		MeshData* pMeshData = new MeshData;
		fbxImpor.LoadStaticMeshData("../../Data/Fbx/Box.fbx",pMeshData);
		pMeshData->Save("../../Data/Fbx/Box.skn");
		SAFE_DELETE(pMeshData);

		m_pBoxMesh = new RenderMesh();
		m_pBoxMesh->Load("Fbx/Box.skn","Fbx/Box.tga");

	}


	void SampleFbxImport::Update()
	{

		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		if (m_pAnimtionPlay)
		{
			m_pAnimtionPlay->AdvanceTime(fTimeElapsed);

			m_pAnimtionPlay->EvaluateAnimation(1.0f);

			Matrix4x4* skinMatrix = m_pAnimtionPlay->GetSkinMatrixArray();
			UINT nNumber = m_pAnimtionPlay->GetSkinMatrixNumber();

			m_pRenderMesh->SetSkinMatrix(skinMatrix,nNumber);
		}
	
		if (m_pEmitter)
		{	
			m_pEmitter->update(fTimeElapsed);
		}
	}

	void SampleFbxImport::Render()
	{
		if (GetRenderDevice() == NULL)
			return;

		if (m_pRenderMesh)
		{
			Matrix4x4 matWorld;
			MatrixTranslation(&matWorld,20,0,0);
 			Matrix4x4 matScale;
 			MatrixScaling(&matScale, 1.0f / 50.0f, 1.0f / 50.0f, 1.0f / 50.0f); 
 
 			matWorld = matScale * matWorld;
			m_pRenderMesh->SetWorldMatrix(matWorld);
		}

		if (m_pStaticMesh)
		{
			Matrix4x4 matWorld;
			MatrixTranslation(&matWorld,0,0,0);

			m_pStaticMesh->SetWorldMatrix(matWorld);
		}

		
		// GbufferPass
		{
			RenderTarget* pPreTar1 = GetRenderDevice()->SetRenderTarget(m_pDepthTex,0);
			RenderTarget* pPreTar2 = GetRenderDevice()->SetRenderTarget(m_pNormalTex,1);

			GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

			if (m_pRenderMesh)
				m_pRenderMesh->Draw("gbuffer");

			if (m_pStaticMesh)
				m_pStaticMesh->Draw("gbuffer");
		
			GetRenderDevice()->SetRenderTarget(pPreTar1,0);
			GetRenderDevice()->SetRenderTarget(pPreTar2,1);
		}

		// Deffered Light
		{
			RenderTarget* pPreTar1 = GetRenderDevice()->SetRenderTarget(m_pDiffuse,0);
			RenderTarget* pPreTar2 = GetRenderDevice()->SetRenderTarget(m_pSpecular,1);

			GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

			Matrix4x4 mInvProj;
			Matrix4x4 matProj = m_pCamera->GetProjMatrix();
			MatrixInverse(&mInvProj, NULL, &matProj);
			m_pMaterDeferred->GetParameter("u_InvProjMatrix")->setMatrix(mInvProj);

			m_pScreenQuad->Render(m_pMaterDeferred);	

			GetRenderDevice()->SetRenderTarget(pPreTar1,0);
			GetRenderDevice()->SetRenderTarget(pPreTar2,1);
		}

		// Shading pass
		{
			GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

			if (m_pRenderMesh)
			{	
				m_pRenderMesh->Draw("shading");
			}

			if (m_pStaticMesh)
			{
				m_pStaticMesh->Draw("shading");
			}

		}

	
// 
// 		if (m_pBoxMesh)
// 		{
// 			Matrix4x4 matWorld;
// 			Matrix4x4 matScale;
// 			MatrixScaling(&matScale,200,200,200); 
// 			MatrixTranslation(&matWorld,0,0,0);
// 			matWorld = matScale * matWorld;
// 			
// 			m_pBoxMesh->SetWorldMatrix(matWorld);
// 
// 			m_pBoxMesh->Draw();
// 		}
// 
// 		if (m_pEmitter)
// 		{
// 			Matrix4x4 matWorld;
// 			MatrixTranslation(&matWorld,-20,0,0);
// 				
// 			m_pEmitter->draw(m_pCamera,matWorld);
// 		
// 		}

	}

}


