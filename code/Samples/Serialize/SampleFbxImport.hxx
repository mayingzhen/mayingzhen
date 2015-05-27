#include "Samples/Serialize/SampleFbxImport.h"
#include "Plugin/FbxImport/Module.h"
#include "SampleBrowser.h"

namespace ma
{

	SampleFbxImport::SampleFbxImport()
	{
		m_pAnimComponent = NULL;
	}

	void SampleFbxImport::LoadSkelMesh()
	{
		if (0)
		{
			
			LoadSkeletonMeshData("Fbx/TestBull.fbx");

			// Material
			{
				CreateDefaultMaterial("FBX/TestBull_DM.png","FBX/TestBull.mat","SKIN");
			}
		}


		RefPtr<SceneNode> pTestBull = CreateSceneNode();

		RefPtr<SkinMeshComponent>  pMeshComp = pTestBull->CreateComponent<SkinMeshComponent>();
		pMeshComp->Load("FBX/TestBull.skn","FBX/TestBull.mat");
			
		RefPtr<AnimationComponent> pAnimationObject = pTestBull->CreateComponent<AnimationComponent>();
		pAnimationObject->SetSkeletonPath("FBX/TestBull.ske");
		pAnimationObject->PlayAnimation( CreateClipNode("Fbx/TestBull.ska").get() );

		m_pAnimComponent = pAnimationObject.get();

		pTestBull->SetScale(Vector3(1.0f / 50.0f));

		m_pScene->GetRootNode()->AddChild(pTestBull.get());
	}


	void SampleFbxImport::LoadSaticMesh()
	{
		if (1)
		{
			LoadStaticMeshData("FBX/shpere.FBX");
				
			LoadStaticMeshData("FBX/Box.FBX");

			LoadStaticMeshData("FBX/MovingPlatform.fbx");

			CreateDefaultMaterial("FBX/Box.tga","FBX/Box.mat");
			
			CreateDefaultMaterial("FBX/PlatformTexture.tga","FBX/MovingPlatform.mat");
		}

		
		RefPtr<SceneNode> pPlatform = CreateSceneNode();
		RefPtr<MeshComponent> pMesh = pPlatform->CreateComponent<MeshComponent>();
		pMesh->Load("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mat");
		//m_pScene->GetRootNode()->AddChild(pPlatform.get());

		RefPtr<SceneNode> pBox = CreateSceneNode();
		RefPtr<MeshComponent> pBoxMesh = pBox->CreateComponent<MeshComponent>();
		pBoxMesh->Load("Fbx/Box.skn","Fbx/Box.mat");
		//m_pScene->GetRootNode()->AddChild(pBox.get());	

		RefPtr<SceneNode> pShpere = CreateSceneNode();
		RefPtr<MeshComponent> pShpereMesh = pShpere->CreateComponent<MeshComponent>();
		pShpereMesh->Load("Fbx/shpere.skn","Fbx/Box.mat");
		m_pScene->GetRootNode()->AddChild(pShpere.get());
			
	}

	void SampleFbxImport::Render()
	{
		if (m_pAnimComponent)
			m_pAnimComponent->DebugRender();

		Transform tsf;
		GetLineRender()->DrawTransform(tsf);
	}

	void SampleFbxImport::Load()
	{
		//Vector3 vEyePos = Vector3(0, 600, 800);
		Vector3 vEyePos = Vector3(10, 10, -10);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos);

		LoadSaticMesh();

		LoadSkelMesh();	

		m_pScene->GetSunShaow()->GetSceneNode()->LookAt(Vector3(1.f, 1.f, -0.f),Vector3::ZERO); 
	}

	void SampleFbxImport::UnLoad()
	{
	}

}


