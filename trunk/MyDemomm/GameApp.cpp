#include "GameApp.h"
#include "Camera.h"
#include "Terrain.h"
#include "Sky.h"
#include "mouse.h"
#include "Text.h"
#include "lua_state.h"
#include "OctreeSceneManager.h"
#include "ResLoader.h"
#include "CObject.h"
#include "D3DRender.h"

#include "XmlFile.h"

int g_nWndWidth = 800;
int g_nWndHeigh = 600;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL ;   //DIRECT3D 设备

float g_ElapsedTime = 0;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_DESTROY:
			PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
		{
			g_SceneMng.ReleaseAll();
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
	m_hMainWnd = NULL;
	m_pD3D = NULL;				   // The main D3D object
	m_pd3dDevice = NULL;        // The D3D rendering device
	m_pFont = NULL;
}

CGameApp::~CGameApp(void)
{	
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(m_pD3D);

	if ( IsWindow(m_hMainWnd) )
	{
		DestroyWindow(m_hMainWnd);
	}

	UnregisterClass( TEXT("MyDemo"), NULL );
}

BOOL LoadMapStaticItem(const char *szFileName)
{
	CXmlFile fileXml;
	BOOL bRet = fileXml.LoadFromFile(szFileName);
	assert(bRet);
	if( bRet == FALSE )
		return FALSE;

	CXmlNode::INDEX index;
	CXmlNode *pNode = fileXml.FindFirstChild("Item", index);
	while( pNode != NULL )
	{
		CXmlNode *TempNode = NULL; 
		CXmlAttribute *pAttr = NULL;

		pAttr = pNode->FindAttribute("ItemName");
		const char* itemName = pAttr->GetValue();

		TempNode = pNode->FindChild("Position");
		pAttr = TempNode->FindAttribute("x");
		float pos_x = static_cast< float >( atof( pAttr->GetValue() ) );
		pAttr = TempNode->FindAttribute("z");
		float pos_z = static_cast< float >( atof( pAttr->GetValue() ) );

		float fRotate = 0.0f;
		pAttr = TempNode->FindAttribute( "Rotate" );
		if( pAttr != NULL )
			fRotate = static_cast< float >( atof( pAttr->GetValue() ) );
		else
			fRotate = 0.0f;

		float fAngle = 0.0f;
		pAttr = TempNode->FindAttribute( "Angle" );
		if( pAttr != NULL )
			fAngle = static_cast< float >( atof( pAttr->GetValue() ) );
		else
			fAngle = 0.0f;

		TempNode = pNode->FindChild("ResName");
		pAttr = TempNode->FindAttribute("Name");
		const char* resName = pAttr->GetValue();

		float pos_y = CTerrain::GetSingleton().GetHeight(pos_x,pos_z);
		D3DXVECTOR3 Pos(pos_x,pos_y,pos_z);
		g_SceneMng.AddMapObject(Pos,fRotate,fAngle,resName,itemName);

		pNode = fileXml.FindNextChild("Item", index);
	}

	return TRUE;
}


BOOL LoadMapDynamicItem(const char *szFileName)
{
	CXmlFile fileXml;
	BOOL bRet = fileXml.LoadFromFile(szFileName);
	assert(bRet);
	if( bRet == FALSE )
		return FALSE;

	CXmlNode::INDEX index;
	CXmlNode *pNode = fileXml.FindFirstChild("Item", index);
	while( pNode != NULL )
	{
		CXmlNode *TempNode = NULL; 
		CXmlAttribute *pAttr = NULL;

		pAttr = pNode->FindAttribute("ItemName");
		const char* itemName = pAttr->GetValue();

		TempNode = pNode->FindChild("Position");
		pAttr = TempNode->FindAttribute("x");
		float pos_x = static_cast<float>( atof( pAttr->GetValue() ) );
		pAttr = TempNode->FindAttribute("z");
		float pos_z = static_cast<float>( atof( pAttr->GetValue() ) );

		float fRotate = 0.0f;
		pAttr = TempNode->FindAttribute( "Rotate" );
		if( pAttr != NULL )
			fRotate = static_cast< float >( atof( pAttr->GetValue() ) );
		else
			fRotate = 0.0f;

		float fAngle = 0.0f;
		pAttr = TempNode->FindAttribute( "Angle" );
		if( pAttr != NULL )
			fAngle = static_cast< float >( atof( pAttr->GetValue() ) );
		else
			fAngle = 0.0f;

		TempNode = pNode->FindChild("ResName");
		pAttr = TempNode->FindAttribute("Name");
		const char* resName = pAttr->GetValue();

		float pos_y = CTerrain::GetSingleton().GetHeight(pos_x,pos_z);
		D3DXVECTOR3 Pos(pos_x,pos_y,pos_z);
		g_SceneMng.AddCharactor(Pos,fRotate,fAngle,resName,itemName);
		
		pNode = fileXml.FindNextChild("Item", index);
	}

	return TRUE;
}

#include "GEMesh.h"

//初始化游戏数据	
void CGameApp::InitGame()
{
 	CObject::StaticInit();

	//启动资源加载
// 	{
// 		if( ResLoader()->Init() == false )
// 		{
// 			assert(false);
// 			return;
// 		}
// 		char sPath[MAX_PATH] = {0};
// 		::GetCurrentDirectory(MAX_PATH,sPath);
// 		
// 		ResLoader()->SetPath( sPath );
// 	}

	GEMesh testMesh("res\\tiny.x");
	//testMesh.parseX();
	
	CAABB sceneMax;
	sceneMax.vMin = D3DXVECTOR3(-12000, -2000, -12000);
	sceneMax.vMax = D3DXVECTOR3(12000, 5000, 12000);

	g_SceneMng.Init( sceneMax, 8 );
	g_D3DRender.Init();

	CSky::GetSingleton().Create();

	CTerrain::GetSingleton().Create("res\\terrain\\shaolin.Terrain");

	g_SceneMng.CreateScene("res\\terrain\\shaolin.Scene");

	LoadMapStaticItem("res\\mapStatic.xml");
	LoadMapDynamicItem("res\\mapDynamic.xml");
	
	CCharactor* pSelf = (CCharactor*)CObject::StaticFindObject("Protagonist");
	g_SceneMng.SetSelfCha(pSelf);
	
	CMapObj* pHouse = (CMapObj*)CObject::StaticFindObject("house2");
	D3DXVECTOR3 LightAtPos = D3DXVECTOR3(0,1,0)/*pHouse->GetWordPos()*/;
	D3DXVECTOR3 LightPos;
	LightPos.x = LightAtPos.x - 10000;
	LightPos.y = LightAtPos.y + 3000;
	LightPos.z = LightAtPos.z - 10000;
	g_SceneMng.SetLightParams(LightPos,LightAtPos);

	D3DXVECTOR3 lookAtPos = pSelf->GetWordPos();
	D3DXVECTOR3 eyePos;
	eyePos.x = lookAtPos.x;
	eyePos.y = lookAtPos.y + 1200;
	eyePos.z = lookAtPos.z + 1200;
	g_Camera.SetViewParams( eyePos, lookAtPos ); 
	//g_Camera.SetViewParams( g_SceneMng.m_Light.m_vSource, g_SceneMng.m_Light.m_vTarget );
	//g_Camera.m_vSource = eyePos;
	//g_Camera.m_vTarget = lookAtPos;

	//// debug_light	
	//g_SceneMng.AddMapObject( LightPos, 10, 0, "res\\spotlight.x", "light_debug" );

	//g_SceneMng.AddMapObject( LightPos, 350*0.5, 0, "res\\sphere0.x", "light1_debug" );
	
	ShowCursor(false);  //隐藏系统光标
	CMouse::GetSingleton().LoadMousePicture();	
}


void CGameApp::Update()
{
	CMouse::GetSingleton().UpdateMousePos(); //鼠标更新

	g_Camera.Update();
	
	g_SceneMng.Update();
}

void CGameApp::Render()
{
	static float timeNow = timeGetTime() / 1000.0f;
	g_ElapsedTime = timeNow;
	timeNow = timeGetTime() / 1000.0f;
	g_ElapsedTime = (timeNow - g_ElapsedTime);

	if (NULL == g_pD3DDevice || 0.00f == g_ElapsedTime)
		return ;

	g_pD3DDevice->BeginScene();

	// Clear the viewport
	g_pD3DDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0x000000ff, 1.0f, 0L);

	//g_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE,D3DCOLORWRITEENABLE_ALPHA);

  	g_D3DRender.ShadowMapPass();
  
	D3DXMATRIX matView = g_Camera.m_mView;  
 	D3DXMATRIX matProj = g_Camera.m_mProj;
 	g_pD3DDevice->SetTransform(D3DTS_VIEW,&matView);
 	g_pD3DDevice->SetTransform(D3DTS_PROJECTION,&matProj);
 
   	g_D3DRender.GBufferPass();

	//g_SceneMng.RenderSSAO();
  
   	g_D3DRender.DefferdShadow();
   
	g_D3DRender.DeferredLight();
   
  	g_D3DRender.ShadingPass();

	//CSky::GetSingleton().Render();

	//RenderWater();

	CMouse::GetSingleton().Render();

  	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL,NULL,NULL,NULL);
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

	DWORD dwWindowStyle = WS_OVERLAPPEDWINDOW;
	RECT rc;
	SetRect( &rc, 0, 0, g_nWndWidth, g_nWndHeigh );        
	AdjustWindowRect( &rc, dwWindowStyle, false );

	m_hMainWnd = CreateWindow("MyDemo", "MyDemo", dwWindowStyle,
		100, 100, rc.right-rc.left, rc.bottom-rc.top, NULL, NULL, hInstance, NULL);

	ShowWindow(m_hMainWnd,SW_SHOW);
	UpdateWindow(m_hMainWnd);

 	if( FAILED( Initialize3DEnvironment() ) )
 	{
 		ASSERT(FALSE && "Initialize3DEnvironment()");
 		SAFE_RELEASE( m_pD3D );
 		return S_FALSE;
 	}

	InitGame();

	return S_OK;
}


HRESULT CGameApp::Initialize3DEnvironment()
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(NULL == m_pD3D)
	{
		ASSERT(FALSE && "Direct3DCreate9(D3D_SDK_VERSION)");
		return S_FALSE;
	}

	RECT rect;
	GetWindowRect(m_hMainWnd,&rect);

	ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Disable vertical synchronization
	m_d3dpp.BackBufferWidth = g_nWndWidth;
	m_d3dpp.BackBufferHeight = g_nWndHeigh;

	UINT AdapterToUse = D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;

#if SHIPPING_VERSION
	// When building a shipping version, disable NVPerfHUD (opt-out)
#else
	// Look for 'NVIDIA NVPerfHUD' adapter
	// If it is present, override default settings
	for (UINT Adapter = 0; Adapter < m_pD3D->GetAdapterCount(); Adapter++)
	{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT Res;
		Res = m_pD3D->GetAdapterIdentifier(Adapter, 0, &Identifier);
		if ( strcmp(Identifier.Description, "NVIDIA NVPerfHUD") == 0 )
		{
			AdapterToUse = Adapter;
			DeviceType = D3DDEVTYPE_REF;
			//Logger::info(String("Using ") + Identifier.Description + " adapter for debug purposes.");
			break;
		}
	}
#endif

  	HRESULT hr = S_OK;
	hr = m_pD3D->CreateDevice(
		AdapterToUse,
		DeviceType,
		m_hMainWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_pd3dDevice);

	g_pD3DDevice = m_pd3dDevice;

	if( FAILED(hr) )
	{
		ASSERT(FALSE && "m_pD3D->CreateDevice()");
		return E_FAIL;
	}

	// Create a D3DX font object
	hr = D3DXCreateFont(
		m_pd3dDevice,
		14,
		0,
		FW_BOLD,
		0,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Arial"),
		&m_pFont) ;


	// Initialize the app's device-dependent objects
	hr = InitDeviceObjects();
	if( SUCCEEDED(hr) )
	{
		hr = RestoreDeviceObjects();
		if( SUCCEEDED(hr) )
			return S_OK;
	}

	return hr;
}


HRESULT CGameApp::InitDeviceObjects()  
{ 
	return S_OK; 
}

HRESULT CGameApp::RestoreDeviceObjects()  
{
	g_Camera.m_fFOV = D3DX_PI / 4;
	g_Camera.m_fAspect = (float)g_nWndWidth / (float)g_nWndHeigh;
	g_Camera.m_fNearClip = 1.0f;
	g_Camera.m_fFarClip = 30000.0f;

	g_D3DRender.LoadShader();
	g_D3DRender.CreateRT();

	//g_HDRPostProcess.Init();

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
	if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
	{
		if( D3DERR_DEVICELOST == hr )
			return S_OK;

		if( D3DERR_DEVICENOTRESET == hr )
		{
			if( FAILED( hr = Resize3DEnvironment() ) )
				return hr;
		}
		return hr;
	}

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
