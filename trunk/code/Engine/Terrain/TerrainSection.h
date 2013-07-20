#ifndef _TerrainSection_H__
#define _TerrainSection_H__

namespace ma
{
	class Terrain;

	class TerrainSection : public Renderable
	{
	public:

		TerrainSection();
		~TerrainSection();

		void Create(Terrain* pParentSystem, 
			int heightMapX, int heightMapY,
			int xVerts, int yVerts);

		//void Render();

		int GetHeightMapX() {return m_heightMapX;}
		int GetHeightMapY() {return m_heightMapY;}

	private:
		void createVertexData();
		void createIndexData();
		void createVertexDeclation();
		void ChangeGridUV( Vector2& topLeft,  Vector2& topRight,
			Vector2& botomLeft, Vector2& botomRight,
			Uint8 op, Uint8 tri );

	private:

		struct sSectorVertex
		{
			Vector3 vPos;
			Vector3 Normal;
			Vector2 VetrexUV1;
			Vector2 VetrexUV2;
		};

		Terrain* m_pTerrainSystem;

		//IDirect3DVertexBuffer9* m_pSectorVerts;
		//IDirect3DIndexBuffer9*  m_pSectorIndex;
		//IDirect3DVertexDeclaration9* m_TerrainVertexDeclaration;
		

		// 顶点数
		int m_xVerts; 
		int m_yVerts; 

		// Section在Terrain内的位置
		int m_heightMapX;
		int m_heightMapY;	

	};
}



#endif // _TerrainSection_H__


