#ifndef _Sample_SceneSerialize_H_
#define _Sample_SceneSerialize_H_

namespace ma
{
	class Scene;

	class SampleSceneSerialize : public Sample
	{
	public:
		SampleSceneSerialize();

		virtual void Init(ApplicationBase* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

	};
}


#endif

