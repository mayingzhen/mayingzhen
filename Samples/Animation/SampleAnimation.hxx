#include "Samples/Animation/SampleAnimation.h"
#include "Animation/Module.h"
#include "DXRender/Module.h"
#include "Serialize/Module.h"

namespace ma
{
	SampleAnimation::SampleAnimation()
	{
	}

	void SampleAnimation::Init(Application* pApplication)
	{
		SimpleSceneView::Init(pApplication);

		D3DXVECTOR3 eyePos = D3DXVECTOR3(0, 400, 400);
		D3DXVECTOR3 lookatPos = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);
		D3DXMatrixLookAtLH(&m_matView,&eyePos,&lookatPos,&vUp);

		DxRenderModuleInit();
		AnimationModuleInit();

		DxRenderDevice* pDxRenderDevice = (DxRenderDevice*)GetRenderDevice();
		pDxRenderDevice->Init( (HWND)pApplication->GetWindID() );

		DxRender* pDxRender = (DxRender*)GetRender();
		pDxRender->InitDefaultShader();

	}

	void SampleAnimation::Shutdown()
	{
		DxRenderModuleShutdown();
		AnimationModuleShutdown();
	}

	void SampleAnimation::Load()
	{		
		if (GetRenderDevice() == NULL)
			return;

		// character A Mesh & skeleton & Animation
		{
			// Mesh B f h
			m_pRenderMeshA_b = GetRenderDevice()->CreateRendMesh();
			MeshData* pMeshDataA_b = LoadMeshFromBinaryFile("../TrineGame/man001/Man001/body_b.skn");
			m_pRenderMeshA_b->InitWithData(pMeshDataA_b);

			m_pRendTextureA_b = GetRenderDevice()->CreateRendTexture();
			m_pRendTextureA_b->Load("../TrineGame/man001/Man001/body_b.tga");

			m_pRenderMeshA_h = GetRenderDevice()->CreateRendMesh();
			MeshData* pMeshDataA_h = LoadMeshFromBinaryFile("../TrineGame/man001/Man001/body_h.skn");
			m_pRenderMeshA_h->InitWithData(pMeshDataA_h);

			m_pRendTextureA_h = GetRenderDevice()->CreateRendTexture();
			m_pRendTextureA_h->Load("../TrineGame/man001/Man001/body_b.tga");	

			m_pRenderMeshA_f = GetRenderDevice()->CreateRendMesh();
			MeshData* pMeshDataA_f = LoadMeshFromBinaryFile("../TrineGame/man001/Man001/body_f.skn");
			m_pRenderMeshA_f->InitWithData(pMeshDataA_f);

			m_pRendTextureA_f = GetRenderDevice()->CreateRendTexture();
			m_pRendTextureA_f->Load("../TrineGame/man001/Man001/body_b.tga");	

			SkeletonData* pSkelDataA = LoadSkeletonFromBinaryFile("../TrineGame/man001/Man001/body.ske");
			m_pSkeletonA = new Skeleton();
			m_pSkeletonA->InitWithData(*pSkelDataA);
			m_pSkeletonA->InitUpLowerBoneSet();

			m_pAnimationA120 = new Animation();
			AnimationData* pAnimData120 = LoadAnimationFromBinaryFile("../TrineGame/Man001/120/bip01.ska");
			m_pAnimationA120->InitWithData(pAnimData120);

			m_pAnimationA100 = new Animation();
			AnimationData* pAnimData100 = LoadAnimationFromBinaryFile("../TrineGame/Man001/140/bip01.ska");
			m_pAnimationA100->InitWithData(pAnimData100);
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
			m_pSkeletonB->InitUpLowerBoneSet();

			m_pAnimationB120 = new Animation();
			AnimationData* pAnimData120 = LoadAnimationFromBinaryFile("../TrineGame/Character/magician/120/bip01.ska");
			m_pAnimationB120->InitWithData(pAnimData120);

			m_pAnimationB100 = new Animation();
			AnimationData* pAnimData100 = LoadAnimationFromBinaryFile(".../TrineGame/Character/magician/100/bip01.ska");
			m_pAnimationB100->InitWithData(pAnimData100);
		}
		
		// original Animation
		// A
		AnimationInst* pAnimInstA100SkeA = new AnimationInst(m_pAnimationA100,m_pSkeletonA);
		AnimationInst* pAnimInstA120SkeA = new AnimationInst(m_pAnimationA120,m_pSkeletonA);
		// B
		AnimationInst* pAnimInstB100SkeB = new AnimationInst(m_pAnimationB100,m_pSkeletonB);
		AnimationInst* pAnimInstB120SkeB = new AnimationInst(m_pAnimationB120,m_pSkeletonB);

		// Animation Retarget 
		// A -- > B
		AnimationInst* pAnimInstA100SkeB = new AnimationInst(m_pAnimationA100,m_pSkeletonB);
		AnimationInst* pAnimInstA120SkeB = new AnimationInst(m_pAnimationA120,m_pSkeletonB);
		// B --> A
		AnimationInst* pAnimInstB100SkeA = new AnimationInst(m_pAnimationB100,m_pSkeletonA);
		AnimationInst* pAnimInstB120SkeA = new AnimationInst(m_pAnimationB120,m_pSkeletonA);


		// A have 4 animation pAnimInstA100SkeA & pAnimInstA120SkeA & pAnimInstB100SkeA & pAnimInstB120SkeA
		{
			AnimationSet* pAnimSetA = new AnimationSet();
			pAnimSetA->AddAnimationInst(pAnimInstA100SkeA,"A100");
			pAnimSetA->AddAnimationInst(pAnimInstA120SkeA,"A120");
			pAnimSetA->AddAnimationInst(pAnimInstB100SkeA,"B100");
			pAnimSetA->AddAnimationInst(pAnimInstB120SkeA,"B120");

			m_pAnimtionPlayA = new AnimationPlay();
			m_pAnimtionPlayA->SetSkeleton(m_pSkeletonA);
			m_pAnimtionPlayA->SetAnimationSet(pAnimSetA);
			//m_pAnimtionPlayA->PlayAnimation("A100");
		}

		// B have 4 animation pAnimInstA100SkeB & pAnimInstA120SkeB & pAnimInstB100SkeB & pAnimInstB120SkeB	
		{
			AnimationSet* pAnimSetB = new AnimationSet();
			pAnimSetB->AddAnimationInst(pAnimInstA100SkeB,"A100");
			pAnimSetB->AddAnimationInst(pAnimInstA120SkeB,"A120");
			pAnimSetB->AddAnimationInst(pAnimInstB100SkeB,"B100");
			pAnimSetB->AddAnimationInst(pAnimInstB120SkeB,"B120");
			
			m_pAnimtionPlayB = new AnimationPlay();
			m_pAnimtionPlayB->SetSkeleton(m_pSkeletonB);
			m_pAnimtionPlayB->SetAnimationSet(pAnimSetB);
			m_pAnimtionPlayB->PlayAnimation("A100");
		}
	}

	void SampleAnimation::Unload()
	{

	}

	void SampleAnimation::Tick(float timeElapsed)
	{
		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		m_pAnimtionPlayA->AdvanceTime(fTimeElapsed);

		m_pAnimtionPlayB->AdvanceTime(fTimeElapsed);

		m_pAnimtionPlayA->EvaluateAnimation(1.0f);

		m_pAnimtionPlayB->EvaluateAnimation(1.0f);
	}

	void SampleAnimation::Render()
	{
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

		pRender->BeginRender();

		pRender->SetViewMatrix(&m_matView);
		pRender->SetProjMatrix(&m_matProj);
	
		// Render A
// 		{
// 			D3DXMATRIX matWorld,matRoat;
// 			D3DXMatrixRotationX(&matRoat,-3.14f / 2.0f);
// 			D3DXMatrixTranslation(&matWorld,-50,0,0);
// 			matWorld = matRoat * matWorld;
// 
// 			D3DXMATRIX* skinMatrix = m_pAnimtionPlayA->GetSkinMatrixArray();
// 			UINT nSkinMatrixNumber = m_pAnimtionPlayA->GetSkinMatrixNumber();
// 			pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshA_b,m_pRendTextureA_b);
// 			pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshA_f,m_pRendTextureA_f);
// 			pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshA_h,m_pRendTextureA_h);
// 		}

		// Render B
		{
			D3DXMATRIX matWorld,matRoat;
			D3DXMatrixRotationX(&matRoat,0/*-3.14f*/ / 2.0f);
			D3DXMatrixTranslation(&matWorld,0,0,0);
			matWorld = matRoat * matWorld;

			D3DXMATRIX* skinMatrix = m_pAnimtionPlayB->GetSkinMatrixArray();
			UINT nSkinMatrixNumber = m_pAnimtionPlayB->GetSkinMatrixNumber();
			pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshB,m_pRendTextureB);
		}

		pRender->EndRender();
	}

	void SampleAnimation::OnResize(int w,int h)
	{

	}
}


