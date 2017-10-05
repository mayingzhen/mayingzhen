#include "Terrain.h"
#include "TerrainTrunk.h"
#include "../../RenderSystem/IRenderDevice/VertexDeclaration.h"

namespace ma
{
	Terrain::Terrain(Scene* pScene,const char* pName)
		:SceneNode(pScene,pName)
	{
		m_nXCellsAmount = m_nYCellsAmount = 0;
		m_fHeightSpacing = m_fCellSpacing = 1.0f;
		m_vStartpoint = Vector3::ZERO;
		m_nTrunkSize = 0;
		m_fLodParamDiv = 0;
		m_uNumLods = 1;
		m_nBlendingMultiple = 1;
		m_bLoadOver = false;
	}

	Terrain::~Terrain()
	{
	}

	void Terrain::RegisterAttribute()
	{
		ACCESSOR_ATTRIBUTE(Terrain, "CellSpacing", GetCellSpacing, SetCellSpacing, float, 1.0f, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(Terrain, "HeightSpacing", GetHeightSpacing, SetHeightSpcing, float, 1.0, AM_DEFAULT);
		REF_ACCESSOR_ATTRIBUTE(Terrain, "StartPoint", GetStartPoint, SetStartPoint, Vector3, Vector3::ZERO, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(Terrain, "TrunkSize", GetTrunkSize, SetTrunkSize, int, 32, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(Terrain, "LodParamDiv", GetLodParamDiv, SetLodParamDiv, float, -1.0f, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(Terrain, "HeightMapPath", GetHeightMap, SetHeightMap, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(Terrain, "MaterialMapPath", GetMaterialMap, SetMaterialMap, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(Terrain, "BlendMapPath", GetBlendMap, SetBlendMap, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(Terrain, "MaterialDataPath", GetMaterialPath, SetMaterialPath, const char*, NULL, AM_DEFAULT);
	}

	void Terrain::Update()
	{
		SceneNode::Update();

		IsReady();

		for (UINT i = 0; i < m_vecTrunk.size(); ++i)
		{
			m_vecTrunk[i]->Update();
		}
	}

	void Terrain::ClearTempData()
	{
		for (UINT i = 0; i < m_vecTrunk.size(); ++i)
		{
			m_vecTrunk[i]->ClearTempData();
		}

	}

	void Terrain::SetScene(Scene* pScene)
	{
		SceneNode::SetScene(pScene);

		for (uint32 i = 0; i < m_vecTrunk.size(); ++i)
		{
			m_vecTrunk[i]->SetSceneNode(this);
		}
	}

	void Terrain::BuildTrunks()
	{
		int nTrunkXCellAmont = Math::ICeil(m_nXCellsAmount / (float)m_nTrunkSize);
		int nTrunkYCellAmont = Math::ICeil(m_nYCellsAmount / (float)m_nTrunkSize);

		for (int i =0; i < nTrunkYCellAmont; ++i)
		{
			for (int j = 0; j < nTrunkXCellAmont; ++j)
			{
				TerrainTrunk* pTrunk = new TerrainTrunk(this);

				if(!pTrunk->Init(j, i))
				{
					ASSERT(false);
					return;
				}

				m_vecTrunk.push_back(pTrunk);
			}
		}

		return;
	}

	void Terrain::SetTrunkSize(int nTrunkSize)
	{
		m_nTrunkSize = nTrunkSize;

		m_uNumLods = nTrunkSize / 2;
		
		if (m_fLodParamDiv <= 0)
		{
			m_uNumLods = 1;
		}
		else
		{
			m_uNumLods = Math::Clamp<uint32>(m_uNumLods, 1, 4);
		}
	}

	void Terrain::SetLodParamDiv(float fParamDiv)
	{
		if (m_fLodParamDiv <= 0)
		{
			m_uNumLods = 1;
		}

		m_fLodParamDiv = fParamDiv;

		for (VEC_TRUNK::iterator iter = m_vecTrunk.begin();iter != m_vecTrunk.end();++iter)
		{
			(*iter)->SetLodParamDiv(fParamDiv);
		}
	}

	TerrainTrunk* Terrain::GetTerrainTrunkByPos( float x, float y )
	{
		int nx = 0;
		int ny = 0;
		bool bRes = WorldToCell(x,y,nx,ny);
		ASSERT(bRes);
		if (!bRes)
			return NULL;

		return this->GetTerrainTrunkByIndex(nx, ny);
	}

	TerrainTrunk* Terrain::GetTerrainTrunkByVertex(int nXVert, int nYVert)
	{
		UINT i = nXVert / m_nXCellsAmount;
		UINT j = nYVert / m_nYCellsAmount;
		return this->GetTerrainTrunkByIndex(i, j);
	}

	TerrainTrunk* Terrain::GetTerrainTrunkByIndex(UINT i, UINT j)
	{
		UINT nTrunkXCellAmont = Math::ICeil(m_nXCellsAmount/(float)m_nTrunkSize);
		UINT nTrunkYCellAmont = Math::ICeil(m_nYCellsAmount/(float)m_nTrunkSize);
		i = Math::Clamp<int>(i, 0, nTrunkXCellAmont-1);
		j = Math::Clamp<int>(j, 0, nTrunkYCellAmont-1);
		UINT index = i + j * nTrunkXCellAmont;
		ASSERT(index >= 0 && index < m_vecTrunk.size());
		if (index >= m_vecTrunk.size())
			return NULL;

		TerrainTrunk* pTerrainTrunk = m_vecTrunk[index].get();
		ASSERT(pTerrainTrunk->GetIndexX() == i && pTerrainTrunk->GetIndexY() == j);
		return pTerrainTrunk;
	}

	void Terrain::BuildVertexDeclaration()
	{
		VertexElement element[3];
		element[0] = VertexElement(0,0,DT_SHORT4N,DU_POSITION,0);
		element[1] = VertexElement(0,8,DT_SHORT2N,DU_TEXCOORD,0);
		element[2] = VertexElement(0,12,DT_UBYTE4N,DU_TANGENT,0);
		m_pVertexDecl = GetRenderSystem()->CreateVertexDeclaration(element,3);

		ASSERT(sizeof(TERRAIN_VERTEX) == m_pVertexDecl->GetStreanmStride(0));
	}


	const char* Terrain::GetHeightMap() const
	{
		return m_strHeightMap.c_str();
	}

	void Terrain::SetHeightMap(const char* pszFile)
	{
		m_strHeightMap = pszFile ? pszFile : "";

		m_pHeightMapData = CreateResource(pszFile);
	}

	const char* Terrain::GetMaterialMap() const
	{
		return m_strMaterialMap.c_str();
	}

	void Terrain::SetMaterialMap(const char* pszFile)
	{
		m_strMaterialMap = pszFile ? pszFile : "";

		m_pMaterialMapData = CreateResource(pszFile);
	}

	const char* Terrain::GetMaterialPath() const
	{
		return m_pMaterial ? m_pMaterial->GetResPath() : NULL;
	}

	void Terrain::SetMaterialPath(const char* pPath)
	{
		m_pMaterial = CreateMaterial(pPath);
	}

	const char* Terrain::GetBlendMap() const
	{
		return m_strBlendMap.c_str();
	}

	void Terrain::SetBlendMap(const char* pszFile)
	{
		m_strBlendMap = pszFile ? pszFile : "";

		m_pBlendMap = CreateTexture(pszFile);
	}
	
	bool Terrain::IsReady()
	{
		if (m_bLoadOver)
			return true;

		if (!Serializable::IsReady())
			return false;

		if (m_pMaterial == NULL || !m_pMaterial->IsReady())
			return false;

		if (m_pHeightMapData == NULL || !m_pHeightMapData->IsReady())
			return false;
		
		if (m_pMaterialMapData && !m_pMaterialMapData->IsReady())
			return false;

		if (m_pBlendMap == NULL || !m_pBlendMap->IsReady())
				return false;

		if (m_pHeightMapData)
		{
			ResourceMapToData<uint16>(m_pHeightMapData.get(),m_vecHeight,m_nXCellsAmount,m_nYCellsAmount);
			m_pHeightMapData = NULL;
		}

		if (m_pMaterialMapData)
		{
			int matW, matH;
			ResourceMapToData<uint8>(m_pMaterialMapData.get(),m_vecVertexMatID,matW,matH);
			m_pMaterialMapData = NULL;
		}

		if (m_pBlendMap)
		{
			m_nBlendingMultiple = m_pBlendMap->GetWidth() / m_nXCellsAmount;
			m_vBlendOffset = Vector2(0.5f / m_pBlendMap->GetWidth(), 0.5f / m_pBlendMap->GetHeight());
		}

		BuildVertexDeclaration();

		BuildTrunks();

		ClearTempData();
		
		m_bLoadOver = true;
		mLoadOverEvent.trigger();

		return true;
	}	

	float Terrain::GetHeight(int nXVert, int nYVert) const
	{
		return m_vStartpoint.z + m_vecHeight[this->CalcIndex(nXVert, nYVert)] * m_fHeightSpacing / 65535.0f;
	}

	float Terrain::GetHeight( float fX, float fY ) const
	{
		fX = (fX - m_vStartpoint.x)/m_fCellSpacing;
		fY = (fY - m_vStartpoint.y)/m_fCellSpacing;
		int i = (int)fX;
		int j = (int)fY;
		if (i < 0 || i >= m_nXCellsAmount || j < 0 || j >= m_nYCellsAmount)
		{
			i = (i<0?0:i);
			i = (i>=m_nXCellsAmount?m_nXCellsAmount:i);
			j = (j<0?0:j);
			j = (j>=m_nYCellsAmount?m_nYCellsAmount:j);
		}

		/*
		  0      1
		   ---->
		  |  / |
		  | /  |
		2 V ---  3   
		*/

		float fHeight0 = this->GetHeight(i, j);
		if (i >= m_nXCellsAmount || j >= m_nYCellsAmount)
		{
			return fHeight0;
		}

		float fHeight1 = this->GetHeight(i+1, j);
		float fHeight2 = this->GetHeight(i, j+1);
		float fHeight3 = this->GetHeight(i+1, j+1);

		float u = fX - (int)fX;
		float v = fY - (int)fY;

		if (u + v < 1.0f)// ио╠ъ
		{
			return fHeight0 + u*(fHeight1 - fHeight0) + v*(fHeight2 - fHeight0);
		}
		else //об╠ъ
		{
			return fHeight3 + (1.f - u)*(fHeight2 - fHeight3) + (1.f - v)*(fHeight1 - fHeight3);
		}
	}

	int Terrain::CalcIndex( int nXVert, int nYVert ) const
	{
		return nYVert * (m_nXCellsAmount + 1) + nXVert;
	}

	template<class T>
	void Terrain::ResourceMapToData(Resource* pSrcResource,vector<T>& vecDestData,int& nWidth,int& nHeight)
	{
		ASSERT(pSrcResource && pSrcResource->GetDataStream());
		if (pSrcResource == NULL || pSrcResource->GetDataStream() == NULL)
			return;

		const char* pszName = pSrcResource->GetResPath();
		void* pMemory = pSrcResource->GetDataStream()->GetPtr(); 
		uint32 nSizeBytes =	pSrcResource->GetDataStream()->GetSize();

		ImageData imageData;
		Texture::BuildImageData(pszName,pMemory,nSizeBytes,imageData);

		PixelBox src = imageData.GetPixelBox(0, 0);
		nWidth = imageData.m_nWidth;
		nHeight = imageData.m_nHeight; 
		PixelFormat format = src.format;
		ASSERT( PixelUtil::getNumElemBytes(src.format) == sizeof(T) );
		if (PixelUtil::getNumElemBytes(src.format) != sizeof(T))
			return;

		vecDestData.resize((nWidth + 1) * (nHeight + 1));

		for (int i = 0; i < nWidth + 1;++i)
		{
			int m = i;
			if (i >= nWidth)
			{
				m = nWidth - 1;
			}

			for (int j = 0; j < nHeight + 1;++j)
			{
				int n = j;
				if (j >= nHeight)
				{
					n = nHeight - 1;
				}

				uint8* srcP = (uint8*)src.data + n * src.rowPitch * sizeof(T) + m * sizeof(T);
				uint8* desP = (uint8*)&vecDestData[0]  + j * (nWidth + 1) * sizeof(T) + i * sizeof(T);
				memcpy(desP, srcP, sizeof(T));
			}
		}
	}


	bool Terrain::Intersect( const Ray& ray, OUT Vector3& vPos )
	{
		Vector3 vPosition = ray.getOrigin();

		Vector3 rayStep = ray.getDirection() * m_fCellSpacing; 
		//Vector3 rayStartPosition =  vPosition;

		// Linear search - Loop until find a point inside and outside the terrain 
		Vector3 lastRayPosition = vPosition; 
		vPosition += rayStep;
		float height = GetHeight(vPosition.x, vPosition.y); 

		int nTimes = 0;
		while (vPosition.z > height) 
		{
			lastRayPosition = vPosition; 
			vPosition += rayStep; 

			if (vPosition.x < m_vStartpoint.x) 
			{
				if (rayStep.x <= 0)
					return false;
			}

			if (vPosition.x > m_vStartpoint.x+m_nXCellsAmount*m_fCellSpacing)
			{
				if (rayStep.x >= 0)
					return false;
			}

			if (vPosition.y < m_vStartpoint.y)
			{
				if (rayStep.y <= 0)
					return false;
			}

			if (vPosition.y > m_vStartpoint.y+m_nYCellsAmount*m_fCellSpacing)
			{
				if (rayStep.y >= 0)
					return false;
			}

			height = this->GetHeight(vPosition.x, vPosition.y); 

			if (nTimes++ > 2000)
			{
				return false;
			}
		} 

		// If the ray collides with the terrain 
		//if (height >= 0)
		{
			Vector3 startPosition = lastRayPosition; 
			Vector3 endPosition = vPosition; 

			// Binary search. Find the exact collision point 
			for (int i= 0; i<32; i++) 
			{ 
				// Binary search pass 
				Vector3 middlePoint = (startPosition + endPosition) * 0.5f; 
				if (middlePoint.z < height) 
					endPosition = middlePoint; 
				else 
					startPosition = middlePoint;
			}

			vPos = (startPosition +endPosition) * 0.5f; 
			return true;
		} 
		return false; 
	}

	bool Terrain::WorldToCell( float fX, float fY, OUT int& i, OUT int& j ) const
	{
		i = (int)::floorf((fX - m_vStartpoint.x)/m_fCellSpacing);
		j = (int)::floorf((fY - m_vStartpoint.y)/m_fCellSpacing);

		if (i < 0 || i >= m_nXCellsAmount || j < 0 || j >= m_nYCellsAmount)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	Vector3 Terrain::GetPos(int nXVert, int nYVert) const
	{
		ASSERT(nXVert >=0 && nXVert <= m_nXCellsAmount);
		ASSERT(nYVert >=0 && nYVert <= m_nYCellsAmount);
		if (nXVert < 0 || nXVert > m_nXCellsAmount)
			return Vector3::ZERO;

		if (nYVert < 0 || nYVert > m_nYCellsAmount)
			return Vector3::ZERO;

		Vector3 vPos;
		vPos.x = m_vStartpoint.x + nXVert * m_fCellSpacing;
		vPos.y = m_vStartpoint.z + nYVert * m_fCellSpacing;
		vPos.z = GetHeight(nXVert,nYVert);	

		return vPos;
	}

	Vector2 Terrain::GetUV(int nXVert, int nYVert) const
	{
		return Vector2((float)nXVert / (float)m_nXCellsAmount,(float)nYVert / (float)m_nYCellsAmount);
	}

	Vector3 Terrain::GetNormal(int nXVert, int nYVert) const
	{
		if (nXVert <= 0 || nXVert >= m_nXCellsAmount)
		{
			return Vector3::UNIT_Y;
		}

		if (nYVert <= 0 || nYVert >= m_nYCellsAmount)
		{
			return Vector3::UNIT_Y;
		}

		float fHeight1 = this->GetHeight(nXVert - 1, nYVert);
		float fHeight2 = this->GetHeight(nXVert, nYVert - 1);
		float fHeight3 = this->GetHeight(nXVert + 1, nYVert);
		float fHeight4 = this->GetHeight(nXVert, nYVert + 1);
		Vector3 vNormal(fHeight1 - fHeight3, 2 * m_fCellSpacing, fHeight2 - fHeight4);
		vNormal.normalise();
		return vNormal;
	}

	Vector3 Terrain::GetNormal(float x, float y) const
	{
		float flip = 1.f;
		float fStep = m_fCellSpacing*0.5f;
		Vector3 here (x, y, this->GetHeight(x, y));
		Vector3 right (x+fStep, y, this->GetHeight(x+fStep, y));
		Vector3 up (x, y+fStep, this->GetHeight(x, y+fStep));
		if (right.x >= m_vStartpoint.x + m_fCellSpacing*m_nXCellsAmount)
		{
			flip *= -1;
			right = Vector3(x-fStep, y, this->GetHeight(x-fStep, y));
		}
		if (up.y >=m_vStartpoint.y + m_fCellSpacing*m_nYCellsAmount)
		{
			flip *= -1;
			up = Vector3(x, y-fStep, this->GetHeight(x, y-fStep));
		}
		right -= here;
		up -= here;

		Vector3 norm = right.crossProduct(up)*flip;
		norm.normalise();

		return norm;
	}

	Vector3 Terrain::GetTangent(int nXVert, int nYVert) const
	{
		if (nXVert <= 0 || nXVert >= m_nXCellsAmount)
		{
			return Vector3::UNIT_X;
		}
		if (nYVert <= 0 || nYVert >= m_nYCellsAmount)
		{
			return Vector3::UNIT_X;
		}

		float fHeight1 = this->GetHeight(nXVert - 1, nYVert);
		float fHeight3 = this->GetHeight(nXVert + 1, nYVert);
		Vector3 vTan(2 * m_fCellSpacing, fHeight3 - fHeight1, 0);
		vTan.normalise();
		return vTan;
	}

	Vector3 Terrain::GetTangent(float x, float y) const
	{
		float flip = 1;
		float fStep = m_fCellSpacing*0.5f;
		Vector3 here(x, y, this->GetHeight(x,y));
		Vector3 right(x+fStep, y, this->GetHeight(x+fStep, y));
		if (right.x >= m_vStartpoint.x + m_fCellSpacing*m_nXCellsAmount)
		{
			flip *= -1;
			right = Vector3(x-fStep, y, this->GetHeight(x-fStep, y));
		}

		right -= here;
		Vector3 vReturn = right*flip;
		vReturn.normalise();
		return vReturn;
	}

	// Material
	uint8 Terrain::GetMaterialID(int nXVert,int nYVert) const
	{
		if ( m_vecVertexMatID.empty() )
			return 0;

		uint32 nIndex = nXVert + nYVert * (m_nXCellsAmount + 1);
		ASSERT(nIndex >= 0 && nIndex < m_vecVertexMatID.size());
		if (nIndex < 0 || nIndex >= m_vecVertexMatID.size())
			return 0;

		return m_vecVertexMatID[nIndex];	
	}

	SubMaterial* Terrain::GetMaterialByID(uint8 matID) const
	{
		if (m_pMaterial == NULL)
			return NULL;

		ASSERT(matID < m_pMaterial->GetLodSubNumber(0));
		if (matID >= m_pMaterial->GetLodSubNumber(0))
			return NULL;

		return m_pMaterial->GetLodSubByIndex(0,matID);
	}

	UINT Terrain::GetMaterialCount() const
	{
		if (m_pMaterial == NULL)
			return 0;

		return m_pMaterial->GetLodSubNumber(0);
	}

	void Terrain::AddMaterial(SubMaterial* pMateral)
	{
		if (m_pMaterial == NULL)
			return;

		m_pMaterial->AddSubMaterial(0,pMateral);
	}

	SubMaterial* Terrain::GetMaterial(int nXVert,int nYVert) const
	{
		return GetMaterialByID( GetMaterialID(nXVert,nYVert) );
	}

	bool Terrain::GetTriFlip(int nXVert,int nYVert) const
	{
		return false;
	}

	void Terrain::SetTriFlip(int nXVert,int nYVert,bool bFlip)
	{

	}

	RefPtr<Terrain>	 CreateTerrain()
	{
		Terrain* pTerrian = new Terrain(NULL);
		return pTerrian;
	}

	RefPtr<Terrain>	 CreateTerrain(const char* pszFile)
	{
		Terrain* pTerrian = new Terrain(NULL);
		pTerrian->LoadFromXML(pszFile);
		pTerrian->IsReady();
		return pTerrian;
	}
}
