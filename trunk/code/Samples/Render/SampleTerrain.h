#ifndef _SampleTerrain_H_
#define _SampleTerrain_H_

namespace ma
{
	class Terrain;
	
	class SampleTerrain : public Sample,OIS::KeyListener
	{
	public:
		SampleTerrain();

		virtual void Load();

		virtual	void UnLoad();

		virtual bool keyPressed(const OIS::KeyEvent &arg);

		virtual bool keyReleased(const OIS::KeyEvent &arg);	
	};
}


#endif

