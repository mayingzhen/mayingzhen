#ifndef  _TerrainRenderable__H__
#define  _TerrainRenderable__H__

#include "Engine/Renderable/Renderable.h"


namespace ma
{
	class TerrainTrunk;

	class TerrainRenderable : public Renderable
	{
	public:
		TerrainRenderable(TerrainTrunk* pParent);

		~TerrainRenderable();

		void PreRender(Technique* pTech);

		void Render(Technique* pTech);

		void UpdateUniform(TerrainTrunk* pParent);

	public:
 		float m_fMateriID;
	};

}

#endif