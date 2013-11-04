#ifndef _TerrainSection_H__
#define _TerrainSection_H__

namespace ma
{
	class TerrainSection : public RenderObject
	{
		DECL_OBJECT(TerrainSection)

	public:
		TerrainSection(GameObject* pGameObj);

		~TerrainSection();

		virtual void		Render(Technique* pTech);

		virtual	void		AddToRenderQueue();

		virtual Material*	GetMaterial();

		virtual void		SetMaterial(Material* pMaterial);

		virtual AABB		GetAABB();

		virtual void		SetWorldMatrix(const Matrix4x4& matWS);

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

		Renderable*		m_pRenderable;

		AABB			m_WorldAABB;

	};
}



#endif // _TerrainSection_H__


