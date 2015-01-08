#ifndef _Sample_SceneSerialize_H_
#define _Sample_SceneSerialize_H_

namespace ma
{
	class Scene;

	class SampleSceneSerialize : public Sample
	{
	public:
		SampleSceneSerialize();

		virtual void	Load();

		virtual void	UnLoad();

		virtual	void	Update();

		virtual bool	keyPressed(const OIS::KeyEvent &arg);

	private:
		bool			OnLoadOver();

	private:
		bool			m_bLoadOver;
	};
}


#endif

