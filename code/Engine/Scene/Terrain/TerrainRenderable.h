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

		void Render(Technique* pTech);

	//private:
		TerrainTrunk* m_pParent;

		float m_fMateriID;

		bool m_bBorder;

	};

}

#endif