#pragma once

#include "Engine/Renderable/Renderable.h"


namespace ma
{
	class TerrainTrunk;

	class TerrainRenderable : public Renderable
	{
	public:
		TerrainRenderable(TerrainTrunk* pParent);

		~TerrainRenderable();

		void PreRender(Technique* pTech,SceneContext* sc) override;

		void UpdateUniform(TerrainTrunk* pParent);

	public:
 		float m_fMateriID;
	};

}
