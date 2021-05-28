#pragma once


namespace ma
{
	class Terrain;
	class Camera;
	class TerrainRenderable;

	class TerrainTrunk : public RenderComponent
	{
	public:
		TerrainTrunk(Terrain* pTerrain);

		~TerrainTrunk();

		virtual	void	Update() override;

		bool Init(int i, int j);

		uint32_t GetLodIndex() const {return m_uLodIndex;}

		int GetIndexX() const{return m_nX;}
		
		int GetIndexY() const{return m_nY;}

		void SetLodParamDiv(float f);

		Terrain* GetTerrain() const {return m_pTerrain;}

		const AABB2D& GetUVAABB() const {return m_uvAABB;}

	private:
		void BuildLod();
		void BuildVertexBuffer();
		void BuildBody();
		void BuildBorder();
		void BuildSkirt();

		void BuildIndexBuffer();
		void BuildSkirtIndexBuffer();
		void BuildSkirtSideIB(RefPtr<IndexBuffer>& pSkirtIB,
			int nCellAmountSelf,int nCellAmountConnect,
			int nSelfStart,int nSelfStep);

		void UpdateRenderable();

		void AddToIBList( vector< vector<uint16_t> >& vecIBList, uint8_t matID, uint16_t triangle[3]);

		void ClearTempData();

	private:
		Terrain* m_pTerrain;

		int m_nX; 
		
		int m_nY;

		float m_fLodParam;

		uint32_t m_uLodIndex;

		struct SkitIB
		{
			RefPtr<IndexBuffer> skirtIB[SideNum];
		};

		std::vector< std::set<uint8_t> > m_vecSetMatIDTemp;
		std::vector< std::vector<uint8_t> > m_vecTriangleMatID;

		std::vector< RefPtr<VertexBuffer> > m_vecVBTemp;
		std::vector< std::vector<TERRAIN_VERTEX> > m_vecVBDataTemp;

		std::vector< RefPtr<IndexBuffer> > m_vecIBTemp;
		std::vector< std::vector<uint16_t> > m_vecIBDataTemp;

		std::vector< std::vector<SkitIB> > m_vecSkirt;

		AABB2D m_uvAABB;

		typedef std::vector< RefPtr<TerrainRenderable> > VEC_RENDERABLE;

		struct SkitRenderable
		{
			RefPtr<TerrainRenderable> skirt[SideNum];
		};

		struct TERRAIN_LOD
		{
			VEC_RENDERABLE			m_vecBody;
			VEC_RENDERABLE			m_vecBorder;

			std::vector<SkitRenderable>	m_vecSkirt;

		};
		std::vector<TERRAIN_LOD> m_vecLodRenderable;

		friend class Terrain;
	};

	class TerrainRenderProxy : public RenderProxy
	{

	public:

		TerrainRenderProxy();

		void UpdateLod(const std::vector< RefPtr<TerrainRenderable> >& vecRenderable);

		virtual uint32_t GetRenderableCount() const override;
		virtual Renderable* GetRenderableByIndex(uint32_t index) const override;

	private:

		std::vector< RefPtr<TerrainRenderable> > m_vecRenderable;

	};

}

