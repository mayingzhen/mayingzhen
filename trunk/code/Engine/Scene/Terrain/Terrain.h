#pragma once

namespace ma
{
	class Terrain;
	class TerrainTrunk;
	class IndexBuffer;
	class VertexDeclaration;

	enum SkirtSide
	{
		North,
		South,
		West,
		East,
		SideNum,
	};

	struct TERRAIN_VERTEX
	{
		Vector3 pos;
		Vector2 uv;
		Vector3 normal;
		Vector3 tan;
		uint32  color;  //.a ΪMaterialID
		uint32	blend;
	};

	class  Terrain : public SceneNode
	{
		DECL_OBJECT(Terrain)

	public:
		Terrain(Scene* pScene,const char* pName = NULL);

		~Terrain();

		static void	RegisterObject(Context* context);

		virtual void Update();

		bool Intersect(const Ray& ray, OUT Vector3& vPos);

		// Height
		float GetHeight(float fX, float fY) const;
		float GetHeight(int nXVert, int nYVert) const;
		Vector3 GetPos(int nXVert, int nYVert) const;

		// normal
		Vector3 GetNormal(int nXVert, int nYVert) const;
		Vector3 GetNormal(float x, float y) const;

		// tangent
		Vector3 GetTangent(int nXVert, int nYVert) const;
		Vector3 GetTangent(float x, float y) const;

		// Material
		uint8 GetMaterialID(int nXVert,int nYVert) const;
		SubMaterial* GetMaterial(int nXVert,int nYVert) const;
		SubMaterial* GetMaterialByID(uint8 matID) const;
		UINT GetMaterialCount() const;
		void AddMaterial(SubMaterial* pMateral);
		const char* GetMaterialPath() const;
		void SetMaterialPath(const char* pPath);

		// Blend
		UINT GetBlendData(int nXVert, int nYVert) const;

		void GetVertexData(int nXVert,int nYVert,TERRAIN_VERTEX& v) const; 
		VertexDeclaration* GetVertexDeclaration() const {return m_pVertexDecl.get();}

		int GetXCellAmount() const {return m_nXCellsAmount;}
		int GetYCellAmount() const {return m_nYCellsAmount;}
		Vector2 GetCellAmount() const {return Vector2(m_nXCellsAmount,m_nYCellsAmount);}

		UINT GetNumLod() const {return m_uNumLods;}

		TerrainTrunk* GetTerrainTrunkByPos(float x, float y);
		TerrainTrunk* GetTerrainTrunkByIndex(UINT i, UINT j);
		TerrainTrunk* GetTerrainTrunkByVertex(int nXVert, int nYVert);
		UINT GetNumTerrainTrunks() const{return m_vecTrunk.size();}
		TerrainTrunk* GetTerrainTrunkByIndex(UINT i){return m_vecTrunk[i].get();}

		//Serialize
		float GetCellSpacing() const {return m_fCellSpacing;}
		void SetCellSpacing(float fCellSpacing) {m_fCellSpacing = fCellSpacing;}

		float GetHeightSpacing() const {return m_fHeightSpacing;}
		void SetHeightSpcing(float fMaxHeight) {m_fHeightSpacing = fMaxHeight;}

		const Vector3& GetStartPoint() const {return m_vStartpoint;}
		void SetStartPoint(const Vector3& vStartPoint) {m_vStartpoint = vStartPoint;}

		int GetTrunkSize() const {return m_nTrunkSize;}
		void SetTrunkSize(int nTrunkSize);

		float GetLodParamDiv() const {return m_fLodParamDiv;}
		void SetLodParamDiv(float fParamDiv);

		const char* GetHeightMap() const;
		void SetHeightMap(const char* pszFile);

		const char* GetMaterialMap() const;
		void SetMaterialMap(const char* pszFile);

		const char* GetBlendMap() const;
		void SetBlendMap(const char* pszFile);
		const Vector2& GetBlendOffSet() const {return m_vBlendOffset;}
		
		bool OnLoadOver();

		virtual	void OnTransformChange();

	private:
		void BuildTrunks();
		void BuildVertexDeclaration();
		void BuildShareIB();
		void BuildBodyIndexBuffer(UINT m);
		void BuildSkirtIndexBuffer(UINT m);
		void BuildSkirtSideIB(RefPtr<IndexBuffer>& pSkirtIB,
			int nCellAmountSelf,int nCellAmountConnect,
			int nSelfStart,int nSelfStep);

		bool WorldToCell(float x, float y, OUT int& i, OUT int& j) const;

		template<class T>
		void ResourceMapToData(Resource* pSrcResource,vector<T>& vecDestData,uint32& nWidth,uint32& nHeight);

		int CalcIndex(int nXVert, int nYVert) const;

	protected:
		typedef vector< RefPtr<TerrainTrunk> > VEC_TRUNK;
		
		VEC_TRUNK m_vecTrunk;

		uint32 m_nXCellsAmount;
		uint32 m_nYCellsAmount;

		float m_fCellSpacing;
		float m_fHeightSpacing;
		Vector3 m_vStartpoint;
		int	m_nTrunkSize;
		float m_fLodParamDiv;

		UINT m_uNumLods;

		string m_strHeightMap;
		RefPtr<Resource> m_pHeightResource;
		vector<uint16> m_vecHeight;

		string m_strMaterialMap;
		RefPtr<Resource> m_pMaterialResource;
		vector<uint8> m_vecVertexMatID;	
		vector< RefPtr<SubMaterial> >m_vecMaterial;
		RefPtr<MaterialData> m_pMaterialData;

		string m_strBlendMap;
		RefPtr<Resource> m_pBlendResource;
		vector<UINT>  m_vecBlenData;
		RefPtr<Texture> m_pBlendMap;
		int m_nBlendingMultiple;
		Vector2 m_vBlendOffset;
		bool m_bBlendMapToVertex;

		bool m_bLoadOver;

		struct SkirtIB
		{
			RefPtr<IndexBuffer>  SkitIB[SideNum];
		};

		struct LodShareIB
		{
			RefPtr<IndexBuffer>		m_pBodyIB;
			vector<SkirtIB>			m_vecSkitIB;
			vector<uint16>			m_vecBodyIBTemp;
		};
		
		vector<LodShareIB>	m_shareIB;

		RefPtr<VertexDeclaration> m_pVertexDecl;

		friend class TerrainTrunk;
	};

	RefPtr<Terrain>	 CreateTerrain();
	RefPtr<Terrain>	 CreateTerrain(const char* pszFile);

}