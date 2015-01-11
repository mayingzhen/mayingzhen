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
	}

	TerrainTrunk::~TerrainTrunk()
	{
	}

	void TerrainTrunk::Show(Camera* pCamera)
	{
		ASSERT(m_pTerrain->GetNumLod() >= 1);

		if (m_pTerrain->GetNumLod() > 1)
		{
			float fDistance = (this->GetAABBWS().getCenter() - pCamera->GetSceneNode()->GetPos()).length();
			m_uLodIndex = (UINT)Math::Log2(__max(1, min(fDistance/m_fLodParam, Math::PowInt(2, (m_pTerrain->GetNumLod()-1)))));

			m_uLodIndex = Math::Clamp<UINT>(m_uLodIndex, 0, m_pTerrain->GetNumLod() - 1);
		}
		else
		{
			m_uLodIndex = 0;
		}

		ASSERT(m_uLodIndex >= 0 && m_uLodIndex < m_vecLod.size());
		if (m_uLodIndex < 0 || m_uLodIndex >= m_vecLod.size())
			return;

		TERRAIN_LOD& lod = m_vecLod[m_uLodIndex];

		for (uint32 i = 0; i < lod.m_vecBody.size(); ++i)
		{
			GetRenderSystem()->GetRenderQueue()->AddRenderObj(RL_Solid, lod.m_vecBody[i].get() );
			//GetRenderQueueManager()->AddRenderable( lod.m_vecBody[i].get() );
		}

		for (uint32 i = 0; i < lod.m_vecBorder.size(); ++i)
		{
			GetRenderSystem()->GetRenderQueue()->AddRenderObj(RL_Solid, lod.m_vecBorder[i].get() );
			//GetRenderQueueManager()->AddRenderable(  lod.m_vecBorder[i].get() );
		}


		int nTrunkXCellAmont = m_pTerrain->GetXCellAmount() / m_pTerrain->GetTrunkSize();
		int nTrunkYCellAmont = m_pTerrain->GetYCellAmount() / m_pTerrain->GetTrunkSize();
		if (m_nX > 0)
		{
			uint32 west = m_pTerrain->GetTerrainTrunkByIndex(m_nX-1, m_nY)->GetLodIndex();
			if (m_uLodIndex < west)
			{
				GetRenderSystem()->GetRenderQueue()->AddRenderObj(RL_Solid, lod.m_vecSkirt[west].skirt[West].get() );
				//GetRenderQueueManager()->AddRenderable(lod.m_vecSkit[west].west.get());
			}
		}

		if (m_nX < nTrunkXCellAmont-1)
		{
			uint32 east = m_pTerrain->GetTerrainTrunkByIndex(m_nX+1, m_nY)->GetLodIndex();
			if (m_uLodIndex < east)
			{
				GetRenderSystem()->GetRenderQueue()->AddRenderObj(RL_Solid, lod.m_vecSkirt[east].skirt[East].get() );
				//GetRenderQueueManager()->AddRenderable(lod.m_vecSkit[east].east.get());
			}
		}

		if (m_nY > 0)
		{
			uint32 north = m_pTerrain->GetTerrainTrunkByIndex(m_nX, m_nY-1)->GetLodIndex();
			if (m_uLodIndex < north)
			{
				GetRenderSystem()->GetRenderQueue()->AddRenderObj(RL_Solid, lod.m_vecSkirt[north].skirt[North].get() );
				//GetRenderQueueManager()->AddRenderable(lod.m_vecSkit[north].north.get());
			}
		}

		if (m_nY < nTrunkYCellAmont -1)
		{
			uint32 south = m_pTerrain->GetTerrainTrunkByIndex(m_nX, m_nY+1)->GetLodIndex();
			if (m_uLodIndex < south)
			{
				GetRenderSystem()->GetRenderQueue()->AddRenderObj( RL_Solid,lod.m_vecSkirt[south].skirt[South].get() );
				//GetRenderQueueManager()->AddRenderable(lod.m_vecSkit[south].south.get());
			}
		}
	}

	bool TerrainTrunk::Init(int i, int j)
	{
		m_nX = i;
		m_nY = j;

		AABB box;
		for (int m = i * m_pTerrain->GetTrunkSize(); m <= (i+1) * m_pTerrain->GetTrunkSize(); ++m)
		{
			for (int n = j * m_pTerrain->GetTrunkSize(); n <= (j + 1) * m_pTerrain->GetTrunkSize(); ++n)
			{
				box.merge(m_pTerrain->GetPos(m, n));
			}
		}
		m_AABB = box;

		OnTransformChange();

		m_vecLod.resize(m_pTerrain->GetNumLod());
		for (uint32 m = 0; m < m_pTerrain->GetNumLod(); ++m)
		{
			m_vecLod[m].m_vecSkirt.resize(m_pTerrain->GetNumLod());
		
			int nXVertsAmount = (m_pTerrain->GetTrunkSize() >> m) + 1;
			int nYVertsAmount = (m_pTerrain->GetTrunkSize() >> m) + 1;
			int nVertexCount = nXVertsAmount * nYVertsAmount;

			m_vecLod[m].m_vecVBTemp.resize(nVertexCount);
		}
		
		BuildLod();			

		return true;
	}

	void TerrainTrunk::BuildVertexBuffer()
	{
		for (uint32 m = 0; m < m_pTerrain->GetNumLod(); ++m)
		{
			vector<TERRAIN_VERTEX>& VBTemp = m_vecLod[m].m_vecVBTemp;

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

					m_pTerrain->GetVertexData(nXVert,nYVert,VBTemp[nIndex]);
	
					nIndex += 1;
				}
			}

			m_vecLod[m].m_BodyVB = GetRenderSystem()->CreateVertexBuffer(
				&VBTemp[0],VBTemp.size() * sizeof(TERRAIN_VERTEX),sizeof(TERRAIN_VERTEX));
		}
	}


	void TerrainTrunk::BuildBody()
	{
		for (uint32 m = 0; m < m_pTerrain->GetNumLod();++m)
		{
			TERRAIN_LOD& lod = m_vecLod[m];
			Terrain::LodShareIB& lodIB = m_pTerrain->m_shareIB[m];

			map< uint8, RefPtr<IndexBuffer> > matIdToIB;

			vector< vector<uint16> > bodyIBList; 
			bool bHaveEmptyMaterial = false;

			vector<uint16>& pIBData = lodIB.m_vecBodyIBTemp; 
			vector<TERRAIN_VERTEX>& pVBData = lod.m_vecVBTemp;

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

				if (matID0 == (uint8)-1 || matID1 == (uint8)-1 || matID2 == (uint8)-1)
					bHaveEmptyMaterial = true;

				AddToIBList(bodyIBList,matID0,triangle);

				if (matID1 != matID0)
				{
					AddToIBList(bodyIBList,matID1,triangle);
				}

				if (matID2 != matID0 && matID2 != matID1)
				{
					AddToIBList(bodyIBList,matID2,triangle);
				}
			}

			int nMatCount = 0;	
			int nMatID;
			for (uint32 i = 0; i < bodyIBList.size(); ++i)
			{
				if (bodyIBList[i].empty())
					continue;

				nMatCount++;
				nMatID = i;
			}

			if (nMatCount ==1 && !bHaveEmptyMaterial)
			{
				matIdToIB[nMatID] = lodIB.m_pBodyIB;
			}
			else
			{
				for (uint32 i = 0; i < bodyIBList.size(); ++i)
				{
					if (bodyIBList[i].empty())
						continue;

					vector<uint16>& indexList  = bodyIBList[i];

					RefPtr<IndexBuffer> pIB = GetRenderSystem()->CreateIndexBuffer(
						&indexList[0], sizeof(uint16) * indexList.size(), sizeof(uint16),USAGE_STATIC);

					matIdToIB[i] = pIB;
				}
			}
	

			lod.m_vecBody.clear();

			map< uint8, RefPtr<IndexBuffer> >::iterator it = matIdToIB.begin();
			for (; it != matIdToIB.end(); ++it)
			{
				SubMaterial* pTerrainMat = m_pTerrain->GetMaterialByID(it->first);

				TerrainRenderable* pRenderable = new TerrainRenderable(this);
				pRenderable->m_pDeclaration = m_pTerrain->GetVertexDeclaration();
				pRenderable->m_pVertexBuffers = lod.m_BodyVB;
				pRenderable->m_pIndexBuffer = it->second;
				pRenderable->m_pMaterial = m_pTerrain->GetMaterialByID(it->first);
				pRenderable->m_fMateriID = (float)it->first;
				lod.m_vecBody.push_back(pRenderable);
			}
		}
	}

	void TerrainTrunk::BuildBorder()
	{
		if (m_pTerrain->GetMaterialCount() <= 1)
			return;

		for (uint32 m = 0; m < m_pTerrain->GetNumLod(); ++m)
		{
			TERRAIN_LOD& lod = m_vecLod[m];
			Terrain::LodShareIB& lodIB = m_pTerrain->m_shareIB[m];

			vector< vector<uint16> > borderIBList; 

			vector<uint16>& pIBData = lodIB.m_vecBodyIBTemp; 
			vector<TERRAIN_VERTEX>& pVBData = lod.m_vecVBTemp;

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

				AddToIBList(borderIBList,matID0,triangle);

				if (matID1 != matID0)
				{
					AddToIBList(borderIBList,matID1,triangle);
				}

				if (matID2 != matID0 && matID2 != matID1)
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
					&indexList[0],sizeof(uint16) * indexList.size(),sizeof(uint16),USAGE_STATIC);

				matAddIdToIB[i] = pIB;
			}

			lod.m_vecBorder.clear();

			map< uint8, RefPtr<IndexBuffer> >::iterator it = matAddIdToIB.begin();
			for (; it != matAddIdToIB.end(); ++it)
			{
				TerrainRenderable* pRenderable = new TerrainRenderable(this);
				pRenderable->m_pDeclaration = m_pTerrain->GetVertexDeclaration();
				pRenderable->m_pVertexBuffers = lod.m_BodyVB;
				pRenderable->m_pIndexBuffer = it->second;
				pRenderable->m_pMaterial = m_pTerrain->GetMaterialByID(it->first);
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
			TERRAIN_LOD& lod = m_vecLod[m];
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

				RefPtr<SubMaterial> pSkirtMaterial = m_vecLod[m].m_vecBody[0]->GetMaterial()->Clone();
				pSkirtMaterial->GetTechnqiueByIndex(0)->GetRenderState().m_eCullMode = CULL_FACE_SIDE_NONE;

				for (UINT i = 0; i < SideNum; ++i)
				{
					TerrainRenderable* pRenderable = new TerrainRenderable(this);
					pRenderable->m_pDeclaration = m_pTerrain->GetVertexDeclaration();
					pRenderable->m_pVertexBuffers = lod.m_BodyVB;
					pRenderable->m_pIndexBuffer = skirtIB.SkitIB[i];
					pRenderable->m_pMaterial = pSkirtMaterial.get();
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
		for (UINT i = 0; i < m_vecLod.size(); ++i)
		{
			m_vecLod[i].m_vecVBTemp.clear();
		}
	}


	void TerrainTrunk::SetLodParamDiv(float f)
	{
		const float Terrain_CellSize = m_pTerrain->GetCellSpacing();
		const float Terrain_TrunkXSize = m_pTerrain->GetTrunkSize() * Terrain_CellSize;
		const float Terrain_TrunkYSize = m_pTerrain->GetTrunkSize() * Terrain_CellSize;

		m_fLodParam = Math::Sqrt(Terrain_TrunkXSize * Terrain_TrunkXSize + Terrain_TrunkYSize * Terrain_TrunkYSize) / f;
	}
}
