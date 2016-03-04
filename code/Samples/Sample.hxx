#include "Sample.h"

namespace ma
{
	Sample::Sample(const char* pszName)
	{
		m_pScene = GetRenderSystem()->GetScene();
		m_pCamera = m_pScene->GetCamera();
	}

	Sample::~Sample()
	{
	}

	CameraController* Sample::GetCameraControll()
	{
		return GetSampleBrowser()->GetCameraController();
	}

	Camera*	Sample::GetCamera()
	{
		return m_pCamera.get();
	}

	void Sample::CreateMeshMaterial(const char* pszTexture,const char* pMatPath,char* pszAddMacro)
	{
		RefPtr<Material> pMaterial = CreateMaterial();

		RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
		pMaterial->AddSubMaterial(0,pSubMaterial.get());
		
		std::string strMacro = "DIFFUSE;DIFFUSECOLOR";
		strMacro = pszAddMacro ? strMacro + ";" + pszAddMacro : strMacro;
		pSubMaterial->SetShadingTechnqiue("mesh",strMacro.c_str());

		pSubMaterial->SetParameter("u_texture", Any( CreateTexture(pszTexture) ) );
		pSubMaterial->SetParameter("u_cDiffuseColor", Any( Vector4(1,1,1,1) ) );
		pSubMaterial->SetParameter("u_cSpecColor", Any( Vector4(0,0,0,1) ) );

		pMaterial->SaveToFile(pMatPath);
	}

}

