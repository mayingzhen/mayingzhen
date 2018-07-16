#pragma once

namespace ma
{
	class Terrain;
	
	class SampleTerrain : public Sample
	{
	public:
		SampleTerrain();

		virtual void		Load();

		virtual void		UnLoad();

	private:
		//RefPtr<Terrain>		m_pTerrain;
	};
}


