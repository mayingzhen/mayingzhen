#pragma once


namespace ma
{

class CParticleSystem;
class CParticleSystemRenderable : public Renderable
{
public:
	CParticleSystemRenderable(CParticleSystem* pParent);
	~CParticleSystemRenderable(void);

	// ---------------------------------------------------------------------
	// 
	// ---------------------------------------------------------------------
	virtual void Render(Technique* pTech);

	CParticleSystem* GetParent() {return m_pParent;}

private:
	CParticleSystem* m_pParent;
	uint32 m_nNumVertices;
	uint32 m_nNumIndices;

	struct VERTEX 
	{
		Vector3 pos;
		Vector2 uv;
		Vector2 nextUV;
		uint32 blend;
		uint32 color;
	};
	vector<VERTEX> vertices;
	vector<uint16> indices;

	friend class CParticleSystem;
};

}