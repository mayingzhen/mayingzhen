#ifndef C_TERRAIN_SECTION_H__
#define C_TERRAIN_SECTION_H__

#include "public.h"
#include "CObject.h"

class CTerrain;

class CTerrainSection : public CObject
{
public:

	CTerrainSection();
	~CTerrainSection();

	void Create(CTerrain* pParentSystem, 
		int heightMapX, int heightMapY,
		int xVerts, int yVerts);

	void Render();

private:
	void BuildVertexBuffer();
	void BuildIndexBuffer();
	void CreateVertexDecl();
	void ChangeGridUV( D3DXVECTOR2& topLeft,  D3DXVECTOR2& topRight,
		D3DXVECTOR2& botomLeft, D3DXVECTOR2& botomRight,
		uint8 op, uint8 tri );

private:

	struct sSectorVertex
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 Normal;
		D3DXVECTOR2 VetrexUV1;
		D3DXVECTOR2 VetrexUV2;
	};

	CTerrain* m_pTerrainSystem;

	IDirect3DVertexBuffer9* m_pSectorVerts;
	IDirect3DIndexBuffer9*  m_pSectorIndex;
	IDirect3DVertexDeclaration9* m_TerrainVertexDeclaration;

	// 顶点数
	int m_xVerts; 
	int m_yVerts; 

	// Section在Terrain内的位置
	int m_heightMapX;
	int m_heightMapY;	

};

#endif // __terrainH__


