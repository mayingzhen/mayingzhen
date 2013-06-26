#ifndef  _SCREENQUAD__H__
#define  _SCREENQUAD__H__


namespace ma
{
	class ENGINE_API ScreenQuad
	{
	public:
		void Init();
		
		void Render(Material* pMaterial);

		Matrix4x4&	GetMatProj() {return m_matProj;}

	private:
		struct Vertex
		{
			Vector3 position;
			Vector2 texCoords;
		};

		Renderable*	m_pRenderable;

		Matrix4x4	m_matProj;
	};

}

#endif


