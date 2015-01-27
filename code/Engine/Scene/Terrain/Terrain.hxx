#include "Terrain.h"
#include "TerrainTrunk.h"
#include "../../RenderSystem/IRenderDevice/VertexDeclaration.h"

namespace ma
{
	IMPL_OBJECT(Terrain,SceneNode)

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
		m_bBlendMapToVertex = false;
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

		OnLoadOver();
	}

	void Terrain::ClearTempData()
	{
		for (UINT i = 0; i < m_shareIB.size(); ++i)
		{
			m_shareIB[i].m_vecBodyIBTemp.clear();
		}

		for (UINT i = 0; i < m_vecTrunk.size(); ++i)
		{
			m_vecTrunk[i]->ClearVBTemp();
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
		if (index < 0 || index >= m_vecTrunk.size())
			return NULL;

		TerrainTrunk* pTerrainTrunk = m_vecTrunk[index].get();
		ASSERT(pTerrainTrunk->GetIndexX() == i && pTerrainTrunk->GetIndexY() == j);
		return pTerrainTrunk;
	}

	void Terrain::BuildSkirtSideIB(RefPtr<IndexBuffer>& pSkirtIB,
										  int nCellAmountSelf,int nCellAmountConnect,
										  int nSelfStart,int nSelfStep)
	{
		UINT nIndexCount = (nCellAmountSelf + nCellAmountConnect)*3;

		vector<uint16> pIndexData;
		pIndexData.resize(nIndexCount);

		ASSERT(nCellAmountSelf%nCellAmountConnect == 0);
		int nStep = nCellAmountSelf/nCellAmountConnect;

		int baseIndex = 0;

		vector<uint16> indexList;

		for (int i = 0;i< nCellAmountConnect + 1;++i)
		{
			indexList.push_back(nSelfStart + i * nStep * nSelfStep);
		}

		for (int i = 0;i< nCellAmountSelf+1;++i)
		{		
			indexList.push_back(nSelfStart + i * nSelfStep);
		}

		for (int i = 0;i< nCellAmountConnect;++i)
		{
			for (int j = 0;j< nStep;++j)
			{
				if (j == 0)
				{
					pIndexData[baseIndex]     = indexList[i];
					pIndexData[baseIndex + 1] = indexList[nCellAmountConnect+1 + i*nStep];
					pIndexData[baseIndex + 2] = indexList[i+1];

					pIndexData[baseIndex + 3] = indexList[nCellAmountConnect+1 + i*nStep];
					pIndexData[baseIndex + 4] = indexList[nCellAmountConnect+1 + i*nStep+1];
					pIndexData[baseIndex + 5] = indexList[i+1];

					baseIndex += 6;
				}
				else
				{
					pIndexData[baseIndex] = indexList[nCellAmountConnect+1 + i*nStep + j];
					pIndexData[baseIndex + 1] = indexList[nCellAmountConnect+1 + i*nStep + j + 1];
					pIndexData[baseIndex + 2] = indexList[i+1];
					baseIndex += 3;
				}
			}
		}

		pSkirtIB = GetRenderSystem()->CreateIndexBuffer(&pIndexData[0],pIndexData.size() * 2, 2);

	}

	void Terrain::BuildVertexDeclaration()
	{
		VertexElement element[6];
		element[0] = VertexElement(0,0,DT_FLOAT3,DU_POSITION,0);
		element[1] = VertexElement(0,12,DT_FLOAT2,DU_TEXCOORD,0);
		element[2] = VertexElement(0,20,DT_FLOAT3,DU_NORMAL,0);
		element[3] = VertexElement(0,32,DT_FLOAT3,DU_TANGENT,0);
		element[4] = VertexElement(0,44,DT_COLOR,DU_COLOR,0);
		element[5] = VertexElement(0,48,DT_COLOR,DU_COLOR,1);
		m_pVertexDecl = GetRenderSystem()->CreateVertexDeclaration(element,6);
	}

	void Terrain::BuildShareIB()
	{
		m_shareIB.resize(m_uNumLods);

		for (UINT m = 0; m < m_uNumLods;++m)
		{
			BuildBodyIndexBuffer(m);

			BuildSkirtIndexBuffer(m);
		}
	}

	void Terrain::BuildSkirtIndexBuffer(UINT m)
	{
		for (UINT n = 0; n < m_uNumLods; ++n)
		{
			if (m >= n)
			{
				continue;
			}

			int nCellAmountSelf = m_nTrunkSize >> m;
			int nCellAmountConnect = m_nTrunkSize >> n;

			SkirtIB& shaerIB = m_shareIB[m].m_vecSkitIB[n];

			BuildSkirtSideIB(shaerIB.SkitIB[North],nCellAmountSelf,nCellAmountConnect, nCellAmountSelf * (nCellAmountSelf +1),-nCellAmountSelf-1);

			BuildSkirtSideIB(shaerIB.SkitIB[South],nCellAmountSelf,nCellAmountConnect,nCellAmountSelf,nCellAmountSelf+1);

			BuildSkirtSideIB(shaerIB.SkitIB[West],nCellAmountSelf,nCellAmountConnect,0,1);

			BuildSkirtSideIB(shaerIB.SkitIB[East],nCellAmountSelf,nCellAmountConnect,(nCellAmountSelf + 1) * (nCellAmountSelf+1) - 1,-1);
		}   
	}


	void Terrain::BuildBodyIndexBuffer(UINT m)
	{
		UINT nXCellsAmount = m_nTrunkSize >> m;
		UINT nYCellsAmount = m_nTrunkSize >> m;

		// ib
		UINT nIndexCount = nXCellsAmount * nYCellsAmount * 2 * 3;
		vector<uint16>&  pIndexData = m_shareIB[m].m_vecBodyIBTemp;
		pIndexData.resize(nIndexCount);

		UINT baseIndex = 0;

		// loop through and compute the triangles of each quad
		for(UINT i = 0;i< nXCellsAmount;i++)
		{
			for(UINT j = 0;j< nYCellsAmount;j++)
			{
				pIndexData[baseIndex]     = i   * (nYCellsAmount+1) + j;
				pIndexData[baseIndex + 1] = (i) * (nYCellsAmount+1) + j+1;
				pIndexData[baseIndex + 2] =  (i+1)   * (nYCellsAmount+1) + j;

				pIndexData[baseIndex + 3] = (i+1) * (nYCellsAmount+1) + j;
				pIndexData[baseIndex + 4] = (i) * (nYCellsAmount+1) + j + 1;
				pIndexData[baseIndex + 5] = (i+1)   * (nYCellsAmount+1) + j + 1;

				// next quad
				baseIndex += 6;
			}
		}
		
		m_shareIB[m].m_pBodyIB = GetRenderSystem()->CreateIndexBuffer(&pIndexData[0],pIndexData.size() * 2, 2);
	}

	const char* Terrain::GetHeightMap() const
	{
		return m_strHeightMap.c_str();
	}

	void Terrain::SetHeightMap(const char* pszFile)
	{
		m_strHeightMap = pszFile ? pszFile : "";

		m_pHeightResource = CreateResource(pszFile);
	}

	const char* Terrain::GetMaterialMap() const
	{
		return m_strMaterialMap.c_str();
	}

	void Terrain::SetMaterialMap(const char* pszFile)
	{
		m_strMaterialMap = pszFile ? pszFile : "";

		m_pMaterialResource = CreateResource(pszFile);
	}

	const char* Terrain::GetMaterialPath() const
	{
		return m_pMaterialData ? m_pMaterialData->GetResPath() : NULL;
	}

	void Terrain::SetMaterialPath(const char* pPath)
	{
		m_pMaterialData = LoadResource<MaterialData>(pPath);
	}

	const char* Terrain::GetBlendMap() const
	{
		return m_strBlendMap.c_str();
	}

	void Terrain::SetBlendMap(const char* pszFile)
	{
		m_strBlendMap = pszFile ? pszFile : "";

		if (m_bBlendMapToVertex)
			m_pBlendResource = CreateResource(pszFile);
		else
			m_pBlendMap = CreateTexture(pszFile);
	}
	
	bool Terrain::OnLoadOver()
	{
		if (m_bLoadOver)
			return true;

		if (m_pMaterialData == NULL || !m_pMaterialData->OnLoadOver())
			return false;

		if (m_pHeightResource == NULL || !m_pHeightResource->OnLoadOver())
			return false;
		
		if (m_pMaterialResource && !m_pMaterialResource->OnLoadOver())
			return false;

		for (uint32 i = 0; i < m_pMaterialData->GetSubMatDataNumber(); ++i)
		{
			RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
			pSubMaterial->InitWithSubMatData(m_pMaterialData->GetSubMatDataByIndex(i));
			this->AddMaterial(pSubMaterial.get());
		}

		if (m_bBlendMapToVertex) 
		{
			if (m_pBlendResource == NULL || !m_pBlendResource->OnLoadOver())
				return false;
		}
		else
		{
			if (m_pBlendMap == NULL || !m_pBlendMap->OnLoadOver())
				return false;
		}

		if (m_pHeightResource)
		{
			ResourceMapToData<uint16>(m_pHeightResource.get(),m_vecHeight,m_nXCellsAmount,m_nYCellsAmount);
			m_pHeightResource = NULL;
		}

		if (m_pMaterialResource)
		{
			int matW, matH;
			ResourceMapToData<uint8>(m_pMaterialResource.get(),m_vecVertexMatID,matW,matH);
			m_pMaterialResource = NULL;
		}

		if (m_pBlendResource)
		{
			int blendW,blenH;
			ResourceMapToData<uint32>(m_pBlendResource.get(),m_vecBlenData,blendW,blenH);
			m_nBlendingMultiple = blendW / m_nXCellsAmount;
			m_pBlendResource = NULL;
		}

		if (m_pBlendMap)
		{
			m_nBlendingMultiple = m_pBlendMap->GetWidth() / m_nXCellsAmount;
			m_vBlendOffset = Vector2(0.5f / m_pBlendMap->GetWidth(), 0.5f / m_pBlendMap->GetHeight());
		}

		BuildVertexDeclaration();

		BuildShareIB();

		BuildTrunks();

		ClearTempData();
		
		m_bLoadOver = true;

		return true;
	}	

	void Terrain::OnTransformChange()
	{
		for (UINT i = 0; i < m_vecTrunk.size(); ++i)
		{
			m_vecTrunk[i]->OnTransformChange();
		}
	}


	float Terrain::GetHeight(int nXVert, int nYVert) const
	{
		return m_vStartpoint.y + m_vecHeight[this->CalcIndex(nXVert, nYVert)] * m_fHeightSpacing / 65535.0f;
	}

	float Terrain::GetHeight( float fX, float fZ ) const
	{
		fX = (fX - m_vStartpoint.x) / m_fCellSpacing;
		fZ = (fZ - m_vStartpoint.z) / m_fCellSpacing;
		int i = (int)fX;
		int j = (int)fZ;
		if (i < 0 || i >= (int)m_nXCellsAmount || j < 0 || j >= (int)m_nYCellsAmount)
		{
			i = i < 0 ? 0 : i;
			i = i >= m_nXCellsAmount ? m_nXCellsAmount : i;
			j = j < 0 ? 0 : j;
			j = j >= m_nYCellsAmount ? m_nYCellsAmount : j;
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
		float v = fZ - (int)fZ;

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
		nWidth = imageData.width;
		nHeight = imageData.height; 
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

			if (vPosition.x > m_vStartpoint.x + m_nXCellsAmount * m_fCellSpacing)
			{
				if (rayStep.x >= 0)
					return false;
			}

			if (vPosition.z < m_vStartpoint.z)
			{
				if (rayStep.z <= 0)
					return false;
			}

			if (vPosition.z > m_vStartpoint.z + m_nYCellsAmount * m_fCellSpacing)
			{
				if (rayStep.z >= 0)
					return false;
			}

			height = this->GetHeight(vPosition.x, vPosition.z); 

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

	bool Terrain::WorldToCell( float fX, float fZ, OUT int& i, OUT int& j ) const
	{
		i = (int)::floorf((fX - m_vStartpoint.x)/m_fCellSpacing);
		j = (int)::floorf((fZ - m_vStartpoint.z)/m_fCellSpacing);

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
		vPos.z = m_vStartpoint.z + nYVert * m_fCellSpacing;
		vPos.y = GetHeight(nXVert,nYVert);	

		return vPos;
	}

	Vector3 Terrain::GetNormal(int nXVert, int nYVert) const
	{
		if (nXVert<= 0 || nXVert>= m_nXCellsAmount)
		{
			return Vector3::UNIT_Z;
		}
		if (nYVert<= 0 || nYVert>= m_nYCellsAmount)
		{
			return Vector3::UNIT_Z;
		}

		float fHeight1 = this->GetHeight(nXVert-1, nYVert);
		float fHeight2 = this->GetHeight(nXVert, nYVert-1);
		float fHeight3 = this->GetHeight(nXVert+1, nYVert);
		float fHeight4 = this->GetHeight(nXVert, nYVert+1);
		Vector3 vNormal(fHeight1-fHeight3, fHeight2-fHeight4, 2*m_fCellSpacing);
		vNormal.normalise();
		return vNormal;
	}

	Vector3 Terrain::GetNormal(float x, float z) const
	{
		float flip = 1.f;
		float fStep = m_fCellSpacing*0.2f;
		Vector3 here(x, this->GetHeight(x, z), z);
		Vector3 right(x + fStep, this->GetHeight(x + fStep, z), z);
		Vector3 up(x, this->GetHeight(x, z + fStep), z + fStep);
		if (right.x >= m_vStartpoint.x + m_fCellSpacing * m_nXCellsAmount)
		{
			flip *= -1;
			right = Vector3(x-fStep, this->GetHeight(x - fStep, z), z);
		}
		if (up.y >= m_vStartpoint.y + m_fCellSpacing * m_nYCellsAmount)
		{
			flip *= -1;
			up = Vector3(x, this->GetHeight(x, z - fStep), z - fStep);
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

	Vector3 Terrain::GetTangent(float x, float z) const
	{
		float flip = 1;
		float fStep = m_fCellSpacing * 0.5f;
		Vector3 here(x, this->GetHeight(x, z), z);
		Vector3 right(x + fStep, this->GetHeight(x + fStep, z), z);
		if (right.x >= m_vStartpoint.x + m_fCellSpacing * m_nXCellsAmount)
		{
			flip *= -1;
			right = Vector3(x - fStep, this->GetHeight(x - fStep, z), z);
		}

		right -= here;
		Vector3 vReturn = right * flip;
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
		ASSERT(matID < m_vecMaterial.size());
		if (matID >= m_vecMaterial.size())
			return NULL;

		return m_vecMaterial[matID].get();
	}

	UINT Terrain::GetMaterialCount() const
	{
		return m_vecMaterial.size();
	}

	void Terrain::AddMaterial(SubMaterial* pMateral)
	{
		m_vecMaterial.push_back(pMateral);
	}

	SubMaterial* Terrain::GetMaterial(int nXVert,int nYVert) const
	{
		return GetMaterialByID( GetMaterialID(nXVert,nYVert) );
	}

	// Blend
	UINT Terrain::GetBlendData(int nXVert, int nYVert) const
	{
		int nX = nXVert * m_nBlendingMultiple;
		int nY = nYVert * m_nBlendingMultiple;

		uint32 nIndex = nX + nY * (m_pBlendMap->GetWidth() + 1);
		ASSERT(nIndex >= 0 && nIndex < m_vecBlenData.size());
		if (nIndex < 0 || nIndex >= m_vecBlenData.size())
			return 0;

		return m_vecBlenData[nIndex];	
	}

	void Terrain::GetVertexData(int nXVert,int nYVert,TERRAIN_VERTEX& v) const
	{
		v.pos = GetPos(nXVert,nYVert);
		v.uv.x = (float)nXVert / (float)m_nXCellsAmount;
		v.uv.y = (float)nYVert / (float)m_nYCellsAmount;
		v.normal = GetNormal(nXVert, nYVert);
		v.tan = GetTangent(nXVert, nYVert);

		ColourValue color;
		color.a = (float)GetMaterialID(nXVert,nYVert) / 255.0f;
		v.color = color.getAsARGB();

		if (m_bBlendMapToVertex)
		{
			v.blend = GetBlendData(nXVert,nYVert);
		}
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
		pTerrian->OnLoadOver();
		return pTerrian;
	}
}
