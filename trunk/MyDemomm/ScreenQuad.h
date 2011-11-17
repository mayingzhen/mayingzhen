#ifndef  C_SCREENQUAD__H__
#define  C_SCREENQUAD__H__

#include <D3Dx9.h>

class CScreenQuad
{
public:
	CScreenQuad();
	~CScreenQuad();

	void Create();
	void Render();

private:
	struct SimpleVertex
	{
		SimpleVertex() {}
		SimpleVertex(const D3DXVECTOR3& position, const D3DXVECTOR2& texCoords)
		{
			this->position = position;
			this->texCoords = texCoords;
		}
		D3DXVECTOR3 position;
		D3DXVECTOR2 texCoords;
	};

	IDirect3DVertexDeclaration9* m_simpleVertexDeclaration;
	SimpleVertex m_quadVerts[4]; 
};

extern CScreenQuad g_SceneQuad;

#endif