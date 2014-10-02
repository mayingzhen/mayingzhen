#ifndef _TerrainSection_H__
#define _TerrainSection_H__

namespace ma
{
	class Terrain;

	class TerrainSection : public RenderComponent
	{
		DECL_OBJECT(TerrainSection)

	public:
		TerrainSection(Terrain* pTerrain);

		~TerrainSection();


		virtual	void		Update();

		virtual	void		Show(Camera* pCamera);

		virtual Material*	GetMaterial();

		virtual void		SetMaterial(Material* pMaterial);

		virtual void		Serialize(Serializer& sl, const char* pszLable = "TerrainSection");

		void				Create(int heightMapX, int heightMapY, int xVerts, int yVerts);

		int					GetHeightMapX() {return m_heightMapX;}

		int					GetHeightMapY() {return m_heightMapY;}

	private:
		void				CreateVertexData();

		void				CreateIndexData();
		
		void				CreateVertexDeclation();
		
		void				ChangeGridUV( Vector2& topLeft,  Vector2& topRight,
			Vector2& botomLeft, Vector2& botomRight,
			Uint8 op, Uint8 tri );

	private:
		// 顶点数
		int				m_xVerts; 
		int				m_yVerts; 

		// Section在Terrain内的位置
		int				m_heightMapX;
		int				m_heightMapY;	

		RenderablePtr	m_pRenderable;

		//AABB			m_WorldAABB;

		Terrain*		m_pParent;

	};

	DeclareRefPtr(TerrainSection);
}



#endif // _TerrainSection_H__


