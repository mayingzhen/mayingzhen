#ifndef _Terrain_H__
#define _Terrain_H__


namespace ma
{
	class TerrainSection;

	class ENGINE_API Terrain 
	{
	public:
		struct g_info
		{
			Uint16 layer0;
			Uint16 layer1;
			Uint8 op0;
			Uint8 op1;
			Uint8 tri;
		};

		struct Pixmap 
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

		void	Create(const char* pPath);
		
		void	Render();

		float	GetHeight(float x, float z);

		int		HeightMapH() {return m_heightMapW;}
		int		HeightMapW() {return m_heightMapH;}

		void	GetPos(int row, int col, Vector3& vWorldPos);
		void	GetNormal(int row, int col, Vector3& vNormal);

		bool	GetInTextureAtlasUV(int textureID,float& texU, float& texV,float& texW, float& texH);

	private:
		void	LoadTerrain(const char* pPath);
		void	LoadTextAltas(const char* pPath);
		void	LoadHeightMap(const char* pFileName);
		void	LoadGridInfo(const char* pFileName);

		float	GetHeightmapData(int row, int col);

	public:

		std::vector<TerrainSection*>	m_arrSection;

		std::vector<Pixmap>				m_PixMapArray;
		std::vector<g_info>				m_GridInfoArray;

		std::vector<std::string>		m_vTextureFileNames;
		std::vector<TextureInAlta>		m_TextureAtlas;
		Texture*						m_pAltasTex;
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
	};
}



#endif // _Terrain_H__


