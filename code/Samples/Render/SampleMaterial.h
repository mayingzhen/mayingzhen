#ifndef _Sample_Material_H_
#define _Sample_Material_H_

namespace ma
{
	class Scene;

	class SampleMaterial: public Sample
	{
	public:
		SampleMaterial();

		virtual void	Load();

		virtual void	UnLoad();

		//virtual bool	keyPressed(const OIS::KeyEvent &arg);
	};
}


#endif

