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
		m_pRenderMaterial = NULL;

		m_pStaticMesh = NULL;
		//m_pStatcMeshTexture = NULL;
		m_pStaticMeshMaterial = NULL;

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
        strPath = std::string( FileSystem::getResourcePath() ) + "FBX/TestBull.skn";
		pMeshData->Load(strPath.c_str());
		//pMeshData = LoadMeshFromBinaryFile("../Data/Fbx/TestBull.skn");
        strPath = std::string( FileSystem::getResourcePath() ) + "FBX/TestBull.ske";
		pSkeData = LoadSkeletonFromBinaryFile(strPath.c_str());
		//pAnimData = LoadAnimationFromBinaryFile("../Data/Fbx/TestBull.ska");

		m_pRenderMesh = new RenderMesh();
		m_pRenderMesh->InitWithData(pMeshData);

		Texture* pRendTexture = GetRenderDevice()->CreateRendTexture();
        strPath = std::string( FileSystem::getResourcePath() ) + "FBX/TestBull_DM.png";
		pRendTexture->Load(strPath.c_str());
		Sampler* sampler = Sampler::create(pRendTexture); // +ref texture

		ShaderProgram* pShaderProgram = GetRenderDevice()->CreateShaderProgram();
		pShaderProgram->CreateFromShaderName("default","SKIN;SKIN_MATRIX_COUNT 55;DIFFUSE");
		m_pRenderMaterial = Material::create(pShaderProgram);
		m_pRenderMaterial->getParameter("u_texture")->SetValue(sampler);

		m_pRenderMesh->SetMaterial(m_pRenderMaterial);

		m_pSkeleton = new Skeleton();
		m_pSkeleton->InitWithData(pSkeData);

		m_pAnimtionPlay = new AnimationPlay(m_pSkeleton);
         strPath = std::string( FileSystem::getResourcePath() ) + "FBX/TestBull.ska";
		m_pAnimtionPlay->AddAction(strPath.c_str(),"TestAction");
		m_pAnimtionPlay->PlayAnimation("TestAction");
	}

	void SampleFbxImport::LoadSaticMesh(FBXImporter& fbxImpor)
	{
		MeshData* pMeshData = new MeshData;

		//fbxImpor.LoadStaticMeshData("../Data/Fbx/MovingPlatform.fbx",pMeshData);

		//SaveMeshToBinaryFile("../Data/Fbx/MovingPlatform.skn",pMeshData);
		//SAFE_DELETE(pMeshData);

		pMeshData = new MeshData();
		pMeshData->Load("../../Data/Fbx/MovingPlatform.skn");
		//pMeshData = LoadMeshFromBinaryFile("../Data/Fbx/MovingPlatform.skn");

		m_pStaticMesh = new RenderMesh();
		m_pStaticMesh->InitWithData(pMeshData);
		
		Texture* pTexture = GetRenderDevice()->CreateRendTexture();
		pTexture->Load("../../Data/Fbx/PlatformTexture.tga");

		ShaderProgram* pShaderProgram = GetRenderDevice()->CreateShaderProgram();
		pShaderProgram->CreateFromShaderName("default","DIFFUSE");
		m_pStaticMeshMaterial = Material::create(pShaderProgram);
		Sampler* sampler = Sampler::create(pTexture); // +ref texture
		m_pStaticMeshMaterial->getParameter("u_texture")->SetValue(sampler);

		m_pStaticMesh->SetMaterial(m_pStaticMeshMaterial);
		
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

		//LoadSkelMesh(fbxImpor);

		//LoadBoxMesh(fbxImpor);

		LoadParticles();

	}

	void SampleFbxImport::LoadBoxMesh(FBXImporter& fbxImpor)
	{
		MeshData* pMeshData = new MeshData;
		fbxImpor.LoadStaticMeshData("../../Data/Fbx/Box.fbx",pMeshData);
		pMeshData->Save("../../Data/Fbx/Box.skn");

		pMeshData = new MeshData();
		pMeshData->Load("../../Data/Fbx/Box.skn");

		m_pBoxMesh = new RenderMesh();
		m_pBoxMesh->InitWithData(pMeshData);

		Texture* pTexture = GetRenderDevice()->CreateRendTexture();
		pTexture->Load("../../Data/Fbx/Box.tga");

		ShaderProgram* pShaderProgram = GetRenderDevice()->CreateShaderProgram();
		pShaderProgram->CreateFromShaderName("default","DIFFUSE");
		m_pBoxMaterial = Material::create(pShaderProgram);
		Sampler* sampler = Sampler::create(pTexture); // +ref texture
		m_pBoxMaterial->getParameter("u_texture")->SetValue(sampler);

		m_pBoxMesh->SetMaterial(m_pBoxMaterial);

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
	
		if (m_pEmitter)
		{	
			m_pEmitter->update(fTimeElapsed);
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

// 			Matrix4x4 matScale;
// 			MatrixScaling(&matScale, 1.0f / 50.0f, 1.0f / 50.0f, 1.0f / 50.0f); 
// 
// 			matWorld = matScale * matWorld;

	
			Matrix4x4* skinMatrix = m_pAnimtionPlay->GetSkinMatrixArray();
			UINT nNumber = m_pAnimtionPlay->GetSkinMatrixNumber();

			Matrix4x4 matWVP = matWorld * m_pCamera->GetViewProjMatrix();

			m_pRenderMaterial->getParameter("u_worldViewProjectionMatrix")->SetValue(matWVP);
			m_pRenderMaterial->getParameter("u_matrixPalette")->SetValue(skinMatrix,nNumber);
			//m_pRenderMesh->Draw();
		}


		// render staticMesh
		if (m_pStaticMesh)
		{
			Matrix4x4 matWorld;
			//Matrix4x4 matScale;
			//MatrixScaling(&matScale,50,50,50); 
			MatrixTranslation(&matWorld,0,0,0);
 			//matWorld = matScale * matWorld;

			Matrix4x4 matWVP = matWorld * m_pCamera->GetViewProjMatrix();

			m_pStaticMeshMaterial->getParameter("u_worldViewProjectionMatrix")->SetValue(matWVP);

			m_pStaticMesh->Draw();
		}

		if (m_pBoxMesh)
		{
			Matrix4x4 matWorld;
			Matrix4x4 matScale;
			MatrixScaling(&matScale,200,200,200); 
			MatrixTranslation(&matWorld,0,0,0);
			matWorld = matScale * matWorld;
			Matrix4x4 matWVP = matWorld * m_pCamera->GetViewProjMatrix();

			m_pBoxMaterial->getParameter("u_worldViewProjectionMatrix")->SetValue(matWVP);

			m_pBoxMesh->Draw();
		}

		if (m_pEmitter)
		{
			Matrix4x4 matWorld;
			MatrixTranslation(&matWorld,-20,0,0);
				
			m_pEmitter->draw(m_pCamera,matWorld);
		
		}

	}

}


