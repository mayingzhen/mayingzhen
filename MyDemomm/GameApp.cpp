#include "GameApp.h"
// #include "Camera.h"
// #include "Terrain.h"
// #include "Sky.h"
// #include "mouse.h"
// #include "Text.h"
// #include "OctreeSceneManager.h"
// #include "ResLoader.h"
// #include "CObject.h"
// #include "D3DRender.h"
// #include "tinyxml.h"

#include "Common/Module.h"
#include "Framework/Module.h"
#include "DXRender/Module.h"
#include "Animation/Module.h"

CGameApp g_GameApp;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON)
		{
			//g_Camera.DoMouseControl();
		}
		else
		{
			//g_Camera.m_ControlState.bRotating = false;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
		{
			//g_SceneMng.ReleaseAll();
			DestroyWindow(hWnd);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

CGameApp::CGameApp()
{
	m_nWndWidth = 800;
	m_nWndHeigh = 600;
	m_hMainWnd = NULL;
	m_pD3D = NULL;				   // The main D3D object
	m_pd3dDevice = NULL;        // The D3D rendering device
	m_pFont = NULL;

	m_pScene = NULL;
}

CGameApp::~CGameApp(void)
{	
// 	SAFE_RELEASE(m_pFont);
// 	SAFE_RELEASE(m_pd3dDevice);
// 	SAFE_RELEASE(m_pD3D);
// 
// 	if ( IsWindow(m_hMainWnd) )
// 	{
// 		DestroyWindow(m_hMainWnd);
// 	}
// 
// 	UnregisterClass( TEXT("MyDemo"), NULL );
}

// BOOL LoadMapStaticItem(const char *szFileName)
// {
// 	TiXmlDocument fileXml;
// 	BOOL bRet = fileXml.LoadFile(szFileName);
// 	assert(bRet);
// 	if( bRet == FALSE )
// 		return FALSE;
// 
// 	TiXmlElement* pNode = fileXml.FirstChildElement("Item"); 
// 	while( pNode != NULL )
// 	{
// 		const char* itemName = pNode->Attribute("ItemName");
// 
// 		TiXmlElement* pTempNode = NULL;
// 
// 		pTempNode = pNode->FirstChildElement("Position");
// 		float pos_x = atof( pTempNode->Attribute("x") );
// 		float pos_z = atof( pTempNode->Attribute("z") );
// 		float fRotate = atof( pTempNode->Attribute("Rotate") );
// 		float fAngle = atof( pTempNode->Attribute("Angle") );
// 		
// 		pTempNode = pNode->FirstChildElement("ResName");
// 		const char* resName = pTempNode->Attribute("Name");
// 
// 		float pos_y = CTerrain::GetSingleton().GetHeight(pos_x,pos_z);
// 		D3DXVECTOR3 Pos(pos_x, pos_y, pos_z);
// 		g_SceneMng.AddMapObject(Pos, fRotate, fAngle, resName, itemName);
// 
// 		pNode = fileXml.NextSiblingElement();
// 	}
// 
// 	return TRUE;
// }
// 
// 
// BOOL LoadMapDynamicItem(const char *szFileName)
// {
// 	TiXmlDocument fileXml;
// 	BOOL bRet = fileXml.LoadFile(szFileName);
// 	assert(bRet);
// 	if( bRet == FALSE )
// 		return FALSE;
// 
// 	TiXmlElement* pNode = fileXml.FirstChildElement("Item"); 
// 	while( pNode != NULL )
// 	{
// 		const char* itemName = pNode->Attribute("ItemName");
// 
// 		TiXmlElement* pTempNode = NULL;
// 
// 		pTempNode = pNode->FirstChildElement("Position");
// 		float pos_x = atof( pTempNode->Attribute("x") );
// 		float pos_z = atof( pTempNode->Attribute("z") );
// 		float fRotate = atof( pTempNode->Attribute("Rotate") );
// 		float fAngle = atof( pTempNode->Attribute("Angle") );
// 
// 		pTempNode = pNode->FirstChildElement("ResName");
// 		const char* resName = pTempNode->Attribute("Name");
// 		float pos_y = CTerrain::GetSingleton().GetHeight(pos_x,pos_z);
// 		D3DXVECTOR3 Pos(pos_x,pos_y,pos_z);
// 		g_SceneMng.AddCharactor(Pos,fRotate,fAngle,resName,itemName);
// 
// 		pNode = fileXml.NextSiblingElement();
// 	}
// 
// 	return TRUE;
// }

void CGameApp::InitGame()
{
	//////////////////////////////////////////////////////////////////////////
	m_pCamera = new ma::Camera();
	D3DXVECTOR3 eyePos = D3DXVECTOR3(0, -200, 200);
	D3DXVECTOR3 lookatPos = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);
	m_pCamera->LookAt(&eyePos, &lookatPos, &vUp);
	
	float fFOV = D3DX_PI / 4;
	float fAspect = m_nWndHeigh / (float)m_nWndHeigh;
	float fNearClip = 1.0f;
	float fFarClip = 3000.0f;
	m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);

	ma::SceneNode* pRootNode = new ma::SceneNode(NULL,"RootNode");
	m_pScene = new ma::Scene(pRootNode);

	ma::GameObject* pGameObj = new ma::GameObject(m_pScene,"char");
	pRootNode->AddChildNode(pGameObj);
	ma::MeshComponent* pMeshBodyB = new ma::MeshComponent();
	pMeshBodyB->Load("../TrineGame/man001/Man001/body_b.skn","../TrineGame/man001/Man001/body_b.tga");
	ma::MeshComponent* pMeshBodyF = new ma::MeshComponent();
	pMeshBodyF->Load("../TrineGame/man001/Man001/body_f.skn","../TrineGame/man001/Man001/body_f.tga");
	ma::MeshComponent* pMeshBodyH = new ma::MeshComponent();
	pMeshBodyH->Load("../TrineGame/man001/Man001/body_h.skn","../TrineGame/man001/Man001/body_h.tga");

 	ma::SkelMeshComponent* pSkelMeshComp = new ma::SkelMeshComponent();
 	pGameObj->AddComponent(pSkelMeshComp);
 	pSkelMeshComp->AddMeshComp(pMeshBodyB);
 	pSkelMeshComp->AddMeshComp(pMeshBodyF);
 	pSkelMeshComp->AddMeshComp(pMeshBodyH);

	////// mag Skeletion
	ma::Skeleton* pMagSkeleton = new ma::Skeleton;
	pMagSkeleton->Load("../TrineGame/Character/magician/Body.ske");
	pMagSkeleton->InitUpLowerBoneSet();

	////// man Skeleton
	ma::Skeleton* pManSkeleton = new ma::Skeleton;
	pManSkeleton->Load("../TrineGame/man001/Man001/body.ske");
	pManSkeleton->InitUpLowerBoneSet();

	ma::AnimationSet* pAnimSet = new ma::AnimationSet();
	ma::AnimationInst* pAniMag100 = pAnimSet->LoadAnimation("../TrineGame/Character/magician/100/bip01.ska","mag100",pManSkeleton,pMagSkeleton);
	ma::AnimationInst* pAniMag120 = pAnimSet->LoadAnimation("../TrineGame/Character/magician/120/bip01.ska","mag120",pManSkeleton,pMagSkeleton);
	ma::AnimationInst* pAniMan120 = pAnimSet->LoadAnimation("../TrineGame/Man001/120/bip01.ska","man120",pManSkeleton,pManSkeleton);
	ma::AnimationInst* pAniMan140 = pAnimSet->LoadAnimation("../TrineGame/Man001/140/bip01.ska","man140",pManSkeleton,pManSkeleton);

	ma::AnimClipNode* pLowerBodyNode = new ma::AnimClipNode(pAniMan120,pManSkeleton->GetBoneSetByName("UpBody"));
	ma::AnimClipNode* pUpBodyNode = new ma::AnimClipNode(pAniMan140,pManSkeleton->GetBoneSetByName("LowerBody"));
	ma::AnimLayerNode* pAnimLayerNode = new ma::AnimLayerNode();
	pAnimLayerNode->AddLayer(pUpBodyNode);
	pAnimLayerNode->AddLayer(pLowerBodyNode);
	ma::AnimationAction* pSkelAnim = new ma::AnimationAction;
	pSkelAnim->SetAnimName("AnimationTree");
	pSkelAnim->SetTreeNode(pLowerBodyNode/*pAnimLayerNode*/);

	ma::LookAtModifier* pLookAtIk = new ma::LookAtModifier();
	D3DXVECTOR3 vForwardLS(0.0f,1.0f,0.0f); //Forward Direction in Bip01 Head's space
	D3DXVECTOR3 vUpLS(1.0f,0.0f,0.0f);		//Up Direction in Bip01 Head's space
	D3DXVECTOR3 vGoalOS = D3DXVECTOR3(100, -200, 200);;//(0.0f,0.0f,0.0f);
	pLookAtIk->Init( pManSkeleton->GetBoneIdByName("Bip01 Head"),vForwardLS,vUpLS);
	pLookAtIk->SetGoalObjectSpace(vGoalOS);
	pSkelAnim->AddPoseModifier(pLookAtIk);

	pAnimSet->AddAnimationAction(pSkelAnim);

	pSkelMeshComp->SetAnimationSet(pAnimSet);
	pSkelMeshComp->SetSkeleton(pManSkeleton);

	pSkelMeshComp->PlayAnimation("AnimationTree");


	/// physics
	BoxCollisionComponent* pBoxCollisionShape = new BoxCollisionComponent();
	pBoxCollisionShape->SetSize(D3DXVECTOR3(100,100,100));
	pGameObj->AddComponent(pBoxCollisionShape);

	//RigidBodyComponent* pRigidBodyComp = new RigidBodyComponent();
	//pGameObj->AddComponent(pRigidBodyComp);	
	//pRigidBodyComp->AddCollisionShape(pBoxCollisionShape);

	


	//////

//  	CObject::StaticInit();
// 
// 	//启动资源加载
// 	if( ResLoader()->Init() == false )
// 	{
// 		assert(false);
// 		return;
// 	}
// 	
// 	// sceneMng
// 	CAABB sceneMax;
// 	sceneMax.vMin = D3DXVECTOR3(-12000, -2000, -12000);
// 	sceneMax.vMax = D3DXVECTOR3(12000, 5000, 12000);
// 	g_SceneMng.Init( sceneMax, 8 );
// 
// 	// render
// 	g_D3DRender.Init();
// 
// 	// create scene
// 	CSky::GetSingleton().Create();
// 	CTerrain::GetSingleton().Create("res\\terrain\\shaolin.Terrain");
// 	g_SceneMng.CreateScene("res\\terrain\\shaolin.Scene");
// 	LoadMapStaticItem("res\\mapStatic.xml");
// 	LoadMapDynamicItem("res\\mapDynamic.xml");
// 	
// 	// light
// 	CMapObj* pHouse = (CMapObj*)CObject::StaticFindObject("house2");
// 	D3DXVECTOR3 LightAtPos = D3DXVECTOR3(0,1,0)/*pHouse->GetWordPos()*/;
// 	D3DXVECTOR3 LightPos;
// 	LightPos.x = LightAtPos.x - 10000;
// 	LightPos.y = LightAtPos.y + 10000;
// 	LightPos.z = LightAtPos.z - 10000;
// 	g_SceneMng.AddLight(LightPos,LightAtPos);
// 
// 	// camera
// 	CCharactor* pLookAt = (CCharactor*)CObject::StaticFindObject("Protagonist");
// 	D3DXVECTOR3 lookAtPos = pLookAt->GetWordPos();
// 	D3DXVECTOR3 eyePos;
// 	eyePos.x = lookAtPos.x;
// 	eyePos.y = lookAtPos.y + 1200;
// 	eyePos.z = lookAtPos.z + 1200;
// 	g_Camera.m_fFOV = D3DX_PI / 4;
// 	g_Camera.m_fAspect = (float)g_nWndWidth / (float)g_nWndHeigh;
// 	g_Camera.m_fNearClip = 1.0f;
// 	g_Camera.m_fFarClip = 30000.0f;
// 	g_Camera.SetViewParams( eyePos, lookAtPos ); 
// 
// 	ShowCursor(false);  //隐藏系统光标
// 	CMouse::GetSingleton().LoadMousePicture();	
}


void CGameApp::Update()
{
	if ( ma::GetTimer() )
	{
		ma::GetTimer()->UpdateFrame();
	}

	if (m_pScene)
	{
		m_pScene->Update(m_fElapsedTime);
	}

	//CMouse::GetSingleton().UpdateMousePos(); //鼠标更新

	//g_SceneMng.Update();
}

void CGameApp::Render()
{
	ma::IRender* pRender = ma::GetRender();
	
	pRender->BeginRender();

	if (m_pScene)
	{
		m_pScene->Render(m_pCamera);
	}

	pRender->EndRender();

// 	if (NULL == g_pD3DDevice || 0.00f == g_ElapsedTime)
// 		return ;
// 
// 	g_pD3DDevice->BeginScene();
// 
// 	g_D3DRender.Render();
// 
//   	g_pD3DDevice->EndScene();
// 
// 	g_pD3DDevice->Present(NULL,NULL,NULL,NULL);
}

HRESULT CGameApp::Create(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_CLASSDC;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "MyDemo";
	wcex.hIconSm		= NULL;

	RegisterClassEx(&wcex);

	DWORD dwWindowStyle = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU;
	RECT rc;
	SetRect( &rc, 0, 0, m_nWndWidth, m_nWndHeigh );        
	AdjustWindowRect( &rc, dwWindowStyle, false );

	m_hMainWnd = CreateWindow("MyDemo", "MyDemo", dwWindowStyle,
		100, 100, rc.right-rc.left, rc.bottom-rc.top, NULL, NULL, hInstance, NULL);

	ShowWindow(m_hMainWnd,SW_SHOW);
	UpdateWindow(m_hMainWnd);

	ma::DxRenderDevice* pDxRenderDevice = new ma::DxRenderDevice();
	ma::SetRenderDevice(pDxRenderDevice);
	pDxRenderDevice->Init(m_hMainWnd);

	ma::DxRender* pDxRender = new ma::DxRender();
	ma::SetRender(pDxRender);
	pDxRender->InitDefaultShader();

	ma::Time* pTime = new ma::Time();
	ma::SetTimer(pTime);


//  	if( FAILED( Initialize3DEnvironment() ) )
//  	{
//  		ASSERT(FALSE && "Initialize3DEnvironment()");
//  		SAFE_RELEASE( m_pD3D );
//  		return S_FALSE;
//  	}

	InitGame();

	return S_OK;
}


HRESULT CGameApp::InitDeviceObjects()  
{ 
	return S_OK; 
}

HRESULT CGameApp::RestoreDeviceObjects()  
{
	//g_D3DRender.LoadShader();
	//g_D3DRender.CreateRT();

	return S_OK; 
}


//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CGameApp::InvalidateDeviceObjects()
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CGameApp::DeleteDeviceObjects()
{

	return S_OK;
}



HRESULT CGameApp::Resize3DEnvironment()
{
	HRESULT hr;

	if( FAILED( hr = InvalidateDeviceObjects() ) )
		return hr;

	if( FAILED( hr = m_pd3dDevice->Reset( &m_d3dpp ) ) )
		return hr;

	hr = RestoreDeviceObjects();
	if( FAILED(hr) )
		return hr;

	return S_OK;
}


HRESULT CGameApp::Render3DEnvironment()
{
  	HRESULT hr = S_OK;
// 	if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
// 	{
// 		if( D3DERR_DEVICELOST == hr )
// 			return S_OK;
// 
// 		if( D3DERR_DEVICENOTRESET == hr )
// 		{
// 			if( FAILED( hr = Resize3DEnvironment() ) )
// 				return hr;
// 		}
// 		return hr;
// 	}

	static float timeNow = timeGetTime() / 1000.0f;
	m_fElapsedTime = timeNow;
	timeNow = timeGetTime() / 1000.0f;
	m_fElapsedTime = (timeNow - g_ElapsedTime);

	Update();

	Render();

	return hr;
}



void CGameApp::Run()
{
	MSG msg;
	ZeroMemory(&msg,sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if( PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			if( FAILED( Render3DEnvironment() ) )
				SendMessage( m_hMainWnd, WM_CLOSE, 0, 0 );
		}
	}
}
