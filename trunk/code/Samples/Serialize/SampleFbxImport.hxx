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

			// Material
			{
				MaterialData matData;
			
				SubMaterialData& subMatData = matData.AddSubMatData();
				subMatData.SetShaderName("default");
				subMatData.SetShderMacro("DIFFUSE;DIFFUSECOLOR;SKIN");
				subMatData.GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;

				RefPtr<SamplerState> pDiff = CreateSamplerState();
				pDiff->SetTexturePath("FBX/TestBull_DM.png");

				subMatData.AddParameter("u_texture", Any(pDiff) );
				subMatData.AddParameter("u_cDiffuseColor", Any( Vector4(1,1,1,1) ) );
		
				matData.SaveToXML("Fbx/TestBull.mat");
			}

			// AnimationSet
			{
				//AnimationSetData animSetData;

				//animSetData.AddAnimClip("Fbx/TestBull.ska","TestAnim");

				//animSetData.SaveToFile("Fbx/TestBull.aniset");
			}
		}


		SceneNodePtr pTestBull = m_pScene->CreateNode("testBull");

		RefPtr<MeshComponent>  pMeshComp = pTestBull->CreateComponent<MeshComponent>();
		pMeshComp->Load("FBX/TestBull.skn","FBX/TestBull.mat");
			
		AnimationComponentPtr pAnimationObject = pTestBull->CreateComponent<AnimationComponent>();
		pAnimationObject->Load("FBX/TestBull.aniset","FBX/TestBull.ske");
		pAnimationObject->PlayAnimation("TestAnim");

		m_pAnimComponent = pAnimationObject.get();

		pTestBull->SetScale(Vector3(1.0f / 50.0f));
	}

	void CreateMaterialData(const char* pszTexture,const char* pMatPath)
	{
		MaterialData matData;

		SubMaterialData& subMatData = matData.AddSubMatData();
		subMatData.SetShaderName("default");
		subMatData.SetShderMacro("DIFFUSE;DIFFUSECOLOR");
		subMatData.GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;

		RefPtr<SamplerState> pDiff = CreateSamplerState();
		pDiff->SetTexturePath(pszTexture);

		subMatData.AddParameter("u_texture", Any(pDiff) );
		subMatData.AddParameter("u_cDiffuseColor", Any( Vector4(1,0,0,0) ) );

		matData.SaveToXML(pMatPath);
	}

	void SampleFbxImport::LoadSaticMesh()
	{
		if (1)
		{
			LoadStaticMeshData("FBX/shpere.FBX");
				
			LoadStaticMeshData("FBX/Box.FBX");

			LoadStaticMeshData("FBX/MovingPlatform.fbx");

			CreateMaterialData("FBX/Box.tga","FBX/Box.mat");
			
			CreateMaterialData("FBX/PlatformTexture.tga","FBX/MovingPlatform.mat");
		}

		
		SceneNodePtr pPlatform = m_pScene->CreateNode("Platform");
		MeshComponentPtr pMesh = pPlatform->CreateComponent<MeshComponent>();
		pMesh->Load("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mat");

		SceneNodePtr pBox = m_pScene->CreateNode("Box");
		MeshComponentPtr pBoxMesh = pBox->CreateComponent<MeshComponent>();
		pBoxMesh->Load("Fbx/Box.skn","Fbx/Box.mat");

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
		Vector3 vEyePos = Vector3(10, -10, 10);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos);

		LoadSaticMesh();

		LoadSkelMesh();	
	}

	void SampleFbxImport::UnLoad()
	{
	}

}


