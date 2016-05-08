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
		if (1)
		{
			LoadSkeletonMeshData("Fbx/TestBull.fbx");
		}

		if (0)
		{
			//LoadAnimationData("Fbx/TestBull.fbx",const Skeleton& skeData,
			//	const char* pOutSkaFile = NULL);
		}

		CreateMeshMaterial("FBX/TestBull_DM.png","FBX/TestBull.mtl","SKIN");

		RefPtr<SceneNode> pTestBull = m_pScene->CreateSceneNode();

		RefPtr<SkinMeshComponent>  pMeshComp = pTestBull->CreateComponent<SkinMeshComponent>();
		pMeshComp->Load("FBX/TestBull.skn","FBX/TestBull.mtl");
			
		m_pAnimComponent = pTestBull->CreateComponent<AnimationComponent>();
		m_pAnimComponent->SetSkeletonPath("FBX/TestBull.ske");
		m_pAnimComponent->PlayAnimation( CreateClipNode("Fbx/TestBull.ska").get() );

		//m_pAnimComponent = pAnimationObject.get();

		pTestBull->SetScale(Vector3(0.01f));

		//m_pScene->GetRootNode()->AddChild(pTestBull.get());
	}


	void SampleFbxImport::LoadSaticMesh()
	{
		LoadStaticMeshData("FBX/shpere.FBX");
				
 		LoadStaticMeshData("FBX/Box.FBX");

		LoadStaticMeshData("FBX/MovingPlatform.fbx");

		CreateMeshMaterial("FBX/Box.tga","FBX/Box.mtl","LIGHT");
			
		CreateMeshMaterial("FBX/PlatformTexture.tga","FBX/MovingPlatform.mtl");

		RefPtr<SceneNode> pPlatform = m_pScene->CreateSceneNode();
		RefPtr<MeshComponent> pMesh = pPlatform->CreateComponent<MeshComponent>();
		pMesh->Load("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mtl");
 
		RefPtr<SceneNode> pBox = m_pScene->CreateSceneNode();
		RefPtr<MeshComponent> pBoxMesh = pBox->CreateComponent<MeshComponent>();
		pBoxMesh->Load("Fbx/Box.skn","Fbx/Box.mtl");

		RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();
		RefPtr<MeshComponent> pShpereMesh = pShpere->CreateComponent<MeshComponent>();
		pShpereMesh->Load("Fbx/shpere.skn","Fbx/Box.mtl");
			
	}

	void SampleFbxImport::Render()
	{
		//if (m_pAnimComponent)
		//	m_pAnimComponent->DebugRender();

		Transform tsf;
		LineRender::DrawTransform(tsf);
	}

	void SampleFbxImport::Load()
	{
		//Vector3 vEyePos = Vector3(0, 600, 800);
		Vector3 vEyePos = Vector3(0, -10, 0);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->LookAt(vEyePos,VAtPos);

		//LoadSaticMesh();

		LoadSkelMesh();	

		m_pScene->GetDirLight()->GetSceneNode()->LookAt(Vector3(0,-10,0),Vector3(0,0,0));
		m_pScene->GetDirLight()->SetLightColor(ColourValue(1.0,1.0,1.0,1.0f));
		m_pScene->SetAmbientColor(Vector3(0.2,0.2,0.2));
	}

	void SampleFbxImport::UnLoad()
	{
		m_pAnimComponent = NULL;
	}

}


