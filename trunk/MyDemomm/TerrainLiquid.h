#ifndef C_TERRAIN_LIQUID_H__
#define C_TERRAIN_LIQUID_H__


#include "public.h"
#include "CObject.h"

class CTerrain;


class TerrainLiquid : public CObject
{
public:
	struct sLiquidVertex
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vNormal;
		D3DXVECTOR2 vVetrexUV0;
		float vVetrexUV1;
	};

private:
	enum
	{
		LEFT_TOP,
		LEFT_BOTTOM,
		RIGHT_TOP,
		RIGHT_BOTTOM
	};

	enum 
	{
		FOUR_DIR,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	struct POINT
	{
		int x;
		int z;

		POINT()
			:x( 0 ), z( 0 )
		{}

		POINT( int x, int z )
			:x( x ),z( z )
		{}

		bool operator<( const POINT& p ) 
		{
			if( z < p.z )
				return true;
			else if( z > p.z )
				return false;
			else if( x < p.x )
				return true;
			else 
				return false;
		}

		bool operator == ( const POINT& p ) 
		{
			return ( p.x == x && p.z == z )? true: false;
		}
	};

public:
	TerrainLiquid();
	~TerrainLiquid();

	void BuildReflectionTexture();
	void BuildRefractionTexture();

	// create TerrainLiquid based on the TerrainInfo
	void create(const D3DXVECTOR3& seedPos, float fTextureScale,
		bool bDepthEnable, float fDepthScale, CTerrain* pTerrainSystem );
	void destroy();
	void Render();
	//virtual void update();

	void	 createVertexData();
	void	 createIndexData();
	void	 createVertexDeclation();
	void	 CreateWaterTex();
	void	 addGrid( int x, int z );
	bool	 isValidGrid( int x, int z, int direction );
	D3DXVECTOR2 getTexCoord( int x, int z, int type );
	float	 getDepthTexCoord( int x, int z );
	void	 spreed( int x, int z, int direction );
	void	 spreedOneStep( int x, int z );
	bool	 isGridContained( int x, int z );

	D3DXVECTOR3 getPos( int x, int z )
	{
		return D3DXVECTOR3( mSeedPos.x + x * mStep.x, mSeedPos.y, mSeedPos.z + z * mStep.y );
	}


	//cIndexBuffer* m_pIndex;
	//cVertexBuffer* m_pVertex;
	IDirect3DVertexBuffer9* m_pVertex;
	IDirect3DIndexBuffer9*  m_pIndex;

	//cRect3d m_worldRect;
	
	CTerrain* m_pTerrainSystem;

	D3DXVECTOR3 mSeedPos;
	D3DXVECTOR2 mStep;
	float m_fTextureScale;
	float m_fDepthScale;
	bool m_bDepthEnable;

	IDirect3DVertexDeclaration9*  m_pVertexDeclaration;

	std::list<TerrainLiquid::POINT>	m_pTempGridArray;

	std::vector<LPDIRECT3DTEXTURE9> m_allFrameTex;
	LPDIRECT3DTEXTURE9 m_pDepthTex;

	LPDIRECT3DTEXTURE9 m_ReflectionTexture;
	LPDIRECT3DTEXTURE9 m_RefractionTexture;


 	//cSurfaceMaterial* m_pSurfaceMaterial;
	//cTexture* m_Texture0;
	//cTexture* m_Texture1;

	double mfLastUpdate;
	double mfTime;
};







#endif // __terrainH__


