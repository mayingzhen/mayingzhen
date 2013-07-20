#ifndef _TerrainLiquid_H__
#define _TerrainLiquid_H__


namespace ma
{
	class Terrain;

	class TerrainLiquid : public Renderable
	{

		struct sLiquidVertex
		{
			Vector3 vPos;
			Vector3 vNormal;
			Vector2 vVetrexUV0;
			float vVetrexUV1;
		};

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

			POINT():x(0),z(0){}
			POINT(int x, int z):x(x),z(z){}
			bool operator < (const POINT& p) 
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

			bool operator == (const POINT& p) 
			{
				return ( p.x == x && p.z == z )? true: false;
			}
		};

	public:
		TerrainLiquid();
		~TerrainLiquid();

		//void BuildReflectionTexture();
		//void BuildRefractionTexture();

		void create(const Vector3& seedPos, float fTextureScale,
			bool bDepthEnable, float fDepthScale, Terrain* pTerrainSystem );
		void destroy();
		void Render();

	private:
		void	 createVertexData();
		void	 createIndexData();
		void	 createVertexDeclation();
		void	 CreateWaterTex();
		void	 addGrid( int x, int z );
		bool	 isValidGrid( int x, int z, int direction );
		Vector2		getTexCoord( int x, int z, int type );
		float	 getDepthTexCoord( int x, int z );
		void	 spreed( int x, int z, int direction );
		void	 spreedOneStep( int x, int z );
		bool	 isGridContained( int x, int z );

		Vector3 getPos( int x, int z )
		{
			return Vector3( mSeedPos.x + x * mStep.x, mSeedPos.y, mSeedPos.z + z * mStep.y );
		}

	private:

		//IDirect3DVertexBuffer9* m_pVertex;
		//IDirect3DIndexBuffer9*  m_pIndex;

		//cRect3d m_worldRect;
		Terrain* m_pTerrainSystem;

		Vector3 mSeedPos;
		Vector2 mStep;
		float m_fTextureScale;
		float m_fDepthScale;
		bool m_bDepthEnable;

		//IDirect3DVertexDeclaration9*  m_pVertexDeclaration;
		std::list<TerrainLiquid::POINT>	m_pTempGridArray;

		std::vector<Texture*> m_allFrameTex;
		/*LPDIRECT3DTEXTURE9*/Texture* m_pDepthTex;

		/*LPDIRECT3DTEXTURE9*/RenderTarget* m_ReflectionTexture;
		/*LPDIRECT3DTEXTURE9*/RenderTarget* m_RefractionTexture;

		double mfLastUpdate;
		double mfTime;
	};


}







#endif // _TerrainLiquid_H__


