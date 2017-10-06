#include "Sample.h"

namespace ma
{
	Sample::Sample(const char* pszName)
	{
		m_pScene = GetRenderSystem()->GetScene();
		m_pCamera = m_pScene->GetCamera();

		Rectangle viewPort = GetRenderSystem()->GetViewPort();
		int nWndWidth = 800;//viewPort.width();
		int nWndHeigh = 600;//viewPort.height();
		//Platform::GetInstance().GetWindowSize(nWndWidth,nWndHeigh);
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

		RefPtr<Technique> pShadingTech;

		bool bSkin = strMacro.find("SKIN") != std::string::npos;
		if (bSkin)
		{
            VertexElement element[5];
            element[0] = VertexElement(0, 0, DT_SHORT4N, DU_POSITION, 0);
            element[1] = VertexElement(0, 8, DT_UBYTE4N, DU_NORMAL, 0);
            element[2] = VertexElement(0, 12, DT_SHORT2N, DU_TEXCOORD, 0);
            element[3] = VertexElement(0, 16, DT_UBYTE4, DU_BLENDINDICES, 0);
            element[4] = VertexElement(0, 20, DT_UBYTE4N, DU_BLENDWEIGHT, 0);
            RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element, 5);
            
			pShadingTech = CreateTechnique("shader/skinmesh.tech", "mesh", "mesh", strMacro.c_str(), pDeclaration.get());

			//pShadingTech->SetVertexDeclaration(pDeclaration.get());

			pShadingTech->SaveToXML("shader/skinmesh.tech");
		}
		else
		{
            std::vector<VertexElement> vec_element;
			vec_element.push_back( VertexElement(0, 0, DT_SHORT4N, DU_POSITION, 0) );
			vec_element.push_back(VertexElement(0, 8, DT_UBYTE4N, DU_NORMAL, 0) );
			vec_element.push_back(VertexElement(0, 12, DT_SHORT2N, DU_TEXCOORD, 0) );
            RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(vec_element.data(), vec_element.size());
            
			pShadingTech = CreateTechnique("shader/mesh.tech", "mesh", "mesh", strMacro.c_str(), pDeclaration.get());
           
			// Instance Shader
			{
			
				ShaderProgram* pShader = pShadingTech->GetShaderProgram();
				VertexDeclaration* pVertexDecl = pShader->GetVertexDeclaration();

				std::vector<VertexElement> vecElement;
				for (UINT i = 0; i < pVertexDecl->GetElementCount(0); ++i)
				{
					vecElement.push_back(pVertexDecl->GetElement(0, i));
				}
				vecElement.push_back(VertexElement(1, 0, DT_FLOAT4, DU_TEXCOORD, 1));
				vecElement.push_back(VertexElement(1, 16, DT_FLOAT4, DU_TEXCOORD, 2));
				vecElement.push_back(VertexElement(1, 32, DT_FLOAT4, DU_TEXCOORD, 3));
				vecElement.push_back(VertexElement(1, 48, DT_FLOAT3, DU_TEXCOORD, 4));
				vecElement.push_back(VertexElement(1, 60, DT_FLOAT3, DU_TEXCOORD, 5));
				vecElement.push_back(VertexElement(1, 72, DT_FLOAT4, DU_TEXCOORD, 6));

				RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(vecElement.data(), vecElement.size());

				std::string strShaderMacro = pShader->GetShaderMacro();
				strShaderMacro += ";INSTANCE";
				RefPtr<Technique> pInstanceTech = CreateTechnique("shader/meshinstance.tech", pShader->GetVSFile(), pShader->GetPSFile(), strShaderMacro.c_str(), pDeclaration.get());
			
				pInstanceTech->SaveToXML("shader/meshinstance.tech");

				pShadingTech->SetInstTech(pInstanceTech.get());
			}

			pShadingTech->SaveToXML("shader/mesh.tech");
		}

		pSubMaterial->SetShadingTechnqiue(pShadingTech.get());

		pSubMaterial->SetParameter("u_texture", Any( CreateSamplerState(pszTexture) ) );
		pSubMaterial->SetParameter("u_cDiffuseColor", Any( Vector4(1,1,1,1) ) );
		pSubMaterial->SetParameter("u_cSpecColor", Any( Vector4(1,1,1,1) ) );
		pSubMaterial->SetParameter("u_roughness", Any(10.0f));
		//pSubMaterial->SetParameter("u_envroughness", Any( Log2(10.0f) / 13.0f ));

		pMaterial->SaveToFile(pMatPath);
	}

}

