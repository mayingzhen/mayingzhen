#include "SampleAnimationRetarget.h"
#if PLATFORM_WIN == 1
#include "S3Serialize/Module.h"
#endif

namespace ma
{
	SampleAnimationRetarget::SampleAnimationRetarget()
	{
	}


	void SampleAnimationRetarget::Load()
	{	
		// DataConver
#if PLATFORM_WIN == 1
		{
			// gigi
			{
				MeshData* pMeshDatab = LoadMeshFromS3BinaryFile("../../Data/S3gigi/gigi/body_b.skn");
				pMeshDatab->SaveToFile("../../Data/gigi/gigi/body_b.skn");

				MeshData* pMeshDatah = LoadMeshFromS3BinaryFile("../../Data/S3gigi/gigi/body_h.skn");
				pMeshDatah->SaveToFile("../../Data/gigi/gigi/body_h.skn");

				MeshData* pMeshDataf = LoadMeshFromS3BinaryFile("../../Data/S3gigi/gigi/body_f.skn");
				pMeshDataf->SaveToFile("../../Data/gigi/gigi/body_f.skn");

				SkeletonData* pSkelData = LoadSkeletonFromS3BinaryFile("../../Data/S3gigi/gigi/body.ske");
				pSkelData->SaveToFile("../../Data/gigi/gigi/body.ske");

				AnimationData* pRun = LoadAnimationFromS3BinaryFile("../../Data/S3gigi/210_run/bip01.ska");
				ConverteAnimDataParentToLocalSpaceAnimation(pRun,pSkelData);
				pRun->SaveToFile("../../Data/gigi/210_run/bip01.ska");

				AnimationData* pJunmp = LoadAnimationFromS3BinaryFile("../../Data/S3gigi/281_run_jump_start/bip01.ska");
				ConverteAnimDataParentToLocalSpaceAnimation(pJunmp,pSkelData);
				pJunmp->SaveToFile("../../Data/gigi/281_run_jump_start/bip01.ska");
			}

			// magic
			{
				MeshData* pMeshDataMag = LoadMeshFromS3BinaryFile("../../Data/S3magician/Body.skn");
				pMeshDataMag->SaveToFile("../../Data/magician/Body.skn");
			
				SkeletonData* pSkelDataMag = LoadSkeletonFromS3BinaryFile("../../Data/S3magician/Body.ske");
				pSkelDataMag->SaveToFile("../../Data/magician/Body.ske");

				AnimationData* Mag602 = LoadAnimationFromS3BinaryFile("../../Data/S3magician/602/bip01.ska");
				ConverteAnimDataParentToLocalSpaceAnimation(Mag602,pSkelDataMag);
				Mag602->SaveToFile("../../Data/magician/602/bip01.ska");

				AnimationData* mag100 = LoadAnimationFromS3BinaryFile("../../Data/S3magician/100/bip01.ska");
				ConverteAnimDataParentToLocalSpaceAnimation(mag100,pSkelDataMag);
				mag100->SaveToFile("../../Data/magician/100/bip01.ska");
			}
		}
#endif

		// character A MeshData & skeleton & Animation
		{
			// MeshData B (b f h)
			m_pRenderMeshA_b = new RenderMesh();
			m_pRenderMeshA_b->Load("gigi/gigi/body_b.skn","gigi/gigi/body_b.tga");

			m_pRenderMeshA_h = new RenderMesh(); 
			m_pRenderMeshA_h->Load("gigi/gigi/body_h.skn","gigi/gigi/body_h.tga");

			m_pRenderMeshA_f = new RenderMesh();
			m_pRenderMeshA_f->Load("gigi/gigi/body_f.skn","gigi/gigi/body_f.tga");

			m_pAnimtionPlayA = GetAnimationDevice()->CreateAnimationPlay();
			m_pAnimtionPlayA->CreateSkeleton("gigi/gigi/body.ske");
			IAnimationSet* pAnimSet = m_pAnimtionPlayA->CreateAnimSet(NULL);

			pAnimSet->AddAction("gigi/210_run/bip01.ska","gigi_Run");
			pAnimSet->AddAction("gigi/281_run_jump_start/bip01.ska","gigi_jump");
			pAnimSet->AddAction("magician/602/bip01.ska","Mag602");
			pAnimSet->AddAction("magician/100/bip01.ska","mag100");

			m_pAnimtionPlayA->PlayAnimation((UINT)0);	
		}

		// character B MeshData & skeleton & Animation
		{
			m_pRenderMeshB = new RenderMesh();
			m_pRenderMeshB->Load("magician/Body.skn","magician/Body.tga");

			m_pAnimtionPlayB =  GetAnimationDevice()->CreateAnimationPlay();
			m_pAnimtionPlayB->CreateSkeleton("magician/Body.ske");
			IAnimationSet* pAnimSet = m_pAnimtionPlayB->CreateAnimSet(NULL);
			
			pAnimSet->AddAction("gigi/210_run/bip01.ska","gigi_Run");
			pAnimSet->AddAction("gigi/281_run_jump_start/bip01.ska","gigi_jump");
			pAnimSet->AddAction("magician/602/bip01.ska","Mag602");
			pAnimSet->AddAction("magician/100/bip01.ska","mag100");

			m_pAnimtionPlayB->PlayAnimation((UINT)0);
		}
	}

	void SampleAnimationRetarget::UnLoad()
	{
		SAFE_DELETE(m_pRenderMeshA_b);
		SAFE_DELETE(m_pRenderMeshA_f);
		SAFE_DELETE(m_pRenderMeshA_h);

		SAFE_DELETE(m_pRenderMeshB);
	}

	void SampleAnimationRetarget::OnInput()
	{
		Input* pInput = GetInput();
		if (pInput == NULL)
			return;

		if (pInput->IsKeyDown(OIS::KC_1))
		{
			m_pAnimtionPlayA->PlayAnimation((UINT)0);
			m_pAnimtionPlayB->PlayAnimation((UINT)0);
		}
		else if (pInput->IsKeyDown(OIS::KC_2))
		{
			m_pAnimtionPlayA->PlayAnimation(1);
			m_pAnimtionPlayB->PlayAnimation(1);
		}
		else if (pInput->IsKeyDown(OIS::KC_3))
		{
			m_pAnimtionPlayA->PlayAnimation(2);
			m_pAnimtionPlayB->PlayAnimation(2);
		}
		else if (pInput->IsKeyDown(OIS::KC_4))
		{
			m_pAnimtionPlayA->PlayAnimation(3);
			m_pAnimtionPlayB->PlayAnimation(3);
		}
	}

	void SampleAnimationRetarget::Update()
	{
		OnInput();

		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		if (m_pAnimtionPlayA)
		{
			m_pAnimtionPlayA->AdvanceTime(fTimeElapsed);
			m_pAnimtionPlayA->EvaluateAnimation(1.0f);

			Matrix4x4* skinMatrixA = m_pAnimtionPlayA->GetSkinMatrixArray();
			UINT nNumberA = m_pAnimtionPlayA->GetSkinMatrixNumber();

			m_pRenderMeshA_b->SetSkinMatrix(skinMatrixA,nNumberA);
			m_pRenderMeshA_f->SetSkinMatrix(skinMatrixA,nNumberA);
			m_pRenderMeshA_h->SetSkinMatrix(skinMatrixA,nNumberA);

			Matrix4x4 matWorld,matRoat;
			MatrixTranslation(&matWorld,-50,120,0);
			MatrixRotationYawPitchRoll(&matRoat,0,PI * 1.2,0);
			matWorld = matWorld * matRoat;
			m_pRenderMeshA_b->SetWorldMatrix(matWorld);
			m_pRenderMeshA_f->SetWorldMatrix(matWorld);
			m_pRenderMeshA_h->SetWorldMatrix(matWorld);

		}

		if (m_pAnimtionPlayB)
		{
			m_pAnimtionPlayB->AdvanceTime(fTimeElapsed);
			m_pAnimtionPlayB->EvaluateAnimation(1.0f);

			Matrix4x4* skinMatrixB = m_pAnimtionPlayB->GetSkinMatrixArray();
			UINT nNumberB = m_pAnimtionPlayB->GetSkinMatrixNumber();

			m_pRenderMeshB->SetSkinMatrix(skinMatrixB,nNumberB);

			Matrix4x4 matWorld,matRoat;
			MatrixTranslation(&matWorld,50,120,0);
			MatrixRotationYawPitchRoll(&matRoat,0,PI * 1.2,0);
			matWorld = matWorld * matRoat;
			m_pRenderMeshB->SetWorldMatrix(matWorld);
		}
	}

	void SampleAnimationRetarget::Render()
	{
		if (GetRenderDevice() == NULL)
			return;

		// A
 		m_pRenderMeshA_b->Draw();
 		m_pRenderMeshA_f->Draw();
 		m_pRenderMeshA_h->Draw();

		// B
		m_pRenderMeshB->Draw();
	}

}


