#pragma once


namespace ma
{
	class Terrain;
	class Camera;
	class TerrainRenderable;

	class TerrainTrunk : public RenderComponent
	{
	public:
		TerrainTrunk(SceneNode* pGameObj);

		~TerrainTrunk();

		void Show(Camera* pCamera);

		bool Init(int i, int j);

		UINT GetLodIndex() const {return m_uLodIndex;}

		int GetIndexX() const{return m_nX;}
		
		int GetIndexY() const{return m_nY;}

		void SetLodParamDiv(float f);

		Terrain* GetTerrain() const {return m_pTerrain;}

	private:
		void BuildLod();
		void BuildVertexBuffer();
		void BuildBody();
		void BuildBorder();
		void BuildSkirt();

		void UpdateTerrainRenderable();

		void AddToIBList( vector< vector<uint16> >& vecIBList, uint8 matID, uint16 triangle[3]);

		void ClearVBTemp();


	private:
		Terrain* m_pTerrain;

		int m_nX; 
		
		int m_nY;

		float m_fLodParam;

		UINT m_uLodIndex;

		typedef vector< RefPtr<TerrainRenderable> > VEC_RENDERABLE;

		struct SkitRenderable
		{
			RefPtr<TerrainRenderable> skirt[SideNum];
		};

		struct TERRAIN_LOD
		{
			RefPtr<VertexBuffer>	m_BodyVB;
			VEC_RENDERABLE			m_vecBody; 
			VEC_RENDERABLE			m_vecBorder;
			vector<SkitRenderable>	m_vecSkirt;
			vector<TERRAIN_VERTEX>	m_vecVBTemp;
		};
		vector<TERRAIN_LOD> m_vecLod;
	};
}

