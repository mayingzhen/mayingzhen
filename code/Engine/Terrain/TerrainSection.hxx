#include "TerrainSection.h"
#include "Terrain.h"


namespace ma
{
	TerrainSection::TerrainSection()
	{
		m_pTerrainSystem = NULL;

		//m_pSectorVerts = NULL;
		//m_pSectorIndex = NULL;
		//m_TerrainVertexDeclaration = NULL;
		m_ePrimitiveType = PRIM_TRIANGLELIST;
		m_pSubMeshData = new SubMeshData();
	}

	TerrainSection::~TerrainSection()
	{
		//SAFE_RELEASE(m_TerrainVertexDeclaration);
		//SAFE_RELEASE(m_pSectorVerts);
		//SAFE_RELEASE(m_pSectorIndex);
	}

	void TerrainSection::Create(Terrain* pParentSystem, 
								 int heightMapX, int heightMapY,
								 int xVerts, int yVerts)
	{
		m_pTerrainSystem = pParentSystem;
		m_heightMapX = heightMapX;
		m_heightMapY = heightMapY;
		m_xVerts = xVerts;
		m_yVerts = yVerts;

		createVertexData();
		createIndexData();
		createVertexDeclation();
	}

	void TerrainSection::createVertexDeclation()
	{
		m_pDeclaration = GetRenderDevice()->CreateVertexDeclaration();
		m_pDeclaration->AddElement(0,0,DT_FLOAT3,DU_POSITION,0);
		m_pDeclaration->AddElement(0,12,DT_FLOAT3,DU_NORMAL,0);
		m_pDeclaration->AddElement(0,24,DT_FLOAT2,DU_TEXCOORD0,0);
		m_pDeclaration->AddElement(0,32,DT_FLOAT2,DU_TEXCOORD1,0);
		m_pDeclaration->Active();
// 		D3DVERTEXELEMENT9 terrainVertexElements[] =
// 		{
// 			{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
// 			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
// 			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	
// 			{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	
// 			D3DDECL_END()
// 		};
// 
// 		V( g_pD3DDevice->CreateVertexDeclaration(terrainVertexElements, &m_TerrainVertexDeclaration) );
	}

	void TerrainSection::createIndexData()
	{
			/* 顶点顺序
				0     1 
				+-----+
				|    /|
				|   / |
				|  /  |
				| /   |
				+-----+
				2     3  
		*/

		int total_indexes = (m_xVerts - 1) * (m_yVerts - 1) * 6;
		WORD* pIndexValues = new WORD[total_indexes];

		WORD* index = pIndexValues;

		for (int j = 0; j < m_yVerts - 1; ++j)
		{
			for (int i = 0; i < m_xVerts - 1; ++i)
			{
				int gridx = m_heightMapX + i;
				int	gridy = m_heightMapY + j;
				int ngridIndex = gridx + gridy * (m_pTerrainSystem->HeightMapW() - 1);
				Terrain::g_info info = m_pTerrainSystem->m_GridInfoArray[ngridIndex];

				WORD iIndexTopLeft = ( i + j * (m_xVerts - 1) ) * 4;
				WORD iIndexTopRight = iIndexTopLeft + 1;
				WORD iIndexBottomLeft = iIndexTopLeft + 2;
				WORD iIndexBottomRight = iIndexTopLeft + 3;

				if( 0 == info.tri ) 
				{
					*index++ = iIndexTopLeft;     // 0
					*index++ = iIndexBottomLeft;  // 2
					*index++ = iIndexTopRight;    // 1
					*index++ = iIndexBottomLeft;  // 2
					*index++ = iIndexBottomRight; // 3
					*index++ = iIndexTopRight;    // 1
				}
				else 
				{
					*index++ = iIndexBottomLeft;  // 2
					*index++ = iIndexBottomRight; // 3
					*index++ = iIndexTopLeft;     // 0
					*index++ = iIndexBottomRight; // 3
					*index++ = iIndexTopRight;    // 1
					*index++ = iIndexTopLeft;     // 0  
				}						
			}
		}

		m_pIndexBuffer = GetRenderDevice()->CreateIndexBuffer(pIndexValues,total_indexes * sizeof(WORD));
		m_pIndexBuffer->Active();

		m_pSubMeshData->m_nIndexCount  = total_indexes;
// 		HRESULT hr = D3D_OK;	
// 		hr = g_pD3DDevice->CreateIndexBuffer(total_indexes * sizeof(WORD), 
// 			D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pSectorIndex,0);
// 		if( FAILED(hr) )
// 			return ;
// 
// 		WORD* indices = 0;
// 		m_pSectorIndex->Lock(0, 0, (void**)&indices, 0);
// 		memcpy( indices, pIndexValues, total_indexes * sizeof(WORD) );	
// 		m_pSectorIndex->Unlock();
// 
// 		SAFE_DELETE_ARRAY(pIndexValues);
	}

	void TerrainSection::createVertexData()
	{
		 
		/* 顶点顺序
				0     1 
				+-----+
				|    /|
				|   / |
				|  /  |
				| /   |
				+-----+
				2     3  
		*/

		bool result = true;

		sSectorVertex* pVerts = new sSectorVertex[(m_xVerts - 1) * (m_yVerts - 1) * 4];

		for (int y = 0; y < m_yVerts - 1; y += 1)
		{
			for (int x = 0; x < m_xVerts - 1; x += 1)
			{
				// 每个格子4个顶点0123 4567 .... 6个索引顶点 021231 465675 .....
				int base_index = 4 * (y * (m_xVerts - 1)  + x); 

				int nHeightMapX = m_heightMapX + x;
				int nHeightMapY = m_heightMapY + y;

				m_pTerrainSystem->GetPos( nHeightMapX,	    nHeightMapY,     pVerts[base_index].vPos );
				m_pTerrainSystem->GetPos( nHeightMapX + 1, nHeightMapY,     pVerts[base_index + 1].vPos );
				m_pTerrainSystem->GetPos( nHeightMapX,     nHeightMapY + 1, pVerts[base_index + 2].vPos );
				m_pTerrainSystem->GetPos( nHeightMapX + 1, nHeightMapY + 1, pVerts[base_index + 3].vPos );

				//m_WorldAABB.Merge(pVerts[base_index].vPos);
				//m_WorldAABB.Merge(pVerts[base_index + 1].vPos);	
				//m_WorldAABB.Merge(pVerts[base_index + 2].vPos);	
				//m_WorldAABB.Merge(pVerts[base_index + 3].vPos);	
			
				m_pTerrainSystem->GetNormal( nHeightMapX,	   nHeightMapY,     pVerts[base_index].Normal );
				m_pTerrainSystem->GetNormal( nHeightMapX + 1, nHeightMapY,     pVerts[base_index + 1].Normal );
				m_pTerrainSystem->GetNormal( nHeightMapX,     nHeightMapY + 1, pVerts[base_index + 2].Normal );
				m_pTerrainSystem->GetNormal( nHeightMapX + 1, nHeightMapY + 1, pVerts[base_index + 3].Normal );
			
				int ngridIndex = nHeightMapX + nHeightMapY * (m_pTerrainSystem->HeightMapW() - 1);
				Terrain::g_info info = m_pTerrainSystem->m_GridInfoArray[ngridIndex];

				///// 第一层UV
				{
					float uOffset = 0,vOffset = 0, uWidth = 0, vHeight = 0;
					Terrain::Pixmap lay0 = m_pTerrainSystem->m_PixMapArray[info.layer0];
					m_pTerrainSystem->GetInTextureAtlasUV(lay0.textureId,uOffset,vOffset,uWidth,vHeight);
					float Left = lay0.left * uWidth + uOffset;
					float top =  lay0.top  * vHeight + vOffset;
					float Right = lay0.right * uWidth + uOffset;
					float bottom = lay0.bottom * vHeight + vOffset;

					pVerts[base_index].VetrexUV1 = Vector2(Left,top); 
					pVerts[base_index + 1].VetrexUV1 = Vector2(Right,top); 	
					pVerts[base_index + 2].VetrexUV1 = Vector2(Left,bottom);
					pVerts[base_index + 3].VetrexUV1 = Vector2(Right,bottom);

					ChangeGridUV( pVerts[base_index].VetrexUV1, 
						pVerts[base_index + 1].VetrexUV1,
						pVerts[base_index + 2].VetrexUV1,
						pVerts[base_index + 3].VetrexUV1, 
						info.op0, info.tri );
				}

				///// 第二层UV
				{
					Terrain::Pixmap emptyPixMap;
					emptyPixMap.textureId = m_pTerrainSystem->m_vTextureFileNames.size() - 1;	// 最后一块
					emptyPixMap.left = 0.1f;
					emptyPixMap.top = 0.1f;
					emptyPixMap.right = 0.9f;
					emptyPixMap.bottom = 0.9f;
					
					Terrain::Pixmap lay1;
					if (info.layer1 != 0XFFFF)
					{
						lay1 = m_pTerrainSystem->m_PixMapArray[info.layer1];
					}
					else 
					{
						lay1 = emptyPixMap; 
						continue;
					}

					float uOffset = 0,vOffset = 0, uWidth = 0, vHeight = 0;
					m_pTerrainSystem->GetInTextureAtlasUV(lay1.textureId,uOffset,vOffset,uWidth,vHeight);
					float Left = lay1.left * uWidth + uOffset;
					float top =  lay1.top  * vHeight + vOffset;
					float Right = lay1.right * uWidth + uOffset;
					float bottom = lay1.bottom * vHeight + vOffset;

					pVerts[base_index].VetrexUV2 = Vector2(Left,top);
					pVerts[base_index + 1].VetrexUV2 = Vector2(Right,top);
					pVerts[base_index + 2].VetrexUV2 = Vector2(Left,bottom);
					pVerts[base_index + 3].VetrexUV2 = Vector2(Right,bottom);

					ChangeGridUV( pVerts[base_index].VetrexUV2,
						pVerts[base_index + 1].VetrexUV2,
						pVerts[base_index + 2].VetrexUV2,
						pVerts[base_index + 3].VetrexUV2,
						info.op1, info.tri );
				}

			}
		}

		UINT nVertSize = (m_xVerts - 1) * (m_yVerts - 1) * 4 * sizeof(sSectorVertex);
		m_pVertexBuffers[0] = GetRenderDevice()->CreateVertexBuffer(pVerts,nVertSize);
		//m_pVertexBuffers->Active();

		m_pSubMeshData->m_nVertexCount = (m_xVerts - 1) * (m_yVerts - 1) * 4;
// 		HRESULT hr = 0;
// 		g_pD3DDevice->CreateVertexBuffer( (m_xVerts - 1) * (m_yVerts - 1) * 4 * sizeof(sSectorVertex),
// 			D3DUSAGE_WRITEONLY,0,D3DPOOL_MANAGED,&m_pSectorVerts,0);
// 		if( FAILED(hr) )
// 			return ;
// 
// 		sSectorVertex* vBuf = 0;
// 		m_pSectorVerts->Lock(0, 0, (void**)&vBuf, 0);
// 		memcpy( vBuf, pVerts, (m_xVerts - 1) * (m_yVerts - 1) * 4 * sizeof(sSectorVertex) );	
// 		m_pSectorVerts->Unlock();
// 
// 		SAFE_DELETE_ARRAY(pVerts);
	}

	void TerrainSection::ChangeGridUV( Vector2& topLeft,  Vector2& topRight,
					  Vector2& botomLeft, Vector2& botomRight,
					  Uint8 op, Uint8 tri )
	{
		if (op & 1) // 水平翻转
		{
			Vector2 temp;
			temp = topLeft;
			topLeft= topRight;
			topRight = temp;

			temp = botomLeft;
			botomLeft = botomRight;
			botomRight = temp;	
		}

		if (op & 2) // 垂直翻转
		{
			Vector2 temp;
			temp = topLeft;
			topLeft = botomLeft;
			botomLeft = temp;

			temp = topRight;
			topRight = botomRight;
			botomRight = temp;	
		}

		if (op & 4) // 逆时针旋转90
		{
			Vector2 temp0,temp1,temp2,temp3;
			temp0 = topLeft;
			temp1 = topRight;
			temp2 = botomLeft;
			temp3 = botomRight;

			topLeft = temp1;
			topRight = temp3;
			botomLeft = temp0;
			botomRight = temp2;
		}

		if (op & 8) // 对角线上方顶点纹理坐标复制到对角线下方顶点。（与对角线垂直的两个顶点）
		{
			if (tri == 1)
			{
				botomLeft = topRight;
			}
			else
			{
				botomRight =topLeft;
			}
		}
	}
}



