#ifndef C_TERRAIN_H__
#define C_TERRAIN_H__


#include "public.h"
#include "effect_file.h"
#include "TextureAtlas.h"
#include "CObject.h"


class CTerrain : public CSingleton<CTerrain>
{
public:
	struct g_info
	{
		uint16 layer0;
		uint16 layer1;
		uint8 op0;
		uint8 op1;
		uint8 tri;
	};

	struct Pixmap 
	{
		int textureId;           
		float left;                 
		float top;                   
		float right;                
		float bottom;
	};               

public:
	CTerrain();
	~CTerrain();

	void Create(std::string sTerrainName);
	void LoadTerrain(std::string sTerrainName);
	bool Render();

	float GetHeight(float x, float z);

	int HeightMapH() {return m_heightMapW;}
	int HeightMapW() {return m_heightMapH;}
	
	void GetPos(int row, int col, D3DXVECTOR3& vWorldPos);
	void GetNormal(int row, int col, D3DXVECTOR3& vNormal);

private:
	bool LoadTexture(std::string fileName);
	void LoadHeightMap(const char* pFileName);
	void LoadGridInfo(const char* pFileName);

	float GetHeightmapData(int row, int col);

public:
	std::vector<Pixmap> m_PixMapArray;
	std::vector<g_info> m_GridInfoArray;

	std::vector<std::string> m_vTextureFileNames;
	CTextureAtlas m_TextureAtlas;
	IDirect3DTexture9* m_pAltasTex;

	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_WorldOffset;

	// heightmap 
	int m_heightMapW;
	int m_heightMapH;
	float* m_heightmap;

	// sector
	int m_sectorShift;
	int m_sectorUnits;
	int m_sectorVerts;
	int m_sectorCountX;
	int m_sectorCountY;
};

#endif // __terrainH__


