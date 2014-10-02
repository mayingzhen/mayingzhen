#ifndef _SampleTerrain_H_
#define _SampleTerrain_H_

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
		RefPtr<Terrain>	m_pTerrain;
	};
}


#endif

