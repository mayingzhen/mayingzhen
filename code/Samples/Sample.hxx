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

	void Sample::CreateDefaultMaterial(const char* pszTexture,const char* pMatPath,char* pszAddMacro)
	{
		RefPtr<Material> pMaterial = CreateMaterial();

		RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
		pMaterial->AddSubMaterial(pSubMaterial.get());
		
		std::string strMacro = "DIFFUSE;DIFFUSECOLOR";
		strMacro = pszAddMacro ? strMacro + ";" + pszAddMacro : strMacro;
		pSubMaterial->SetShaderName("default");
		pSubMaterial->SetShderMacro(strMacro.c_str());

		pSubMaterial->AddParameter("u_texture", Any( CreateTexture(pszTexture) ) );
		pSubMaterial->AddParameter("u_cDiffuseColor", Any( Vector4(1,1,1,1) ) );
		pSubMaterial->AddParameter("u_cSpecColor", Any( Vector4(0,0,0,1) ) );

		pMaterial->SaveToXML(pMatPath);
	}

}

