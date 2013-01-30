#include "Samples/FbxImport/FbxImport.h"

namespace ma
{

	void SampleFbxImport::Init(int windID)
	{
		DxRenderModuleInit();
		AnimationModuleInit();

		ma::DxRenderDevice* pDxRenderDevice = (ma::DxRenderDevice*)ma::GetRenderDevice();
		pDxRenderDevice->Init(windID);

		ma::DxRender* pDxRender = (ma::DxRender*)ma::GetRender();
		pDxRender->InitDefaultShader();
	}

	void SampleFbxImport::Shutdown()
	{
		DxRenderModuleShutdown();
		AnimationModuleShutdown();
	}

	void SampleFbxImport::Load()
	{
		FBXImporter fbxImpor;
		fbxImpor.Initialize();
		MeshData* pMeshData = new MeshData;
		SkeletonData* pSkeData = new SkeletonData;
		std::vector<AnimationData*> vAnimData;
		fbxImpor.LoadScene("../Fbx/TestBull_anim.fbx"/*"../Fbx/Naruto/NarutoYUP.FBX"*/,pMeshData,pSkeData,vAnimData);

		//GameObject* pGameObj = new GameObject(m_pScene,"Fbx");
		//pRootNode->AddChildNode(pGameObj);

		//SkelMeshComponent* pSkelMeshComp = new SkelMeshComponent();
		//pGameObj->AddComponent(pSkelMeshComp);

		//MeshComponent* pMeshComp = new MeshComponent;
		//pSkelMeshComp->AddMeshComp(pMeshComp);
		//pGameObj->AddComponent(pMeshComp);

		//MeshRes* pMeshRes = new MeshRes(); 
		//pMeshComp->SetMeshRes(pMeshRes);

		DxRendMesh* pRendMesh = new DxRendMesh();
		pRendMesh->InitWithData(pMeshData);
		//pMeshRes->SetRendMesh(pRendMesh);

		const char* pTexPath = "../Fbx/TestBull_DM.png";//"../Fbx/Naruto/Naruto.fbm/FbxTemp_0003.jpg";
		DxRendTexture* pTexture = new DxRendTexture();
		pTexture->Load(pTexPath);
		//pMeshComp->SetTexture(pTexture);

		Skeleton* pSkele = new Skeleton();
		pSkele->InitWithData(*pSkeData);
		//pSkelMeshComp->SetSkeleton(pSkele);

		Animation* pAnimation = new Animation;
		pAnimation->InitWithData(vAnimData[0]);
		pAnimation->ConverteAnimDataParentToLocalSpaceAnimation(pSkele);
		AnimationInst* pAnimInst = new AnimationInst(pAnimation,pSkele);

		AnimationSet* pAnimSet = new AnimationSet();
		pAnimSet->AddAnimationInst(pAnimInst,"xxx");

		//pSkelMeshComp->SetAnimationSet(pAnimSet);
		//pSkelMeshComp->PlayAnimation("xxx");
	}

	void SampleFbxImport::Unload()
	{

	}

	void SampleFbxImport::Tick(float timeElapsed)
	{

	}

	void SampleFbxImport::Render()
	{

	}

	void SampleFbxImport::OnResize(int w,int h)
	{

	}
}


