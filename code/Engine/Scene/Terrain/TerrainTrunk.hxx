#include "TerrainTrunk.h"
#include "TerrainRenderable.h"
#include "Engine/Renderable/MeshData.h"

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
			m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Terrain, lod.m_vecBody[i].get());
		}

		for (uint32 i = 0; i < lod.m_vecBorder.size(); ++i)
		{
			m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Terrain, lod.m_vecBorder[i].get());
		}


		int nTrunkXCellAmont = m_pTerrain->GetXCellAmount() / m_pTerrain->GetTrunkSize();
		int nTrunkYCellAmont = m_pTerrain->GetYCellAmount() / m_pTerrain->GetTrunkSize();
		if (m_nX > 0)
		{
			uint32 west = m_pTerrain->GetTerrainTrunkByIndex(m_nX-1, m_nY)->GetLodIndex();
			if (m_uLodIndex < west)
			{
				m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Terrain, lod.m_vecSkirt[west].skirt[West].get());
			}
		}

		if (m_nX < nTrunkXCellAmont-1)
		{
			uint32 east = m_pTerrain->GetTerrainTrunkByIndex(m_nX+1, m_nY)->GetLodIndex();
			if (m_uLodIndex < east)
			{
				m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Terrain, lod.m_vecSkirt[east].skirt[East].get());
			}
		}

		if (m_nY > 0)
		{
			uint32 north = m_pTerrain->GetTerrainTrunkByIndex(m_nX, m_nY-1)->GetLodIndex();
			if (m_uLodIndex < north)
			{
				m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Terrain, lod.m_vecSkirt[north].skirt[North].get());
			}
		}

		if (m_nY < nTrunkYCellAmont -1)
		{
			uint32 south = m_pTerrain->GetTerrainTrunkByIndex(m_nX, m_nY+1)->GetLodIndex();
			if (m_uLodIndex < south)
			{
				m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Terrain, lod.m_vecSkirt[south].skirt[South].get());
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
		
		AABB aabb;
		for (int m = i * m_pTerrain->GetTrunkSize(); m <= (i + 1) * m_pTerrain->GetTrunkSize(); ++m)
		{
			for (int n = j * m_pTerrain->GetTrunkSize(); n <= (j + 1) * m_pTerrain->GetTrunkSize(); ++n)
			{
				aabb.merge( m_pTerrain->GetPos(m, n) );
				m_uvAABB.merge( m_pTerrain->GetUV(m,n) );
			}
		}

		SetAABB(aabb);

		m_vecLodRenderable.resize(m_pTerrain->GetNumLod());
		m_vecVBDataTemp.resize(m_pTerrain->GetNumLod());
		m_vecVBTemp.resize(m_pTerrain->GetNumLod());
		m_vecSetMatIDTemp.resize(m_pTerrain->GetNumLod());
		m_vecIBDataTemp.resize(m_pTerrain->GetNumLod());
		m_vecIBTemp.resize(m_pTerrain->GetNumLod());
		for (uint32 m = 0; m < m_pTerrain->GetNumLod(); ++m)
		{
			m_vecLodRenderable[m].m_vecSkirt.resize(m_pTerrain->GetNumLod());
		
			int nXCellsAmount = m_pTerrain->GetTrunkSize() >> m;
			int nYCellsAmount = m_pTerrain->GetTrunkSize() >> m;
			int nXVertsAmount = nXCellsAmount + 1;
			int nYVertsAmount = nYCellsAmount + 1;
			int nVertexCount = nXVertsAmount * nYVertsAmount;
			int nIndexCount = nXCellsAmount*nYCellsAmount*2*3;

			m_vecVBDataTemp[m].resize(nVertexCount);
			m_vecIBDataTemp[m].resize(nIndexCount);
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
		
					Vector3 vPos = m_pTerrain->GetPos(nXVert,nYVert);
					Vector2 vUV = m_pTerrain->GetUV(nXVert,nYVert);
					Vector3 vNormal = m_pTerrain->GetNormal(nXVert, nYVert);;
					Vector3 vTanget = m_pTerrain->GetTangent(nXVert, nYVert);
					uint8 nMateriID = m_pTerrain->GetMaterialID(nXVert,nYVert);

					vertex.pos = CompressPos(vPos,GetAABB().getCenter(),GetAABB().getHalfSize());
					vertex.uv = CompressUV(vUV,m_uvAABB.getCenter(),m_uvAABB.getHalfSize());
					vertex.tangent_quat = CompressQuaternion(vTanget,vNormal);
					vertex.pos.w = nMateriID;

					m_vecVBDataTemp[m][nIndex] = vertex;
					
					m_vecSetMatIDTemp[m].insert(nMateriID);

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
			m_vecTriangleMatID[m].resize(m_vecIBDataTemp[m].size() / 3, -1);	

			lod.m_vecBody.clear();

			set<uint8>& setMatIDTemp = m_vecSetMatIDTemp[m];

			if (setMatIDTemp.size() == 1)
			{	
				uint8 iMatID = *setMatIDTemp.begin();

				if (iMatID == (uint8)-1)
					continue;

				TerrainRenderable* pRenderable = new TerrainRenderable(this);
				//pRenderable->m_pDeclaration = m_pTerrain->GetVertexDeclaration();
				pRenderable->m_pVertexBuffer = m_vecVBTemp[m];
				pRenderable->m_pIndexBuffer = m_vecIBTemp[m];
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

					vector<uint16>& pIBData = m_vecIBDataTemp[m]; 
					vector<TERRAIN_VERTEX>& pVBData = m_vecVBDataTemp[m];

					for (UINT i = 0; i < pIBData.size(); i += 3)
					{
						uint16 triangle[3] = { pIBData[i+0], pIBData[i+1], pIBData[i+2] };

						TERRAIN_VERTEX& vertex0 = pVBData[ triangle[0] ];
						TERRAIN_VERTEX& vertex1 = pVBData[ triangle[1] ];
						TERRAIN_VERTEX& vertex2 = pVBData[ triangle[2] ];

						uint8 matID0 = (uint8)(vertex0.pos.w);
						uint8 matID1 = (uint8)(vertex1.pos.w);
						uint8 matID2 = (uint8)(vertex2.pos.w);

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
						(uint8*)&bodyIBList[0],sizeof(uint16) * bodyIBList.size(),sizeof(uint16));

					TerrainRenderable* pRenderable = new TerrainRenderable(this);
					//pRenderable->m_pDeclaration = m_pTerrain->GetVertexDeclaration();
					pRenderable->m_pVertexBuffer = m_vecVBTemp[m];
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
			
			lod.m_vecBorder.clear();

			if (m_vecSetMatIDTemp[m].size() <= 1)
				continue;

			vector< vector<uint16> > borderIBList; 

			vector<uint16> pIBData = m_vecIBDataTemp[m]; 
			vector<TERRAIN_VERTEX>& pVBData = m_vecVBDataTemp[m];

			for (UINT i = 0; i < pIBData.size(); i += 3)
			{
				uint16 triangle[3] = { pIBData[i+0], pIBData[i+1], pIBData[i+2] };

				TERRAIN_VERTEX& vertex0 = pVBData[ triangle[0] ];
				TERRAIN_VERTEX& vertex1 = pVBData[ triangle[1] ];
				TERRAIN_VERTEX& vertex2 = pVBData[ triangle[2] ];

				uint8 matID0 = (uint8)(vertex0.pos.w);
				uint8 matID1 = (uint8)(vertex1.pos.w);
				uint8 matID2 = (uint8)(vertex2.pos.w);

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

			map< uint8, RefPtr<IndexBuffer> >::iterator it = matAddIdToIB.begin();
			for (; it != matAddIdToIB.end(); ++it)
			{
				RefPtr<SubMaterial> pBorderMaterial = m_pTerrain->GetMaterialByID(it->first)->Clone();
				Technique* pTech = pBorderMaterial->GetShadingTechnqiue();
				
				RefPtr<BlendState> pBlendState = CreateBlendState();
				pBlendState->m_eBlendMode = BM_TRANSPARENT;
				pTech->SetBlendState(pBlendState.get());

				RefPtr<DepthStencilState> pDSState = CreateDepthStencilState();
				pDSState->m_bDepthWrite = false;
				pDSState->m_eDepthCheckMode = CMPF_EQUAL;
				pTech->SetDepthStencilState(pDSState.get());

				TerrainRenderable* pRenderable = new TerrainRenderable(this);
				//pRenderable->m_pDeclaration = m_pTerrain->GetVertexDeclaration();
				pRenderable->m_pVertexBuffer = m_vecVBTemp[m];
				pRenderable->m_pIndexBuffer = it->second;
				pRenderable->m_pSubMaterial = pBorderMaterial;
				pRenderable->m_fMateriID = (float)it->first;
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
	
			for (uint32 n = 0; n < m_pTerrain->GetNumLod(); ++n)
			{
				if (m >= n)
				{
					continue;
				}

				if (lod.m_vecBody.empty())
					continue;

				SkitRenderable& skirt = lod.m_vecSkirt[n];

				SkitIB& skitIB = m_vecSkirt[m][n];

				RefPtr<SubMaterial> pSkirtMaterial = lod.m_vecBody[0]->GetMaterial()->Clone();
				RefPtr<RasterizerState> pRSState = CreateRasterizerState();
				pRSState->m_eCullMode = CULL_FACE_SIDE_NONE;
				pSkirtMaterial->GetShadingTechnqiue()->SetRasterizerState(pRSState.get());

				for (UINT i = 0; i < SideNum; ++i)
				{
					TerrainRenderable* pRenderable = new TerrainRenderable(this);
					//pRenderable->m_pDeclaration = m_pTerrain->GetVertexDeclaration();
					pRenderable->m_pVertexBuffer = m_vecVBTemp[m];
					pRenderable->m_pIndexBuffer = skitIB.skirtIB[i];
					pRenderable->m_pSubMaterial = pSkirtMaterial.get();
					skirt.skirt[i] = pRenderable;
				}
			}
		}
	}

	void TerrainTrunk::BuildLod()
	{
		BuildIndexBuffer();

		BuildSkirtIndexBuffer();
		
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

	void TerrainTrunk::ClearTempData()
	{
		m_vecVBTemp.clear();
		m_vecVBDataTemp.clear();
	}

	void TerrainTrunk::BuildSkirtSideIB(RefPtr<IndexBuffer>& pSkirtIB,
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

		pSkirtIB = GetRenderSystem()->CreateIndexBuffer((uint8*)&pIndexData[0],pIndexData.size() * 2, 2);
	}

	void TerrainTrunk::BuildSkirtIndexBuffer()
	{
		if (m_pTerrain->GetNumLod() <= 1)
			return;

		m_vecSkirt.resize(m_pTerrain->GetNumLod());
		for (uint32 i = 0;i< m_pTerrain->GetNumLod();++i)
		{
			m_vecSkirt[i].resize(m_pTerrain->GetNumLod());
		}

		for (uint32 m = 0; m < m_pTerrain->GetNumLod(); ++m)
		{
			for (uint32 n = 0; n < m_pTerrain->GetNumLod(); ++n)
			{
				SkitIB& skirt = m_vecSkirt[m][n];
				if (m >= n)
				{
					continue;
				}

				int nCellAmountSelf = m_pTerrain->GetXCellAmount() >> m; 
				int nCellAmountConnect = m_pTerrain->GetYCellAmount() >> n;

				BuildSkirtSideIB(skirt.skirtIB[North],nCellAmountSelf,nCellAmountConnect, nCellAmountSelf * (nCellAmountSelf +1),-nCellAmountSelf-1);

				BuildSkirtSideIB(skirt.skirtIB[South],nCellAmountSelf,nCellAmountConnect,nCellAmountSelf,nCellAmountSelf+1);

				BuildSkirtSideIB(skirt.skirtIB[West],nCellAmountSelf,nCellAmountConnect,0,1);

				BuildSkirtSideIB(skirt.skirtIB[East],nCellAmountSelf,nCellAmountConnect,(nCellAmountSelf + 1) * (nCellAmountSelf+1) - 1,-1);
			}   
		}
	}


	void TerrainTrunk::BuildIndexBuffer()
	{
		for (uint32 m = 0; m < m_pTerrain->GetNumLod(); ++m)
		{
			int nXCellsAmount = m_pTerrain->GetTrunkSize() >> m;
			int nYCellsAmount = m_pTerrain->GetTrunkSize() >> m;
			int nXVertsAmount = nXCellsAmount + 1;
			int nYVertsAmount = nYCellsAmount + 1;

			// ib
			vector<uint16>& pIndexData = m_vecIBDataTemp[m];

			int baseIndex = 0;

			int nStep = 1 << m;
			int nIndex = 0;

			for(int i = 0;i< nXCellsAmount;i++)
			{
				for(int j = 0;j< nYCellsAmount;j++)
				{
					int nXVert = m_pTerrain->GetXCellAmount() *  m_nX  + i * nStep;
					int nYVert = m_pTerrain->GetYCellAmount() *  m_nY  + j * nStep;

					uint16 iIndexTopLeft = i  * (nYCellsAmount+1) + j;			    
					uint16 iIndexTopRight = (i+1)   * (nYCellsAmount+1) + j;	    
					uint16 iIndexBottomLeft = (i) * (nYCellsAmount+1) + j+1;			
					uint16 iIndexBottomRight = (i+1)   * (nYCellsAmount+1) + j + 1; 
					uint16 iRect[4] = {iIndexTopLeft,iIndexTopRight,iIndexBottomLeft,iIndexBottomRight};

					if (m_pTerrain->GetTriFlip(nXVert,nYVert))
					{
						pIndexData[baseIndex]     = iRect[0];
						pIndexData[baseIndex + 1] = iRect[1];
						pIndexData[baseIndex + 2] = iRect[3];

						pIndexData[baseIndex + 3] = iRect[0];
						pIndexData[baseIndex + 4] = iRect[3];
						pIndexData[baseIndex + 5] = iRect[2];
					}
					else
					{
						pIndexData[baseIndex]     = iRect[0];
						pIndexData[baseIndex + 1] = iRect[1];
						pIndexData[baseIndex + 2] = iRect[2];

						pIndexData[baseIndex + 3] = iRect[2];
						pIndexData[baseIndex + 4] = iRect[1];
						pIndexData[baseIndex + 5] = iRect[3];
					}

					// next quad
					baseIndex += 6;
				}
			}

			m_vecIBTemp[m] = GetRenderSystem()->CreateIndexBuffer((uint8*)&pIndexData[0],sizeof(uint16) * pIndexData.size(),sizeof(uint16));
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
