#pragma once


namespace ma
{

class ParticleSystem;
class ParticleSystemRenderable : public Renderable
{
public:
	ParticleSystemRenderable(ParticleSystem* pParent);
	~ParticleSystemRenderable(void);

	// ---------------------------------------------------------------------
	// 
	// ---------------------------------------------------------------------
	virtual void Render(Technique* pTech);

	ParticleSystem* GetParent() {return m_pParent;}

private:
	ParticleSystem* m_pParent;
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

	friend class ParticleSystem;
	friend class BatchParticleRenderable;
};

}