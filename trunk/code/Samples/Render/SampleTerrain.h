#ifndef _SampleTerrain_H_
#define _SampleTerrain_H_

namespace ma
{
	class Terrain;
	
	class SampleTerrain : public Sample
	{
	public:
		SampleTerrain();

		virtual void Load();

		virtual	void UnLoad();

		virtual void Update();

		virtual void Render();

	private:

	};
}


#endif

