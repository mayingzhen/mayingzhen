#include "NPC.h"
#include "SinMesh.h"
#include "mouse.h"
#include "public.h"

extern LPDIRECT3DDEVICE9 g_pD3DDevice ;  

CNpc::CNpc()
{
	m_npc = new CSkinMesh;
}

CNpc::~CNpc()
{
	SAFE_DELETE(m_npc);
}

void CNpc::LoadNpc(const char *filename)
{
	m_npc->LoadFromXFile(filename);
}

void CNpc::SetAnimation(const char *anmoName)
{
	m_npc->SetAnimationName(anmoName);

}

void CNpc::SetNpc(D3DXVECTOR3 &pos,float scale,float angle)
{
	m_npc->SetAngle(angle);
	m_npc->SetPos(pos);
	m_npc->SetScale(scale);
}

void CNpc::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_npc->Render();

	if(CMouse::GetSingleton().MouseTest(m_npc)) //…‰œﬂºÏ≤‚
		CMouse::GetSingleton().SetMouseType(DIAOG);
	else
		CMouse::GetSingleton().SetMouseType(COMMON);
}
