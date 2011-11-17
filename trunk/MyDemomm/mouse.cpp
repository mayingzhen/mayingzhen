#include "mouse.h"
#include "sprite.h"
#include "SinMesh.h"

#include <Windows.h>

//extern HWND g_hwnd;

CMouse::CMouse()
{
	m_mouseType = COMMON;
	m_mouseCommon = new CSprite;
	m_mouseDiaog = new CSprite;
	m_mouseFight = new CSprite;	

}

CMouse::~CMouse()
{
	CleanUp();
}

void CMouse::CleanUp()
{
	SAFE_DELETE(m_mouseCommon);
	SAFE_DELETE(m_mouseDiaog);
	SAFE_DELETE(m_mouseFight);
}

void CMouse::LoadMousePicture()
{
	m_mouseCommon->SetTexture("res\\mouse1.bmp",0xffffffff);
	m_mouseDiaog->SetTexture("res\\mouse2.png", 0xffffffff);
}

void CMouse::Render()
{
	if (m_mouseType == COMMON)	
		m_mouseCommon->Render();
	else if (m_mouseType == DIAOG)
		m_mouseDiaog->Render();
	else if (m_mouseType == FIGHT)
		m_mouseFight->Render();
}

void CMouse::UpdateMousePos()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	//ScreenToClient(g_hwnd,&cursorPos);
	HWND hwnd = GetForegroundWindow(); //»ñµÃ´°¿Ú¾ä±ú
	ScreenToClient(hwnd,&cursorPos);

	m_x = cursorPos.x;
	m_y = cursorPos.y;

	m_mouseDiaog->SetPos(m_x,m_y);
	m_mouseCommon->SetPos(m_x,m_y);
	m_mouseFight->SetPos(m_x,m_y);
}

void CMouse::SetMouseType(MOUSE_TYPE mouseType)
{
	m_mouseType = mouseType; 
}

bool CMouse::MouseTest(CSkinMesh *mesh)
{
 	CRay ray(m_x,m_y);

 	bool res = false; 
	//res = mesh->RaySphereIntTest(ray);

	if (res)
	{
		int i = 5;
	}

	return res;
}