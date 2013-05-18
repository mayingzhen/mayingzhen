#include "Samples/Animation/SampleAnimationRetarget.h"
#include "Animation/Module.h"


namespace ma
{
	SampleAnimationRetarget::SampleAnimationRetarget()
	{
	}

	void SampleAnimationRetarget::Init(ApplicationBase* pApplication)
	{
		Sample::Init(pApplication);

		//Vector3 vEyePos = Vector3(0, 400, 400);
		//m_pCamera->LookAt(&vEyePos);

		AnimationModuleInit();
	}

	void SampleAnimationRetarget::Shutdown()
	{
		AnimationModuleShutdown();
	}

	void SampleAnimationRetarget::Load()
	{		
		if (GetRenderDevice() == NULL)
			return;

		// character A MeshData & skeleton & Animation
		{
			// MeshData B (b f h)
			m_pRenderMeshA_b = GetRenderDevice()->CreateRendMesh();
			MeshData* pMeshDataA_b = LoadMeshFromBinaryFile("../Data/Man001/body_b.skn");
			m_pRenderMeshA_b->InitWithData(pMeshDataA_b);

			m_pRendTextureA_b = GetRenderDevice()->CreateRendTexture();
			m_pRendTextureA_b->Load("../Data/Man001/body_b.tga");

			m_pRenderMeshA_h = GetRenderDevice()->CreateRendMesh();
			MeshData* pMeshDataA_h = LoadMeshFromBinaryFile("../Data/Man001/body_h.skn");
			m_pRenderMeshA_h->InitWithData(pMeshDataA_h);

			m_pRendTextureA_h = GetRenderDevice()->CreateRendTexture();
			m_pRendTextureA_h->Load("../Data/Man001/body_h.tga");	

			m_pRenderMeshA_f = GetRenderDevice()->CreateRendMesh();
			MeshData* pMeshDataA_f = LoadMeshFromBinaryFile("../Data/Man001/body_f.skn");
			m_pRenderMeshA_f->InitWithData(pMeshDataA_f);

			m_pRendTextureA_f = GetRenderDevice()->CreateRendTexture();
			m_pRendTextureA_f->Load("../Data/Man001/body_f.tga");	

			SkeletonData* pSkelDataA = LoadSkeletonFromBinaryFile("../Data/Man001/body.ske");
			m_pSkeletonA = new Skeleton();
			m_pSkeletonA->InitWithData(pSkelDataA);
		}

		// character B MeshData & skeleton & Animation
		{
			m_pRenderMeshB = GetRenderDevice()->CreateRendMesh();
			MeshData* pMeshDataB = LoadMeshFromBinaryFile("../Data/magician/Body.skn");
			m_pRenderMeshB->InitWithData(pMeshDataB);

			m_pRendTextureB = GetRenderDevice()->CreateRendTexture();
			m_pRendTextureB->Load("../Data/magician/Body.tga");	

			SkeletonData* pSkelDataB = LoadSkeletonFromBinaryFile("../Data/magician/Body.ske");
			m_pSkeletonB = new Skeleton();
			m_pSkeletonB->InitWithData(pSkelDataB);
		}
	

		{
			m_pAnimtionPlayA = new AnimationPlay(m_pSkeletonA);
			m_pAnimtionPlayA->AddAction("../Data/man001/120/bip01.ska","man120");
			m_pAnimtionPlayA->AddAction("../Data/man001/140/bip01.ska","Man140");
			m_pAnimtionPlayA->AddAction("../Data/magician/602/bip01.ska","Mag602");
			m_pAnimtionPlayA->AddAction("../Data/magician/100/bip01.ska","mag100");

			m_pAnimtionPlayA->PlayAnimation((ActionID)0);
		} 

		{
			m_pAnimtionPlayB = new AnimationPlay(m_pSkeletonB);

			m_pAnimtionPlayA->AddAction("../Data/man001/120/bip01.ska","man120");
			m_pAnimtionPlayA->AddAction("../Data/man001/140/bip01.ska","Man140");
			m_pAnimtionPlayA->AddAction("../Data/magician/602/bip01.ska","Mag602");
			m_pAnimtionPlayA->AddAction("../Data/magician/100/bip01.ska","mag100");

			m_pAnimtionPlayA->PlayAnimation((ActionID)0);
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
			m_pAnimtionPlayA->PlayAnimation((ActionID)0);
			m_pAnimtionPlayB->PlayAnimation((ActionID)0);
		}
		else if (pInput->IsKeyPressed(OIS::KC_2))
		{
			m_pAnimtionPlayA->PlayAnimation(1);
			m_pAnimtionPlayB->PlayAnimation(2);
		}
		else if (pInput->IsKeyPressed(OIS::KC_3))
		{
			m_pAnimtionPlayA->PlayAnimation(3);
			m_pAnimtionPlayB->PlayAnimation(3);
		}
		else if (pInput->IsKeyPressed(OIS::KC_4))
		{
			m_pAnimtionPlayA->PlayAnimation(4);
			m_pAnimtionPlayB->PlayAnimation(4);
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
		if (GetRenderDevice() == NULL)
			return;

		GetRenderDevice()->BeginRender();
	
		// Render A
		{
			Matrix4x4 matWorld,matRoat;
			MatrixRotationX(&matRoat,-3.14f / 2.0f);
			MatrixTranslation(&matWorld,-50,0,0);
			matWorld = matRoat * matWorld;

			Matrix4x4* skinMatrix = m_pAnimtionPlayA->GetSkinMatrixArray();
			UINT nSkinMatrixNumber = m_pAnimtionPlayA->GetSkinMatrixNumber();
			//pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshA_b,m_pRendTextureA_b);
			//pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshA_f,m_pRendTextureA_f);
			//pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshA_h,m_pRendTextureA_h);
		}

		// Render B
		{
			Matrix4x4 matWorld,matRoat;
			MatrixRotationX(&matRoat,-3.14f / 2.0f);
			MatrixTranslation(&matWorld,50,0,0);
			matWorld = matRoat * matWorld;

			Matrix4x4* skinMatrix = m_pAnimtionPlayB->GetSkinMatrixArray();
			UINT nSkinMatrixNumber = m_pAnimtionPlayB->GetSkinMatrixNumber();
			//pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMeshB,m_pRendTextureB);
		}

		GetRenderDevice()->EndRender();
	}

}


