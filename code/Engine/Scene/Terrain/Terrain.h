#ifndef _Terrain_H_
#define _Terrain_H_


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
		SHORTV4 pos;
		SHORTV2 uv;
		uint32 tangent_quat;
	};

	class Terrain : public SceneNode
	{
	public:
		Terrain(Scene* pScene,const char* pName = NULL);

		~Terrain();

		DECL_OBJECT(Terrain)

		static void	RegisterAttribute();

		virtual void Update();
	
		bool Intersect(const Ray& ray, OUT Vector3& vPos);

		// Height
		float GetHeight(float fX, float fY) const;
		float GetHeight(int nXVert, int nYVert) const;
		Vector3 GetPos(int nXVert, int nYVert) const;

		Vector2 GetUV(int nXVert, int nYVert) const;

		Vector3 GetNormal(int nXVert, int nYVert) const;
		Vector3 GetNormal(float x, float y) const;

		Vector3 GetTangent(int nXVert, int nYVert) const;
		Vector3 GetTangent(float x, float y) const;

		uint8 GetMaterialID(int nXVert,int nYVert) const;
		SubMaterial* GetMaterial(int nXVert,int nYVert) const;
		SubMaterial* GetMaterialByID(uint8 matID) const;
		UINT GetMaterialCount() const;
		void AddMaterial(SubMaterial* pMateral);
		const char* GetMaterialPath() const;
		void SetMaterialPath(const char* pPath);

		bool GetTriFlip(int nXVert,int nYVert) const;
		void SetTriFlip(int nXVert,int nYVert,bool bFlip);

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
		
		virtual bool IsReady();

	private:
		void SetScene(Scene* pScene);
		void BuildTrunks();
		void BuildVertexDeclaration();

		void ClearTempData();

		bool WorldToCell(float x, float y, OUT int& i, OUT int& j) const;

		template<class T>
		void ResourceMapToData(Resource* pSrcResource,vector<T>& vecDestData,int& nWidth,int& nHeight);

		int CalcIndex(int nXVert, int nYVert) const;

	protected:
		typedef vector< RefPtr<TerrainTrunk> > VEC_TRUNK;
		
		VEC_TRUNK m_vecTrunk;

		int m_nXCellsAmount;
		int m_nYCellsAmount;

		float m_fCellSpacing;
		float m_fHeightSpacing;
		Vector3 m_vStartpoint;
		int	m_nTrunkSize;
		float m_fLodParamDiv;

		UINT m_uNumLods;

		// Height
		string m_strHeightMap;
		RefPtr<Resource> m_pHeightMapData;
		vector<uint16> m_vecHeight;

		// Material
		string m_strMaterialMap;
		RefPtr<Resource> m_pMaterialMapData;
		vector<uint8> m_vecVertexMatID;	
		RefPtr<Material> m_pMaterial;

		// Blend
		string m_strBlendMap;
		RefPtr<Texture> m_pBlendMap;
		int m_nBlendingMultiple;
		Vector2 m_vBlendOffset;

		bool m_bLoadOver;

		RefPtr<VertexDeclaration> m_pVertexDecl;

		friend class TerrainTrunk;

	public:
		Signal< void() > mLoadOverEvent;
	};

	RefPtr<Terrain>	 CreateTerrain();
	RefPtr<Terrain>	 CreateTerrain(const char* pszFile);

}

#endif