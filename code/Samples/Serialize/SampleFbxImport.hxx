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
		if (1)
		{
			{
				LoadSkeletonMeshData("../../Data/Fbx/TestBull.fbx");
			}


			// Material
			{
				MaterialData* pMatData = new MaterialData();
				pMatData->m_strShaderName = "default";
				pMatData->m_strShaderMacro = "DIFFUSE;SKIN; SKIN_MATRIX_COUNT 55";

				MaterialParameter* pMatParmTex = new MaterialParameter("u_texture");
				SamplerState diff;
				diff.SetTexture("FBX/TestBull_DM.png");
				pMatParmTex->setSampler(&diff);
				pMatData->m_arrParameters.push_back(pMatParmTex);

				MaterialParameter* pMatColor = new MaterialParameter("u_cDiffuseColor");
				pMatColor->setVector4(Vector4(1,1,1,1));
				pMatData->m_arrParameters.push_back(pMatColor);

				pMatData->SaveToFile("../../Data/Fbx/TestBull.mat");
			}

			// AnimationSet
			{
				AnimationSetData* pAniSetData = new AnimationSetData();

				pAniSetData->AddAnimClip("Fbx/TestBull.ska","TestAnim");

				pAniSetData->SaveToFile("../../Data/Fbx/TestBull.aniset");
			}
		}


		GameObject* pTestBull = GetEntitySystem()->CreateGameObject("testBull");

		MeshComponent* pMeshComp = pTestBull->CreateComponent<MeshComponent>();
		pMeshComp->Load("FBX/TestBull.skn","FBX/TestBull.mat");

		IAnimationObject* pAnimationObject = pTestBull->CreateComponent<IAnimationObject>();
		pAnimationObject->Load("FBX/TestBull.aniset","FBX/TestBull.ske");
		pAnimationObject->PlayAnimation("TestAnim");

		pTestBull->GetSceneNode()->Scale(1.0f / 50.0f);
	}

	void SampleFbxImport::LoadSaticMesh()
	{
		if (1)
		{
			//LoadStaticMeshData("../../Data/FBX/shpere.FBX");

			LoadStaticMeshData("../../Data/FBX/MovingPlatform.fbx");

			// Material
			{
				MaterialData* pMatData = new MaterialData();
				pMatData->m_strShaderName = "default";
				pMatData->m_strShaderMacro = "DIFFUSE";

				MaterialParameter* pMatParmTex = new MaterialParameter("u_texture");
				SamplerState diff;
				diff.SetTexture("Fbx/PlatformTexture.tga");
				pMatParmTex->setSampler(&diff);
				pMatData->m_arrParameters.push_back(pMatParmTex);

				MaterialParameter* pMatColor = new MaterialParameter("u_cDiffuseColor");
				pMatColor->setVector4(Vector4(1,1,1,1));
				pMatData->m_arrParameters.push_back(pMatColor);

				pMatData->SaveToFile("../../Data/Fbx/MovingPlatform.mat");
			}
		}

		
		GameObject* pPlatform = GetEntitySystem()->CreateGameObject("Platform");

		MeshComponent* pMesh = pPlatform->CreateComponent<MeshComponent>();
		pMesh->Load("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mat");
	}


	void SampleFbxImport::Load()
	{
		//Vector3 vEyePos = Vector3(0, 600, 800);
		Vector3 vEyePos = Vector3(0, 40, 40);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos,vUp);

		GetLightSystem()->SetAmbientColor(Vector4(0.1f,0.1f,0.1f,0.1f));

		FBXImporterModuleInit();

		LoadSaticMesh();

		//LoadSkelMesh();
		
	}

	void SampleFbxImport::UnLoad()
	{
		FBXImporterModuleShutdown();
	}

}


