#include "TerrainSection.h"
#include "Terrain.h"


namespace ma
{
	struct sSectorVertex
	{
		Vector3 vPos;
		Vector3 Normal;
		Vector2 VetrexUV1;
		Vector2 VetrexUV2;
	};

	IMPL_OBJECT(TerrainSection,RenderObject)

	TerrainSection::TerrainSection(GameObject* pGameObj):
		RenderObject(pGameObj)
	{
		m_pRenderable = new Renderable();
		m_pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
		m_pRenderable->m_pSubMeshData = new SubMeshData();
		//m_pRenderable->m_pMaterial = GetMaterial();
	}

	TerrainSection::~TerrainSection()
	{
		SAFE_DELETE(m_pRenderable->m_pSubMeshData);
		SAFE_DELETE(m_pRenderable);
	}

	void TerrainSection::Render(Technique* pTech)
	{
		GetRenderSystem()->DrawRenderable(m_pRenderable,pTech);
	}

	Material* TerrainSection::GetMaterial()
	{
		Terrain* pTerrain = (Terrain*)GetTerrain();
		if (pTerrain == NULL)
			return NULL;

		return pTerrain->GetMaterial();
	}

	void TerrainSection::AddToRenderQueue()
	{
		GetRenderQueue()->AddRenderObj(RL_Solid,this);
	}

	void TerrainSection::SetMaterial(Material* pMaterial)
	{

	}

	AABB TerrainSection::GetAABB()
	{
		return AABB();
	}

	void TerrainSection::SetWorldMatrix(const Matrix4x4& matWS)
	{

	}

	void TerrainSection::Serialize(Serializer& sl, const char* pszLable/* = "TerrainSection"*/)
	{

	}

	void TerrainSection::Create(int heightMapX, int heightMapY,int xVerts, int yVerts)
	{
		m_heightMapX = heightMapX;
		m_heightMapY = heightMapY;
		m_xVerts = xVerts;
		m_yVerts = yVerts;

		CreateVertexData();
		CreateIndexData();
		CreateVertexDeclation();
	}

	void TerrainSection::CreateVertexDeclation()
	{
		m_pRenderable->m_pDeclaration = GetRenderDevice()->CreateVertexDeclaration();
		m_pRenderable->m_pDeclaration->AddElement(0,0,DT_FLOAT3,DU_POSITION,0);
		m_pRenderable->m_pDeclaration->AddElement(0,12,DT_FLOAT3,DU_NORMAL,0);
		m_pRenderable->m_pDeclaration->AddElement(0,24,DT_FLOAT2,DU_TEXCOORD0,0);
		m_pRenderable->m_pDeclaration->AddElement(0,32,DT_FLOAT2,DU_TEXCOORD1,0);
	}

	void TerrainSection::CreateIndexData()
	{
		Terrain* pTerrain = (Terrain*)GetTerrain();

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
				Terrain::GridInfo info = pTerrain->GetGridInfo(m_heightMapX + i,m_heightMapY + j);

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

		m_pRenderable->m_pIndexBuffer = GetRenderDevice()->CreateIndexBuffer(pIndexValues,total_indexes * sizeof(WORD));
		m_pRenderable->m_pSubMeshData->m_nIndexCount  = total_indexes;
	}

	void TerrainSection::CreateVertexData()
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

		Terrain* pTerrain = (Terrain*)GetTerrain();

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

				pVerts[base_index].vPos = pTerrain->GetPos(nHeightMapX,nHeightMapY);
				pVerts[base_index + 1].vPos = pTerrain->GetPos(nHeightMapX + 1,nHeightMapY);
				pVerts[base_index + 2].vPos = pTerrain->GetPos(nHeightMapX,nHeightMapY + 1);
				pVerts[base_index + 3].vPos = pTerrain->GetPos(nHeightMapX + 1, nHeightMapY + 1);

				m_WorldAABB.Merge(pVerts[base_index].vPos);
				m_WorldAABB.Merge(pVerts[base_index + 1].vPos);	
				m_WorldAABB.Merge(pVerts[base_index + 2].vPos);	
				m_WorldAABB.Merge(pVerts[base_index + 3].vPos);	
			
				pVerts[base_index].Normal = pTerrain->GetNormal( nHeightMapX,	   nHeightMapY);
				pVerts[base_index + 1].Normal = pTerrain->GetNormal( nHeightMapX + 1, nHeightMapY);
				pVerts[base_index + 2].Normal = pTerrain->GetNormal( nHeightMapX,     nHeightMapY + 1);
				pVerts[base_index + 3].Normal = pTerrain->GetNormal( nHeightMapX + 1, nHeightMapY + 1);
			
				int ngridIndex = nHeightMapX + nHeightMapY * (pTerrain->HeightMapW() - 1);
				Terrain::GridInfo info = pTerrain->GetGridInfo(nHeightMapX,nHeightMapY);

				///// 第一层UV
				{
					float uOffset = 0,vOffset = 0, uWidth = 0, vHeight = 0;
					Terrain::PixMap lay0 = pTerrain->GetPixMapByIndex(info.layer0);
					pTerrain->GetInTextureAtlasUV(lay0.textureId,uOffset,vOffset,uWidth,vHeight);
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
					Terrain::PixMap emptyPixMap;
					emptyPixMap.textureId = pTerrain->GetTextureFileSize() - 1;	// 最后一块
					emptyPixMap.left = 0.1f;
					emptyPixMap.top = 0.1f;
					emptyPixMap.right = 0.9f;
					emptyPixMap.bottom = 0.9f;
					
					Terrain::PixMap lay1;
					if (info.layer1 != 0XFFFF)
					{
						lay1 = pTerrain->GetPixMapByIndex(info.layer1);
					}
					else 
					{
						lay1 = emptyPixMap; 
						continue;
					}

					float uOffset = 0,vOffset = 0, uWidth = 0, vHeight = 0;
					pTerrain->GetInTextureAtlasUV(lay1.textureId,uOffset,vOffset,uWidth,vHeight);
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
		m_pRenderable->m_pVertexBuffers = GetRenderDevice()->CreateVertexBuffer(pVerts,nVertSize);
		m_pRenderable->m_pSubMeshData->m_nVertexCount = (m_xVerts - 1) * (m_yVerts - 1) * 4;

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



