#include "SampleLighting.h"

namespace ma
{
	SampleLighting::SampleLighting()
	{
	}

	void SampleLighting::Load()
	{
		Vector3 vEyePos = Vector3(0, 400, 600);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->LookAt(vEyePos,VAtPos,vUp);

		// Render Mesh
		{
			GameObject* pCharMagic = GetEntitySystem()->CreateGameObject("magic");

			RenderMesh* pMeshComp = pCharMagic->CreateComponent<RenderMesh>();
			pMeshComp->Load("magician/Body.skn","magician/Body.tga");

			IAnimationObject* pAnimObj = pCharMagic->CreateComponent<IAnimationObject>();
			pAnimObj->Load(NULL,"magician/Body.ske");
			IAnimationSet* pAnimSet = pAnimObj->GetAnimationSet();
			pAnimSet->AddAnimClip("magician/100/bip01.ska","mag100");
			pAnimObj->PlayAnimation((UINT)0);

			pCharMagic->GetSceneNode()->Right(100);

			GameObject* pPlatform = CreateMeshGameObject("Fbx/MovingPlatform.skn","Fbx/PlatformTexture.tga");

			GameObject* pBox = CreateMeshGameObject("Fbx/Box.skn","Fbx/Box.tga");

			pPlatform->GetSceneNode()->Scale(50);

			pBox->GetSceneNode()->Scale(50);
		}

		// Light
		{
 	
			{
// 				PointLight* pLight = new PointLight(NULL);	
// 				pLight->SetLightColor(Vector4(1.0f,1.0f,1.0f,1.0f));
// 				pLight->SetPos(Vector3(200, 100, 0));
// 				pLight->SetRadius(300);
// 				GetRenderSystem()->AddLight(pLight);
// 				m_arrPointLight.push_back(pLight);
			}

			{
// 				PointLight* pLight = new PointLight(NULL);	
// 				pLight->SetLightColor(Vector4(1.0f,1.0f,1.0f,1.0f));
// 				pLight->SetPos(Vector3(-200, 200, -200));
// 				pLight->SetRadius(300);
// 				GetRenderSystem()->AddLight(pLight);
// 				m_arrPointLight.push_back(pLight);
			}
// 
//  			
// 			DirectonalLight* pDirLight = new DirectonalLight();
// 			pDirLight->SetLightColor(Vector4(0.2f,0.2f,0.2f,1.0f));
// 			pDirLight->SetDirection(Vector3(10,10,0));
// 			GetRenderSystem()->AddLight(pDirLight);

		}

		GetRenderSystem()->SetAmbientColor(Vector4(0.0f,0.0f,0.0f,0.0f));

		GetRenderSystem()->SetClearClor(Color(0,0,0,0));

	}

	void SampleLighting::UnLoad()
	{
		GetRenderSystem()->SetAmbientColor(Vector4(1.0f,1.0f,1.0f,1.0f));	
		GetRenderSystem()->SetClearClor(Color(0,45.0f / 255.0f,50.0f/255.0f,170.0f/255.0f));
	}

	void SampleLighting::Update()
	{
		for (UINT i = 0; i < m_arrPointLight.size(); ++i)
		{
			PointLight* pPointLight = m_arrPointLight[i];

			float fAngle = ToRadian( 30 * GetTimer()->GetFrameDeltaTime() );

			Vector3 vUp(0,1,0);
			Matrix4x4 matRoat;
			MatrixRotationAxis(&matRoat,&vUp,fAngle);

			Vector3 vPos = pPointLight->GetPos();
			Vec3TransformCoord(&vPos,&vPos,&matRoat);

			//pPointLight->SetPos(vPos);
		}
	}

	void SampleLighting::Render()
	{

		for (UINT i = 0; i < m_arrPointLight.size(); ++i)
		{
			PointLight* pPointLight = m_arrPointLight[i];
			Matrix4x4 matWS;
			MatrixTranslation(&matWS,pPointLight->GetPos().x,pPointLight->GetPos().y,pPointLight->GetPos().z);
			GetLineRender()->DrawWireSphere(matWS,10,Color(1,0,0,0));
		}

		// Deubug Rraw Light Radius
// 		for (UINT i = 0; i < m_arrPointLight.size(); ++i)
// 		{
// 			PointLight* pPointLight = m_arrPointLight[i];
// 			Matrix4x4 matWS;
// 			MatrixTranslation(&matWS,pPointLight->GetPos().x,pPointLight->GetPos().y,pPointLight->GetPos().z);
// 			LineRender::DrawWireSphere(matWS,pPointLight->GetRadius(),Color(1,0,0,0));
// 		}
		
	}
}


