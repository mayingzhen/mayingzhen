#include "Samples/Serialize/SampleFbxImport.h"
#include "Animation/Module.h"
#include "D3D9Render/Module.h"
#include "Plugin/FbxImport/Module.h"

namespace ma
{
	SampleFbxImport::SampleFbxImport()
	{
	}

	void SampleFbxImport::LoadSkelMesh()
	{
		if (0)
		{
			
			LoadSkeletonMeshData("../../Data/Fbx/TestBull.fbx");


			// Material
			{
				MaterialData matData;
				matData.SetShader("default","DIFFUSE;SKIN; SKIN_MATRIX_COUNT 55");
				
				SamplerState diff;
 				diff.SetTexture("FBX/TestBull_DM.png");
				matData.SetParameter("u_texture",diff);
				matData.SetParameter("u_cDiffuseColor",Vector4(1,1,1,1));

				matData.SaveToFile("../../Data/Fbx/TestBull.mat");
			}

			// AnimationSet
			{
				AnimationSetData animSetData;

				animSetData.AddAnimClip("Fbx/TestBull.ska","TestAnim");

				animSetData.SaveToFile("../../Data/Fbx/TestBull.aniset");
			}
		}


		GameObjectPtr pTestBull = GetEntitySystem()->CreateGameObject("testBull");

		MeshComponentPtr pMeshComp = pTestBull->CreateComponent<MeshComponent>();
		pMeshComp->Load("FBX/TestBull.skn","FBX/TestBull.mat");
		
		
		IAnimationObjectPtr pAnimationObject = pTestBull->CreateComponent<IAnimationObject>();
		pAnimationObject->Load("FBX/TestBull.aniset","FBX/TestBull.ske");
		pAnimationObject->PlayAnimation("TestAnim");

		pTestBull->GetSceneNode()->Scale(1.0f / 50.0f);
	}

	void CreateMaterialData(const char* pszTexture,const char* pMatPath)
	{
		MaterialData matData;
		matData.SetShader("default","DIFFUSE");

		SamplerState diff;
		diff.SetTexture(pszTexture);
		matData.SetParameter("u_texture",diff);
		matData.SetParameter("u_cDiffuseColor",Vector4(1,1,1,1));

		matData.SaveToFile(pMatPath);
	}

	void SampleFbxImport::LoadSaticMesh()
	{
		if (1)
		{
			LoadStaticMeshData("../../Data/FBX/shpere.FBX");
				
			LoadStaticMeshData("../../Data/FBX/Box.FBX");

			LoadStaticMeshData("../../Data/FBX/MovingPlatform.fbx");


			CreateMaterialData("FBX/Box.tga","../../Data/FBX/Box.mat");
			
			CreateMaterialData("FBX/PlatformTexture.tga","../../Data/FBX/MovingPlatform.mat");
		}

		
		GameObjectPtr pPlatform = GetEntitySystem()->CreateGameObject("Platform");
		MeshComponentPtr pMesh = pPlatform->CreateComponent<MeshComponent>();
		pMesh->Load("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mat");

		GameObjectPtr pBox = GetEntitySystem()->CreateGameObject("Box");
		MeshComponentPtr pBoxMesh = pBox->CreateComponent<MeshComponent>();
		pBoxMesh->Load("Fbx/Box.skn","Fbx/Box.mat");

	}


	void SampleFbxImport::Load()
	{
		//Vector3 vEyePos = Vector3(0, 600, 800);
		Vector3 vEyePos = Vector3(0, 40, 40);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos,vUp);

		FBXImporterModuleInit();

		LoadSaticMesh();

		LoadSkelMesh();
		
	}

	void SampleFbxImport::UnLoad()
	{
		FBXImporterModuleShutdown();
	}

}


