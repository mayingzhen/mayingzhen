#ifndef _Sample_SceneSerialize_H_
#define _Sample_SceneSerialize_H_

namespace ma
{
	class Scene;

	class SampleSceneSerialize : public Sample
	{
	public:
		SampleSceneSerialize();

		virtual void Load();

		virtual void UnLoad();

		virtual void Update();

		virtual void Render();

	private:
		//Scene*		m_pScene;

	};
}


#endif

