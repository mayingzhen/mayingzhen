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

	public:
		Vector2 m_vBlendOffSet;
		Vector2 m_vCellAmount;
		float m_fMateriID;

		AABB m_posAABB;
		AABB2D m_tcAABB;

	};

}

#endif