#include "Samples/Animation/SampleAnimationRetarget.h"
#include "Animation/Module.h"
#include "D3D9Render/Module.h"
#include "Serialize/Module.h"

namespace ma
{
	SampleAnimationRetarget::SampleAnimationRetarget()
	{
	}

	void SampleAnimationRetarget::Init(Application* pApplication)
	{
		SimpleSceneView::Init(pApplication);

		m_vEyePos = D3DXVECTOR3(0, 400, 400);
		D3DXVECTOR3 lookatPos = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);
		D3DXMatrixLookAtLH(&m_matView,&m_vEyePos,&lookatPos,&vUp);

		D3D9RenderModuleInit();
		AnimationModuleInit();

		D3D9RenderDevice* pDxRenderDevice = (D3D9RenderDevice*)GetRenderDevice();
		pDxRenderDevice->Init( (HWND)pApplication->GetWindID() );

		D3D9Render* pDxRender = (D3D9Render*)GetRender();
		pDxRender->InitDefaultShader();

	}

	void SampleAnimationRetarget::Shutdown()
	{
		D3D9RenderModuleShutdown();
		AnimationModuleShutdown();
	}

	void SampleAnimationRetarget::Load()
	{		
		if (GetRenderDevice() == NULL)
			return;

		// character A Mesh & skeleton & Animation
		{
			// Mesh B (b f h)
			m_pRenderMeshA_b = GetRenderDevice()->CreateRendMesh();
			MeshData* pMeshDataA_b = LoadMeshFromBinaryFile("../TrineGame/man001/Man001/body_b.skn");
			m_pRenderMeshA_b->InitWithData(pMeshDataA_b);

			m_pRendTextureA_b = GetRenderDevice()->CreateRendTexture();
			m_pRendTextureA_b->Load("../TrineGame/man001/Man001/body_b.tga");

			m_pRenderMeshA_h = GetRenderDevice()->CreateRendMesh();
			MeshData* pMeshDataA_h = LoadMeshFromBinaryFile("../TrineGame/man001/Man001/body_h.skn");
			m_pRenderMeshA_h->InitWithData(pMeshDataA_h);

			m_pRendTextureA_h = GetRenderDevice()->CreateRendTexture();
			m_pRendTextureA_h->Load("../TrineGame/man001/Man001/body_h.tga");	

			m_pRenderMeshA_f = GetRenderDevice()->CreateRendMesh();
			MeshData* pMeshDataA_f = LoadMeshFromBinaryFile("../TrineGame/man001/Man001/body_f.skn");
			m_pRenderMeshA_f->InitWithData(pMeshDataA_f);

			m_pRendTextureA_f = GetRenderDevice()->CreateRendTexture();
			m_pRendTextureA_f->Load("../TrineGame/man001/Man001/body_f.tga");	

			SkeletonData* pSkelDataA = LoadSkeletonFromBinaryFile("../TrineGame/man001/Man001/body.ske");
			m_pSkeletonA = new Skeleton();
			m_pSkeletonA->InitWithData(*pSkelDataA);

			m_pAnimationA120 = new Animation();
			AnimationData* pAnimData120 = LoadAnimationFromBinaryFile("../TrineGame/Man001/120/bip01.ska");
			m_pAnimationA120->InitWithData(pAnimData120);
			m_pAnimationA120->ConverteAnimDataParentToLocalSpaceAnimation(m_pSkeletonA);

			m_pAnimationA100 = new Animation();
			AnimationData* pAnimData100 = LoadAnimationFromBinaryFile("../TrineGame/Man001/140/bip01.ska");
			m_pAnimationA100->InitWithData(pAnimData100);
			m_pAnimationA100->ConverteAnimDataParentToLocalSpaceAnimation(m_pSkeletonA);
		}

		// character B Mesh & skeleton & Animation
		{
			m_pRenderMeshB = GetRenderDevice()->CreateRendMesh();
			MeshData* pMeshDataB = LoadMeshFromBinaryFile("../TrineGame/Character/magician/Body.skn");
			m_pRenderMeshB->InitWithData(pMeshDataB);

			m_pRendTextureB = GetRenderDevice()->CreateRendTexture();
			m_pRendTextureB->Load("../TrineGame/Character/magician/Body.tga");	

			SkeletonData* pSkelDataB = LoadSkeletonFromBinaryFile("../TrineGame/Character/magician/Body.ske");
			m_pSkeletonB = new Skeleton();
			m_pSkeletonB->InitWithData(*pSkelDataB);

			m_pAnimationB602 = new Animation();
			AnimationData* pAnimData100 = LoadAnimationFromBinaryFile("../TrineGame/Character/magician/602/bip01.ska");
			m_pAnimationB602->InitWithData(pAnimData100);
			m_pAnimationB602->ConverteAnimDataParentToLocalSpaceAnimation(m_pSkeletonB);

			m_pAnimationB120 = new Animation();
			AnimationData* pAnimData120 = LoadAnimationFromBinaryFile("../TrineGame/Character/magician/100/bip01.ska");
			m_pAnimationB120->InitWithData(pAnimData120);
			m_pAnimationB120->ConverteAnimDataParentToLocalSpaceAnimation(m_pSkeletonB);
		}
		
		// original Animation
		// A
		AnimationInst* pAnimInstA100SkeA = new AnimationInst(m_pAnimationA100,m_pSkeletonA);
		AnimationInst* pAnimInstA120SkeA = new AnimationInst(m_pAnimationA120,m_pSkeletonA);
		// B
		AnimationInst* pAnimInstB602SkeB = new AnimationInst(m_pAnimationB602,m_pSkeletonB);
		AnimationInst* pAnimInstB120SkeB = new AnimationInst(m_pAnimationB120,m_pSkeletonB);

		// Animation Retarget 
		// A -- > B
		AnimationInst* pAnimInstA100SkeB = new AnimationInst(m_pAnimationA100,m_pSkeletonB);
		AnimationInst* pAnimInstA120SkeB = new AnimationInst(m_pAnimationA120,m_pSkeletonB);
		// B --> A
		AnimationInst* pAnimInstB602SkeA = new AnimationInst(m_pAnimationB602,m_pSkeletonA);
		AnimationInst* pAnimInstB120SkeA = new AnimationInst(m_pAnimationB120,m_pSkeletonA);


		// A have 4 animation pAnimInstA100SkeA & pAnimInstA120SkeA & pAnimInstB100SkeA & pAnimInstB120SkeA
		{
			AnimationSet* pAnimSetA = new AnimationSet();
			pAnimSetA->AddAnimationInst(pAnimInstA100SkeA,"A100");
			pAnimSetA->AddAnimationInst(pAnimInstA120SkeA,"A120");
			pAnimSetA->AddAnimationInst(pAnimInstB602SkeA,"B602");
			pAnimSetA->AddAnimationInst(pAnimInstB120SkeA,"B120");

			m_pAnimtionPlayA = new AnimationPlay();
			m_pAnimtionPlayA->SetSkeleton(m_pSkeletonA);
			m_pAnimtionPlayA->SetAnimationSet(pAnimSetA);
			m_pAnimtionPlayA->PlayAnimation("A100");
		}

		// B have 4 animation pAnimInstA100SkeB & pAnimInstA120SkeB & pAnimInstB100SkeB & pAnimInstB120SkeB	
		{
			AnimationSet* pAnimSetB = new AnimationSet();
			pAnimSetB->AddAnimationInst(pAnimInstA100SkeB,"A100");
			pAnimSetB->AddAnimationInst(pAnimInstA120SkeB,"A120");
			pAnimSetB->AddAnimationInst(pAnimInstB602SkeB,"B602");
			pAnimSetB->AddAnimationInst(pAnimInstB120SkeB,"B120");
			
			m_pAnimtionPlayB = new AnimationPlay();
			m_pAnimtionPlayB->SetSkeleton(m_pSkeletonB);
			m_pAnimtionPlayB->SetAnimationSet(pAnimSetB);
			m_pAnimtionPlayB->PlayAnimation("A100");
		}
	}

	void SampleAnimationRetarget::Unload()
	{

	}

	void SampleAnimationRetarget::OnInput()
	{
		Input* pInput = GetInput();
		if (pInput == NULL)
			return;

		if (pInput->IsKeyPressed(OIS::KC_1))
		{
			m_pAnimtionPlayA->PlayAnimation("A100");
			m_pAnimtionPlayB->PlayAnimation("A100");
		}
		else if (pInput->IsKeyPressed(OIS::KC_2))
		{
			m_pAnimtionPlayA->PlayAnimation("A120");
			m_pAnimtionPlayB->PlayAnimation("A120");
		}
		else if (pInput->IsKeyPressed(OIS::KC_3))
		{
			m_pAnimtionPlayA->PlayAnimation("B602");
			m_pAnimtionPlayB->PlayAnimation("B602");
		}
		else if (pInput->IsKeyPressed(OIS::KC_4))
		{
			m_pAnimtionPlayA->PlayAnimation("B120");
			m_pAnimtionPlayB->PlayAnimation("B120");
		}
	}

	void SampleAnimationRetarget::Tick(float timeElapsed)
	{
		__super::Tick(timeElapsed);

		OnInput();

		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		m_pAnimtionPlayA->AdvanceTime(fTimeElapsed);

		m_pAnimtionPlayB->AdvanceTime(fTimeElapsed);

		m_pAnimtionPlayA->EvaluateAnimation(1.0f);

		m_pAnimtionPlayB->EvaluateAnimation(1.0f);
	}

	void SampleAnimationRetarget::Render()
	{
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

		pRender->BeginRender();

		pRender->SetViewMatrix(&m_matView);
		pRender->SetProjMatrix(&m_matProj);
	
		// Render A
		{
			D3DXMATRIX matWorld,matRoat;
			D3DXMatrixRotationX(&matRoat,-3.14f / 2.0f);
			D3DXMatrixTranslation(&matWorld,-50,0,0);
			matWorld = matRoat * matWorld;

			D3DXMATRIX* skinMatrix = m_pAnimtionPlayA->GetSkinMatrixArray();
			UINT nSkinMatrixNumber = m_pAnimtionPlayA->GetSkinMatrixNumber();
			pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshA_b,m_pRendTextureA_b);
			pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshA_f,m_pRendTextureA_f);
			pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshA_h,m_pRendTextureA_h);
		}

		// Render B
		{
			D3DXMATRIX matWorld,matRoat;
			D3DXMatrixRotationX(&matRoat,-3.14f / 2.0f);
			D3DXMatrixTranslation(&matWorld,50,0,0);
			matWorld = matRoat * matWorld;

			D3DXMATRIX* skinMatrix = m_pAnimtionPlayB->GetSkinMatrixArray();
			UINT nSkinMatrixNumber = m_pAnimtionPlayB->GetSkinMatrixNumber();
			pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshB,m_pRendTextureB);
		}

		pRender->EndRender();
	}

	void SampleAnimationRetarget::OnResize(int w,int h)
	{

	}
}


