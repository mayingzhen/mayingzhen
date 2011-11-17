#ifndef C_NPC_HH__
#define C_NPC_HH__

#include <d3dx9.h>
#include <d3d9.h>

class CSkinMesh;

class CNpc
{
public:
	CNpc();
	~CNpc();

	void LoadNpc(const char *filename);
	void SetAnimation(const char *anmoName);
	void SetNpc(D3DXVECTOR3 &pos,float scale = 1,float angle = 0);
	void Render();

private:
	CSkinMesh *m_npc;
};

#endif

