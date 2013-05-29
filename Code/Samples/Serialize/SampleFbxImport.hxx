#include "Samples/Serialize/SampleFbxImport.h"
#include "Animation/Module.h"
//#include "D3D9Render/Module.h"
//#include "Plugin/FbxImport/Module.h"

namespace ma
{
	SampleFbxImport::SampleFbxImport()
	{
		m_pAnimtionPlay = NULL;
		m_pSkeleton = NULL;
		m_pRenderMesh = NULL;
		m_pRendTexture = NULL;

		m_pStaticMesh = NULL;
		m_pStatcMeshTexture = NULL;

		m_pBoxMesh = NULL;
		m_pBoxTexture = NULL;

		m_pEmitter = NULL;
	}

	void SampleFbxImport::LoadSkelMesh(/*FBXImporter& fbxImpor*/)
	{
		MeshData* pMeshData = new MeshData;
		SkeletonData* pSkeData = new SkeletonData;
		AnimationData* pAnimData = new AnimationData;

// 		fbxImpor.LoadSkeletonMeshData("../Data/Fbx/TestBull_anim.fbx",pMeshData,pSkeData);
// 
// 		fbxImpor.LoadAnimationData("../Data/Fbx/TestBull_anim.fbx",pAnimData,pSkeData);
// 
// 		// Save
// 		SaveMeshToBinaryFile("../Data/Fbx/TestBull.skn",pMeshData);
// 		SaveSkeletonToBinaryFile("../Data/Fbx/TestBull.ske",pSkeData);
// 		SaveAnimationToBinaryFile("../Data/Fbx/TestBull.ska",pAnimData);
// 		SAFE_DELETE(pMeshData);
// 		SAFE_DELETE(pSkeData);
// 		SAFE_DELETE(pAnimData);
		///
        std::string strPath;
        
		pMeshData = new MeshData;
        strPath = std::string( FileSystem::getResourcePath() ) + "Data/FBX/TestBull.skn";
		pMeshData->Load(strPath.c_str());
		//pMeshData = LoadMeshFromBinaryFile("../Data/Fbx/TestBull.skn");
        strPath = std::string( FileSystem::getResourcePath() ) + "Data/FBX/TestBull.ske";
		pSkeData = LoadSkeletonFromBinaryFile(strPath.c_str());
		//pAnimData = LoadAnimationFromBinaryFile("../Data/Fbx/TestBull.ska");

		m_pRenderMesh = new RenderMesh();
		m_pRenderMesh->InitWithData(pMeshData);

		m_pRendTexture = GetRenderDevice()->CreateRendTexture();
        strPath = std::string( FileSystem::getResourcePath() ) + "Data/FBX/TestBull_DM.png";
		m_pRendTexture->Load(strPath.c_str());

		m_pSkeleton = new Skeleton();
		m_pSkeleton->InitWithData(pSkeData);

		m_pAnimtionPlay = new AnimationPlay(m_pSkeleton);
         strPath = std::string( FileSystem::getResourcePath() ) + "Data/FBX/TestBull.ska";
		m_pAnimtionPlay->AddAction(strPath.c_str(),"TestAction");
		m_pAnimtionPlay->PlayAnimation("TestAction");
	}

	void SampleFbxImport::LoadSaticMesh(/*FBXImporter& fbxImpor*/)
	{
		MeshData* pMeshData = new MeshData;

		//fbxImpor.LoadStaticMeshData("../Data/Fbx/MovingPlatform.fbx",pMeshData);

		//SaveMeshToBinaryFile("../Data/Fbx/MovingPlatform.skn",pMeshData);
		//SAFE_DELETE(pMeshData);

		pMeshData = new MeshData();
		pMeshData->Load("Data/Fbx/MovingPlatform.skn");
		//pMeshData = LoadMeshFromBinaryFile("../Data/Fbx/MovingPlatform.skn");

		m_pStaticMesh = new RenderMesh();
		m_pStaticMesh->InitWithData(pMeshData);
		
		m_pStatcMeshTexture = GetRenderDevice()->CreateRendTexture();
		m_pStatcMeshTexture->Load("Data/Fbx/PlatformTexture.tga");
		
	}

	void SampleFbxImport::LoadParticles()
	{
		m_pEmitter = ParticleEmitter::create("Data/Particle/fire.particle");
		m_pEmitter->start();
	}

	void SampleFbxImport::Init(const Platform* pPlatform)
	{
		Sample::Init(pPlatform);
		
		Vector3 vEyePos = Vector3(0, 600, 800);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		m_pCamera->LookAt(vEyePos,VAtPos,vUp);

		//FBXImporter fbxImpor;
		//fbxImpor.Initialize();

		//LoadSaticMesh(fbxImpor);

		//LoadSkelMesh(/*fbxImpor*/);

		//LoadBoxMesh(fbxImpor);

		LoadParticles();

	}

	void SampleFbxImport::LoadBoxMesh(/*FBXImporter& fbxImpor*/)
	{
		MeshData* pMeshData = new MeshData;
		//fbxImpor.LoadStaticMeshData("../Data/Fbx/Box.fbx",pMeshData);
		//pMeshData->Save("../Data/Fbx/Box.skn");

		pMeshData = new MeshData();
		pMeshData->Load("Data/Fbx/Box.skn");

		m_pBoxMesh = new RenderMesh();
		m_pBoxMesh->InitWithData(pMeshData);

		m_pBoxTexture = GetRenderDevice()->CreateRendTexture();
		m_pBoxTexture->Load("Data/Fbx/Box.tga");

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
		}
	}

	void SampleFbxImport::Render()
	{
		if (GetRenderDevice() == NULL)
			return;

		// render skelMesh
		if (m_pRenderMesh && m_pAnimtionPlay)
		{
			Matrix4x4 matWorld;
			MatrixTranslation(&matWorld,-50,0,0);
	
			Matrix4x4* skinMatrix = m_pAnimtionPlay->GetSkinMatrixArray();
			UINT nNumber = m_pAnimtionPlay->GetSkinMatrixNumber();

			Matrix4x4 matWVP = matWorld * m_pCamera->GetViewProjMatrix();

			m_pDefaultTech->Begin(ShderFLag_SKIN);

			m_pDefaultTech->SetMatrixArray("u_matrixPalette",skinMatrix,nNumber);
			m_pDefaultTech->SetMatrix("u_worldViewProjectionMatrix",&matWVP);
			//m_pDefaultTech->SetTexture("u_diffuseTexture",m_pRendTexture);

			//m_pDefaultTech->CommitChanges();
							
			m_pRenderMesh->Draw(m_pDefaultTech);
			//GetRenderDevice()->DrawRenderMesh(m_pRenderMesh,m_pDefaultTech);

			m_pDefaultTech->End(); 
		}


		// render staticMesh
		if (m_pStaticMesh)
		{
			Matrix4x4 matWorld;
			Matrix4x4 matScale;
			MatrixScaling(&matScale,50,50,50); 
			MatrixTranslation(&matWorld,0,0,0);
			matWorld = matScale * matWorld;
			Matrix4x4 matWVP = matWorld * m_pCamera->GetViewProjMatrix();

			m_pDefaultTech->Begin();

			m_pDefaultTech->SetMatrix("u_worldViewProjectionMatrix",&matWVP);
			m_pDefaultTech->SetTexture("u_diffuseTexture",m_pStatcMeshTexture);

			m_pDefaultTech->CommitChanges();

			GetRenderDevice()->DrawRenderMesh(m_pStaticMesh,m_pDefaultTech);
			//m_pStaticMesh->Draw();

			m_pDefaultTech->End();
		}

		if (m_pBoxMesh)
		{
			Matrix4x4 matWorld;
			Matrix4x4 matScale;
			MatrixScaling(&matScale,200,200,200); 
			MatrixTranslation(&matWorld,0,0,0);
			matWorld = matScale * matWorld;
			Matrix4x4 matWVP = matWorld * m_pCamera->GetViewProjMatrix();

			m_pDefaultTech->Begin();

			m_pDefaultTech->SetMatrix("u_worldViewProjectionMatrix",&matWVP);
			m_pDefaultTech->SetTexture("u_diffuseTexture",m_pBoxTexture);

			m_pDefaultTech->CommitChanges();

			GetRenderDevice()->DrawRenderMesh(m_pBoxMesh,m_pDefaultTech);
			//m_pStaticMesh->Draw();

			m_pDefaultTech->End();
		}

		if (m_pEmitter)
		{
// 			Matrix4x4 matWorld;
// 			Matrix4x4 matScale;
// 			MatrixScaling(&matScale,200,200,200); 
// 			MatrixTranslation(&matWorld,0,0,0);
// 			matWorld = matScale * matWorld;
// 			Matrix4x4 matWVP = matWorld * m_pCamera->GetViewProjMatrix();
// 
// 			m_pDefaultTech->Begin();
// 
// 			m_pDefaultTech->SetMatrix("u_worldViewProjectionMatrix",&matWVP);
// 			m_pDefaultTech->SetTexture("u_diffuseTexture",m_pBoxTexture);
// 
// 			m_pDefaultTech->CommitChanges();

			//GetRenderDevice()->Draw(m_pBoxMesh,m_pDefaultTech);
			//m_pStaticMesh->Draw();
			Matrix4x4 matWorld;
			MatrixIdentity(&matWorld);
			m_pEmitter->update(GetTimer()->GetFrameDeltaTime(),matWorld);
			m_pEmitter->draw(m_pCamera);
			

			//m_pDefaultTech->End();
		}

	}

}


