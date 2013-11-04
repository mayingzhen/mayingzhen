#include "Samples/Serialize/SampleFbxImport.h"
#include "Animation/Module.h"
#include "D3D9Render/Module.h"
#include "Plugin/FbxImport/Module.h"

namespace ma
{
	SampleFbxImport::SampleFbxImport()
	{
		m_pAnimtionObject = NULL;
		m_pSkeleton = NULL;
		m_pRenderMesh = NULL;

		m_pStaticMesh = NULL;

		m_pBoxMesh = NULL;

		m_fFame = 0.5;
	}

	void SampleFbxImport::LoadSkelMesh(FBXImporter& fbxImpor)
	{
		MeshData* pMeshData = new MeshData;
		Skeleton* pSkeData = new Skeleton;
		Animation* pAnimData = new Animation;
		fbxImpor.LoadSkeletonMeshData("../../Data/Fbx/TestBull_anim.fbx",pMeshData,pSkeData);
		fbxImpor.LoadAnimationData("../../Data/Fbx/TestBull_anim.fbx",pAnimData,pSkeData);

		// Save
		pMeshData->SaveToFile("../../Data/Fbx/TestBull.skn");
		pSkeData->SaveToFile("../../Data/Fbx/TestBull.ske");
		pAnimData->SaveToFile("../../Data/Fbx/TestBull.ska");
		SAFE_DELETE(pMeshData);
		SAFE_DELETE(pSkeData);
		SAFE_DELETE(pAnimData);


		//m_pRenderMesh = new RenderMesh();
		//m_pRenderMesh->Load("FBX/TestBull.skn","FBX/TestBull_DM.png");        

		//m_pAnimtionObject =  GetAnimationSystem()->CreateAnimationObject("FBX/TestBull.ske",NULL);
		IAnimationSet* pAnimSet = m_pAnimtionObject->GetAnimationSet();
		pAnimSet->AddAnimClip("FBX/TestBull.ska","TestAction");
		m_pAnimtionObject->PlayAnimation("TestAction");
	}

	void SampleFbxImport::LoadSaticMesh(FBXImporter& fbxImpor)
	{
		MeshData* pMeshData = new MeshData;
		fbxImpor.LoadStaticMeshData("../../Data/Fbx/MovingPlatform.fbx",pMeshData);
		pMeshData->SaveToFile("../../Data/Fbx/MovingPlatform.skn");
		SAFE_DELETE(pMeshData);

		//m_pStaticMesh = new RenderMesh();
		m_pStaticMesh->Load("Fbx/MovingPlatform.skn","Fbx/PlatformTexture.tga");

		{
			MeshData* pBoxData = new MeshData;
			fbxImpor.LoadStaticMeshData("../../Data/Fbx/Box.fbx",pBoxData);
			pBoxData->SaveToFile("../../Data/Fbx/Box.skn");
			SAFE_DELETE(pBoxData);

			//m_pBoxMesh = new RenderMesh();
			m_pBoxMesh->Load("Fbx/Box.skn","Fbx/Box.tga");
		}

		{
			MeshData* pSphereData = new MeshData;
			fbxImpor.LoadStaticMeshData("../../Data/Fbx/shpere.fbx",pSphereData);
			pSphereData->SaveToFile("../../Data/Fbx/shpere.skn");
			SAFE_DELETE(pSphereData);

			//m_pSphereMesh = new RenderMesh();
			m_pSphereMesh->Load("Fbx/shpere.skn","Fbx/Box.tga");
		}

	}


	void SampleFbxImport::Load()
	{
		//Vector3 vEyePos = Vector3(0, 600, 800);
		Vector3 vEyePos = Vector3(0, 40, 40);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos,vUp);

		//float fZoomSpeed = GetCameraControll()->GetZoomSpeed();
		//GetCameraControll()->SetZoomSpeed(fZoomSpeed * 0.1);

		FBXImporter fbxImpor;
		fbxImpor.Initialize();

		LoadSaticMesh(fbxImpor);

		//LoadSkelMesh(fbxImpor);
		
	}

	void SampleFbxImport::UnLoad()
	{
		SAFE_DELETE(m_pRenderMesh);
		SAFE_DELETE(m_pBoxMesh);
		SAFE_DELETE(m_pBoxMesh);
	}


	void SampleFbxImport::Render()
	{
		if (m_pAnimtionObject)
		{
			//m_pAnimtionObject->AdvanceTime(fTimeElapsed);
			//if (GetInput()->IsKeyDown(OIS::KC_A))
			//	m_pAnimtionObject->SetFrame(++m_fFame);
			//if (GetInput()->IsKeyDown(OIS::KC_D))
			//	m_pAnimtionObject->SetFrame(--m_fFame);

			m_pAnimtionObject->AdvanceTime(GetTimer()->GetFrameDeltaTime());
			m_pAnimtionObject->EvaluateAnimation(1.0f);
		

// 			Matrix4x4* skinMatrix = m_pAnimtionObject->GetSkinMatrixArray();
// 			UINT nNumber = m_pAnimtionObject->GetSkinMatrixNumber();
// 
// 			m_pRenderMesh->SetSkinMatrix(skinMatrix,nNumber);
		}

		if (m_pRenderMesh)
		{
			Matrix4x4 matWorld;
			MatrixTranslation(&matWorld,20,0,0);
			Matrix4x4 matScale;
			MatrixScaling(&matScale, 1.0f / 50.0f, 1.0f / 50.0f, 1.0f / 50.0f); 

			matWorld = matScale * matWorld;
			m_pRenderMesh->SetWorldMatrix(matWorld);

			//m_pRenderMesh->Render();

		}

		if (m_pStaticMesh)
		{
			Matrix4x4 matWorld;
			MatrixTranslation(&matWorld,0,0,0);

			m_pStaticMesh->SetWorldMatrix(matWorld);

			//m_pStaticMesh->Render();
		}

		if (m_pBoxMesh)
		{
			Matrix4x4 matWorld;
			Matrix4x4 matScale;
			MatrixScaling(&matScale,1,1,1); 
			MatrixTranslation(&matWorld,0,0,0);
			matWorld = matScale * matWorld;

			m_pBoxMesh->SetWorldMatrix(matWorld);

			//m_pBoxMesh->Render();
		}
	}

}


