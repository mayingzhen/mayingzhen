#ifndef _Terrain_H__
#define _Terrain_H__

#include "Engine/Terrain/ITerrain.h"

namespace ma
{
	class TerrainSection;

	class  Terrain : public ITerrain
	{
	public:
		struct GridInfo
		{
			Uint16 layer0;
			Uint16 layer1;
			Uint8 op0;
			Uint8 op1;
			Uint8 tri;
		};

		struct PixMap 
		{
			int textureId;           
			float left;                 
			float top;                   
			float right;                
			float bottom;
		}; 

		struct TextureInAlta
		{
			std::string mpFilename;
			float uOffset;
			float vOffset;
			float uWidth;
			float vHeight;
		};

	public:
		Terrain();

		~Terrain();

		void		Create(const char* pPath);
		
		void		Update();

		float		GetHeight(float x, float z);

		int			HeightMapH() {return m_heightMapW;}

		int			HeightMapW() {return m_heightMapH;}

		const AABB&	GetWorldAABB() {return m_aabbWorld;}
	
		GridInfo	GetGridInfo(int xGrid,int yGrid) {return m_GridInfoArray[xGrid + yGrid * (m_heightMapW - 1)];}

		PixMap		GetPixMapByIndex(UINT index) {return m_PixMapArray[index];}	

		Vector3		GetPos(int row, int col);

		Vector3		GetNormal(int row, int col);

		bool		GetInTextureAtlasUV(int textureID,float& texU, float& texV,float& texW, float& texH);

		UINT		GetTextureFileSize() {return m_vTextureFileNames.size();}

		Material*	GetMaterial() {return m_pMaterial;}

	private:
		void		LoadTerrain(const char* pPath);

		void		LoadTextAltas(const char* pPath);

		void		LoadHeightMap(const char* pFileName);

		void		LoadGridInfo(const char* pFileName);

		float		GetHeightmapData(int row, int col);

	private:

		std::vector<TerrainSectionPtr>	m_arrSection;
		//std::vector<GameObject*>		m_arrGameObj;

		std::vector<PixMap>				m_PixMapArray;

		std::vector<GridInfo>			m_GridInfoArray;

		std::vector<std::string>		m_vTextureFileNames;

		std::vector<TextureInAlta>		m_TextureAtlas;
		
		ref_ptr<Texture>				m_pAltasTex;
		
		Material*						m_pMaterial;

		Vector3							m_scale;
		
		Vector3							m_WorldOffset;

		// heightmap 
		int								m_heightMapW;
		int								m_heightMapH;
		float*							m_heightmap;

		// sector
		int								m_sectorShift;
		int								m_sectorUnits;
		int								m_sectorVerts;
		int								m_sectorCountX;
		int								m_sectorCountY;

		AABB							m_aabbWorld;
	};
}



#endif // _Terrain_H__


