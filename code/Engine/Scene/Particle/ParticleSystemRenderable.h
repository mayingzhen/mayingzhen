#pragma once


#include "../../RenderSystem/IRenderDevice/TransientBuffer.h"

namespace ma
{
	class ParticleSystemRenderable : public Renderable
	{
	public:

		ParticleSystemRenderable();

		~ParticleSystemRenderable(void);

		virtual void Render(Technique* pTech);

		bool AllocVertices( int nAllocVerts, int nAllocInds );

		struct VERTEX 
		{
			Vector3 pos;
			Vector2 uv;
			Vector2 nextUV;
			uint32 blend;
			uint32 color;
		};

	private:

		SubAllocVB m_subAllocVB;
		SubAllocIB m_subAllocIB;

		friend class ParticleSystem;
		friend class BatchParticleRenderable;
	};

}