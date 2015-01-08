#include "Samples/Serialize/SampleSceneSerialize.h"

namespace ma
{
	SampleSceneSerialize::SampleSceneSerialize()
	{
		m_bLoadOver = false;
	}

	void SampleSceneSerialize::Load()
	{
		Vector3 vEyePos = Vector3(0, 15, 20);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos);

		GetInput()->AddKeyListener(this);

		//GetResourceSystem()->SetDataThreadEnable(true);

		{
			MaterialData matData;

			SubMaterialData& subMatData = matData.AddSubMatData();
			subMatData.SetShaderName("default");
			subMatData.SetShderMacro("DIFFUSE;DIFFUSECOLOR");
			subMatData.GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;

			RefPtr<SamplerState> pDiff = CreateSamplerState();
			pDiff->SetTexturePath("FBX/Box.tga");

			subMatData.AddParameter("u_texture", Any(pDiff) );
			subMatData.AddParameter("u_cDiffuseColor", Any( Vector4(1,0,0,0) ) );

			matData.SaveToXML("Fbx/Box.mat");

			SceneNodePtr pGameObj = m_pScene->CreateNode("Test");  
			MeshComponentPtr pMehBox = pGameObj->CreateComponent<MeshComponent>();
			pMehBox->Load("Fbx/Box.skn","Fbx/box.mat");
			
			pMehBox->GetSubMaterial(0)->GetShadingTechnqiue()->SetParameter("u_cDiffuseColor",Any( Vector4(0,1,0,0) ));
			

			int nClone = 5;
			for (int i = 0; i < nClone; ++i)
			{
				std::string pName = pGameObj->GetName();
				pName += std::string("_clone"); // + StringConverter::toString(i);
				SceneNodePtr pClone = pGameObj->Clone(pName.c_str());
				pClone->Translate(Vector3(2 * (i + 1),0,0));
			}
		}
	
		std::string strScenePath = "scene/Test.scene.xml";
		

		// test Save
		{
			m_pScene->SaveToXML(strScenePath.c_str());
		}
		
 		{	
// 			GetCameraControll()->SetCamera(NULL);
//  			m_pScene->Reset();
//  			m_pScene->LoadFromXML(strScenePath.c_str());
//  			OnLoadOver();
 		}
	}

	void SampleSceneSerialize::UnLoad()
	{
		GetInput()->RemoveKeyListener(this);
	}

	bool SampleSceneSerialize::OnLoadOver()
	{
		if (m_bLoadOver)
			return true;

		ASSERT(m_pScene);
		if (m_pScene && !m_pScene->IsInited())
			return false;

		SceneNode* pCamaerNode = m_pScene->FindChildNode("defaultCamera");
		ASSERT(pCamaerNode);
		if (pCamaerNode == NULL)
			return false;

		m_pCamera = pCamaerNode->GetTypeComponent<Camera>();
		ASSERT(m_pCamera);
		GetCameraControll()->SetCamera(m_pCamera.get());

		m_pScene->SetCamera(m_pCamera.get());

		m_bLoadOver = true;

		return true;
	}

	void SampleSceneSerialize::Update()
	{
		OnLoadOver();
	}

	bool SampleSceneSerialize::keyPressed(const OIS::KeyEvent &arg)
	{
		std::string strScenePath = "scene/Test.scene";

		if (arg.key == OIS::KC_X)
		{
			{
// 				XMLOutputSerializer arOut;
// 				m_pScene->Serialize(arOut);
// 				arOut.Save(strScenePath.c_str());
				m_pScene->SaveToFile(strScenePath.c_str());
			}

			{
// 				XMLInputSerializer arIn;
// 				bool bOpenOK = arIn.Open(strScenePath.c_str());
// 				ASSERT(bOpenOK);
// 				m_pScene->Serialize(arIn);
// 				arIn.Close();
				m_pScene->LoadFromFile(strScenePath.c_str());
			}
		}

		return true;
	}
}


