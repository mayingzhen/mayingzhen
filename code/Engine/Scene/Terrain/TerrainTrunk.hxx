#include "TerrainTrunk.h"
#include "TerrainRenderable.h"

namespace ma
{
	TerrainTrunk::TerrainTrunk(Terrain* pTerrain)
	{
		m_nX = 0;
		m_nY = 0;
		m_uLodIndex = 0;
		m_pTerrain = pTerrain;
		
		m_pSceneNode = pTerrain;
	}

	TerrainTrunk::~TerrainTrunk()
	{
	}

	void TerrainTrunk::UpdateRenderable()
	{
		ASSERT(m_uLodIndex >= 0 && m_uLodIndex < m_vecLodRenderable.size());
		if (m_uLodIndex < 0 || m_uLodIndex >= m_vecLodRenderable.size())
			return;

		TERRAIN_LOD& lod = m_vecLodRenderable[m_uLodIndex];

		for (uint32 i = 0; i < lod.m_vecBody.size(); ++i)
		{
			m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_TerrainBody, lod.m_vecBody[i].get());
		}

		for (uint32 i = 0; i < lod.m_vecBorder.size(); ++i)
		{
			m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_TerrainBorder, lod.m_vecBorder[i].get());
		}


		int nTrunkXCellAmont = m_pTerrain->GetXCellAmount() / m_pTerrain->GetTrunkSize();
		int nTrunkYCellAmont = m_pTerrain->GetYCellAmount() / m_pTerrain->GetTrunkSize();
		if (m_nX > 0)
		{
			uint32 west = m_pTerrain->GetTerrainTrunkByIndex(m_nX-1, m_nY)->GetLodIndex();
			if (m_uLodIndex < west)
			{
				m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_TerrainSkirt, lod.m_vecSkirt[west].skirt[West].get());
			}
		}

		if (m_nX < nTrunkXCellAmont-1)
		{
			uint32 east = m_pTerrain->GetTerrainTrunkByIndex(m_nX+1, m_nY)->GetLodIndex();
			if (m_uLodIndex < east)
			{
				m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_TerrainSkirt, lod.m_vecSkirt[east].skirt[East].get());
			}
		}

		if (m_nY > 0)
		{
			uint32 north = m_pTerrain->GetTerrainTrunkByIndex(m_nX, m_nY-1)->GetLodIndex();
			if (m_uLodIndex < north)
			{
				m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_TerrainSkirt, lod.m_vecSkirt[north].skirt[North].get());
			}
		}

		if (m_nY < nTrunkYCellAmont -1)
		{
			uint32 south = m_pTerrain->GetTerrainTrunkByIndex(m_nX, m_nY+1)->GetLodIndex();
			if (m_uLodIndex < south)
			{
				m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_TerrainSkirt, lod.m_vecSkirt[south].skirt[South].get());
			}
		}
	}

	void TerrainTrunk::Show(Camera* pCamera)
	{
		ASSERT(m_pTerrain->GetNumLod() >= 1);

		if (m_pTerrain->GetNumLod() > 1)
		{
			float fDistance = (this->GetAABBWS().getCenter() - pCamera->GetPos()).length();
			
			int nNumLodPow2 = Math::PowInt(2, m_pTerrain->GetNumLod() - 1);
			float fLodValue = Math::Max<float>( 1.0f, Math::Min<float>(fDistance / m_fLodParam, (float)nNumLodPow2) );

			m_uLodIndex = (UINT)Math::Log2(fLodValue);

			m_uLodIndex = Math::Clamp<UINT>(m_uLodIndex, 0, m_pTerrain->GetNumLod() - 1);
		}
		else
		{
			m_uLodIndex = 0;
		}

		UpdateRenderable();
	}

	bool TerrainTrunk::Init(int i, int j)
	{
		m_nX = i;
		m_nY = j;

		for (int m = i * m_pTerrain->GetTrunkSize(); m <= (i + 1) * m_pTerrain->GetTrunkSize(); ++m)
		{
			for (int n = j * m_pTerrain->GetTrunkSize(); n <= (j + 1) * m_pTerrain->GetTrunkSize(); ++n)
			{
				m_AABB.merge(m_pTerrain->GetPos(m, n));
			}
		}

		//OnTransformChange();
		MarkDirty();

		m_vecLodRenderable.resize(m_pTerrain->GetNumLod());
		m_vecVBDataTemp.resize(m_pTerrain->GetNumLod());
		m_vecVBTemp.resize(m_pTerrain->GetNumLod());
		m_vecSetMatIDTemp.resize(m_pTerrain->GetNumLod());
		for (uint32 m = 0; m < m_pTerrain->GetNumLod(); ++m)
		{
			m_vecLodRenderable[m].m_vecSkirt.resize(m_pTerrain->GetNumLod());
		
			int nXVertsAmount = (m_pTerrain->GetTrunkSize() >> m) + 1;
			int nYVertsAmount = (m_pTerrain->GetTrunkSize() >> m) + 1;
			int nVertexCount = nXVertsAmount * nYVertsAmount;

			m_vecVBDataTemp[m].resize(nVertexCount);
		}
		
		BuildLod();	

		return true;
	}

	void TerrainTrunk::BuildVertexBuffer()
	{
		m_vecSetMatIDTemp.clear();
		m_vecSetMatIDTemp.resize(m_pTerrain->GetNumLod());

		for (uint32 m = 0; m < m_pTerrain->GetNumLod(); ++m)
		{
			TERRAIN_LOD& lod = m_vecLodRenderable[m];

			int nXCellsAmount = m_pTerrain->GetTrunkSize() >> m;
			int nYCellsAmount = m_pTerrain->GetTrunkSize() >> m;
			int nXVertsAmount = nXCellsAmount + 1;
			int nYVertsAmount = nYCellsAmount + 1;

			// vb
			int nStep = 1 << m;
			int nIndex = 0;

			for (int i = 0;i< nXVertsAmount;++i)
			{
				for (int j = 0;j< nYVertsAmount;++j)
				{
					int nXVert = m_pTerrain->GetTrunkSize() *  m_nX  + i * nStep;
					int nYVert = m_pTerrain->GetTrunkSize() *  m_nY  + j * nStep;

					TERRAIN_VERTEX vertex;
					m_pTerrain->GetVertexData(nXVert,nYVert,vertex);

					m_vecVBDataTemp[m][nIndex] = vertex;
					
					ColourValue color0;
					color0.setAsARGB(vertex.color);
					uint8 matID0 = (uint8)(color0.a * 255.0f);
					m_vecSetMatIDTemp[m].insert(matID0);

					nIndex += 1;
				}
			}

			m_vecVBTemp[m] = GetRenderSystem()->CreateVertexBuffer(
				(uint8*)&(m_vecVBDataTemp[m][0]),m_vecVBDataTemp[m].size() * sizeof(TERRAIN_VERTEX),sizeof(TERRAIN_VERTEX));
		}
	}


	void TerrainTrunk::BuildBody()
	{
		m_vecTriangleMatID.clear();
		m_vecTriangleMatID.resize(m_pTerrain->GetNumLod());

		for (uint32 m = 0; m < m_pTerrain->GetNumLod();++m)
		{
			TERRAIN_LOD& lod = m_vecLodRenderable[m];
			Terrain::LodShareIB& lodIB = m_pTerrain->m_shareIB[m];

			m_vecTriangleMatID[m].resize(lodIB.m_vecBodyIBTemp.size() / 3, -1);	

			lod.m_vecBody.clear();

			set<uint8>& setMatIDTemp = m_vecSetMatIDTemp[m];

			if (setMatIDTemp.size() == 1)
			{	
				uint8 iMatID = *setMatIDTemp.begin();

				if (iMatID == (uint8)-1)
					continue;

				RefPtr<IndexBuffer> pIB = lodIB.m_pBodyIB;
				RefPtr<VertexBuffer>& vb = m_vecVBTemp[m];

				TerrainRenderable* pRenderable = new TerrainRenderable(this);
				pRenderable->m_pDeclaration = m_pTerrain->GetVertexDeclaration();
				pRenderable->m_pVertexBuffers = m_vecVBTemp[m];
				pRenderable->m_pIndexBuffer = lodIB.m_pBodyIB;	
				pRenderable->m_pSubMaterial = m_pTerrain->GetMaterialByID(iMatID);
				pRenderable->m_fMateriID = (float)iMatID;
				lod.m_vecBody.push_back(pRenderable);
			}
			else
			{
				for (set<uint8>::iterator it = setMatIDTemp.begin(); it != setMatIDTemp.end(); ++it)
				{
					uint8 iMatID = *it;
					if (iMatID == (uint8)-1)
						continue;

					vector<uint16> bodyIBList;

					uint16* pIBData = &lodIB.m_vecBodyIBTemp[0]; // GLES ²»ÄÜLOCK CHardwareBuffer
					TERRAIN_VERTEX* pVBData = &m_vecVBDataTemp[m][0];

					for (UINT i = 0; i < lodIB.m_vecBodyIBTemp.size(); i += 3)
					{
						uint16 triangle[3] = { pIBData[i+0], pIBData[i+1], pIBData[i+2] };

						ColourValue color0,color1,color2;
						color0.setAsARGB(pVBData[ triangle[0] ].color);
						color1.setAsARGB(pVBData[ triangle[1] ].color);
						color2.setAsARGB(pVBData[ triangle[2] ].color);

						uint8 matID0 = (uint8)(color0.a * 255.0f);
						uint8 matID1 = (uint8)(color1.a * 255.0f);
						uint8 matID2 = (uint8)(color2.a * 255.0f);

						if ( (matID0 == iMatID || matID1 == iMatID || matID2 == iMatID) && m_vecTriangleMatID[m][i / 3] == (uint8)-1)	
						{
							m_vecTriangleMatID[m][i / 3] = iMatID;

							bodyIBList.push_back( triangle[0] );	
							bodyIBList.push_back( triangle[1] );	
							bodyIBList.push_back( triangle[2] );	
						}
					}

					if (bodyIBList.empty())
						continue;

					RefPtr<IndexBuffer> pIB = GetRenderSystem()->CreateIndexBuffer(
						(uint8*)&bodyIBList[0], sizeof(uint16) * bodyIBList.size(), sizeof(uint16));

					TerrainRenderable* pRenderable = new TerrainRenderable(this);
					pRenderable->m_pDeclaration = m_pTerrain->GetVertexDeclaration();
					pRenderable->m_pVertexBuffers = m_vecVBTemp[m];
					pRenderable->m_pIndexBuffer = pIB;
					pRenderable->m_pSubMaterial = m_pTerrain->GetMaterialByID(iMatID);
					pRenderable->m_fMateriID = (float)iMatID;
					lod.m_vecBody.push_back(pRenderable);
				}
			}
		}
	}

	void TerrainTrunk::BuildBorder()
	{
		if (m_pTerrain->GetMaterialCount() <= 1)
			return;

		for (uint32 m = 0; m < m_pTerrain->GetNumLod(); ++m)
		{
			TERRAIN_LOD& lod = m_vecLodRenderable[m];
			Terrain::LodShareIB& lodIB = m_pTerrain->m_shareIB[m];

			vector< vector<uint16> > borderIBList; 

			vector<uint16>& pIBData = lodIB.m_vecBodyIBTemp; 
			vector<TERRAIN_VERTEX>& pVBData = m_vecVBDataTemp[m];

			for (UINT i = 0; i < pIBData.size(); i += 3)
			{
				uint16 triangle[3] = { pIBData[i+0], pIBData[i+1], pIBData[i+2] };

				ColourValue color0,color1,color2;
				color0.setAsARGB(pVBData[ triangle[0] ].color);
				color1.setAsARGB(pVBData[ triangle[1] ].color);
				color2.setAsARGB(pVBData[ triangle[2] ].color);

				uint8 matID0 = (uint8)(color0.a * 255.0f);
				uint8 matID1 = (uint8)(color1.a * 255.0f);
				uint8 matID2 = (uint8)(color2.a * 255.0f);

				if (matID2 == matID0 && matID2 == matID1)
					continue;

				uint8 nTriangleMatID = m_vecTriangleMatID[m][i / 3];

				if (matID0 != nTriangleMatID)
				{
					AddToIBList(borderIBList,matID0,triangle);
				}
				
				if (matID1 != matID0 && matID1 != nTriangleMatID)
				{
					AddToIBList(borderIBList,matID1,triangle);
				}

				if (matID2 != matID0 && matID2 != matID1 && matID2 != nTriangleMatID)
				{
					AddToIBList(borderIBList,matID2,triangle);
				}
			}

			map< uint8, RefPtr<IndexBuffer> > matAddIdToIB;
			for (uint32 i = 0; i < borderIBList.size(); ++i)
			{
				if (borderIBList[i].empty())
					continue;

				vector<uint16>& indexList  = borderIBList[i];

				RefPtr<IndexBuffer> pIB = GetRenderSystem()->CreateIndexBuffer(
					(uint8*)&indexList[0],sizeof(uint16) * indexList.size(),sizeof(uint16));

				matAddIdToIB[i] = pIB;
			}

			lod.m_vecBorder.clear();

			map< uint8, RefPtr<IndexBuffer> >::iterator it = matAddIdToIB.begin();
			for (; it != matAddIdToIB.end(); ++it)
			{
				RefPtr<SubMaterial> pBorderMaterial = m_pTerrain->GetMaterialByID(it->first)->Clone();
				Technique* pTech = pBorderMaterial->GetShadingTechnqiue();
				pTech->GetRenderState().SetBlendMode(BM_TRANSPARENT);
				pTech->GetRenderState().SetDepthCheckMode(DCM_EQUAL);
				pTech->AddShaderMarco("BOREDER");

				TerrainRenderable* pRenderable = new TerrainRenderable(this);
				pRenderable->m_pDeclaration = m_pTerrain->GetVertexDeclaration();
				pRenderable->m_pVertexBuffers = m_vecVBTemp[m];
				pRenderable->m_pIndexBuffer = it->second;
				pRenderable->m_pSubMaterial = pBorderMaterial;//m_pTerrain->GetMaterialByID(it->first);
				pRenderable->m_fMateriID = (float)it->first;
				pRenderable->m_bBorder = true;
				lod.m_vecBorder.push_back(pRenderable);
			}
		}
	}

	void TerrainTrunk::BuildSkirt()
	{
		if (m_pTerrain->GetNumLod() <= 1)
			return;

		for (uint32 m = 0; m < m_pTerrain->GetNumLod(); ++m)
		{
			TERRAIN_LOD& lod = m_vecLodRenderable[m];
			Terrain::LodShareIB& lodIB = m_pTerrain->m_shareIB[m];

			for (uint32 n = 0; n < m_pTerrain->GetNumLod(); ++n)
			{
				if (m >= n)
				{
					continue;
				}

				if (lod.m_vecBody.empty())
					continue;

				SkitRenderable& skirt = lod.m_vecSkirt[n];
				Terrain::SkirtIB& skirtIB = lodIB.m_vecSkitIB[n];

				RefPtr<SubMaterial> pSkirtMaterial = lod.m_vecBody[0]->GetMaterial()->Clone();
				pSkirtMaterial->GetShadingTechnqiue()->GetRenderState().SetCullMode(CULL_FACE_SIDE_NONE);

				for (UINT i = 0; i < SideNum; ++i)
				{
					TerrainRenderable* pRenderable = new TerrainRenderable(this);
					pRenderable->m_pDeclaration = m_pTerrain->GetVertexDeclaration();
					pRenderable->m_pVertexBuffers = m_vecVBTemp[m];
					pRenderable->m_pIndexBuffer = skirtIB.SkitIB[i];
					pRenderable->m_pSubMaterial = pSkirtMaterial.get();
					skirt.skirt[i] = pRenderable;
				}
			}
		}
	}

	void TerrainTrunk::BuildLod()
	{
		BuildVertexBuffer();

		BuildBody();

		BuildBorder();

		BuildSkirt();
	}

	void TerrainTrunk::AddToIBList( vector< vector<uint16> >& vecIBList, uint8 matID, uint16 triangle[3])
	{
		if (matID == (uint8)-1)
			return;

		if (vecIBList.size() <= matID)
		{
			vecIBList.resize(matID + 1);
		}

		vecIBList[matID].push_back(triangle[0]);
		vecIBList[matID].push_back(triangle[1]);
		vecIBList[matID].push_back(triangle[2]);
	}

	void TerrainTrunk::ClearVBTemp()
	{
		m_vecVBTemp.clear();
		m_vecVBDataTemp.clear();
	}


	void TerrainTrunk::SetLodParamDiv(float f)
	{
		const float Terrain_CellSize = m_pTerrain->GetCellSpacing();
		const float Terrain_TrunkXSize = m_pTerrain->GetTrunkSize() * Terrain_CellSize;
		const float Terrain_TrunkYSize = m_pTerrain->GetTrunkSize() * Terrain_CellSize;

		m_fLodParam = Math::Sqrt(Terrain_TrunkXSize * Terrain_TrunkXSize + Terrain_TrunkYSize * Terrain_TrunkYSize) / f;
	}
}
