#include "Samples/Render/SampleMaterial.h"

namespace ma
{
	SampleMaterial::SampleMaterial()
	{
	}

	void SampleMaterial::Load()
	{
		GetCamera()->LookAt(Vector3(0, 5, 3), Vector3(0, 0, 0));

		//GetInput()->AddKeyListener(this);

		//GetResourceSystem()->SetDataThreadEnable(false);
		
		{
			RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();
			RefPtr<MeshComponent> pShpereMesh = pShpere->CreateComponent<MeshComponent>();
			pShpereMesh->Load("Fbx/shpere.skn","Fbx/Box.mtl");
		}

		{
			RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();
			RefPtr<MeshComponent> pShpereMesh = pShpere->CreateComponent<MeshComponent>();
			pShpereMesh->Load("Fbx/shpere.skn","Fbx/Box.mtl");

			RefPtr<Material> pMaterial = CreateMaterial("Fbx/Box.mtl");
			RefPtr<Material> pClone = pMaterial->Clone();
			pClone->GetSubMaterialByIndex(0,0)->GetShadingTechnqiue()->SetShaderMacro("BRDF",true);
			pShpereMesh->SetMaterial(pClone.get());

			pShpere->Translate(Vector3(2,0,0));

		}
		
		m_pScene->GetDirLight()->GetSceneNode()->LookAt(Vector3(0,0,1),Vector3(0,0,0));
		m_pScene->GetDirLight()->SetLightColor(ColourValue(0.5,0.5,0.5,1.0f));
		m_pScene->SetAmbientColor(Vector3(0,0,0));
	}

	void SampleMaterial::UnLoad()
	{
		//GetInput()->RemoveKeyListener(this);
	}

}


