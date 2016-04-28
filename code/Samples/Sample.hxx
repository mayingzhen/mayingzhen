#include "Sample.h"

namespace ma
{
	Sample::Sample(const char* pszName)
	{
		m_pScene = GetRenderSystem()->GetScene();
		m_pCamera = m_pScene->GetCamera();


		int nWndWidth,nWndHeigh;
		Platform::GetInstance().GetWindowSize(nWndWidth,nWndHeigh);
		float fFOV = DegreesToRadians(50.0f);
		float fAspect = (float)nWndWidth / (float)nWndHeigh;
		float fNearClip = 0.10f;
		float fFarClip = 1000.0f;
		m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);

		Game::GetInstance().mkeyEvent.notify(this,&Sample::keyEvent);
	}

	Sample::~Sample()
	{
		Game::GetInstance().mkeyEvent.remove(this,&Sample::keyEvent);
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
		
		std::string strMacro = "DIFFUSECOLOR";
		strMacro = pszAddMacro ? strMacro + ";" + pszAddMacro : strMacro;
		pSubMaterial->SetShadingTechnqiue("mesh",strMacro.c_str());

		pSubMaterial->SetParameter("u_texture", Any( CreateSamplerState(pszTexture) ) );
		pSubMaterial->SetParameter("u_cDiffuseColor", Any( Vector4(1,1,1,1) ) );
		pSubMaterial->SetParameter("u_cSpecColor", Any( Vector4(1,1,1,1) ) );
		pSubMaterial->SetParameter("u_specPower", Any(10.0f));

		pMaterial->SaveToFile(pMatPath);
	}

}

