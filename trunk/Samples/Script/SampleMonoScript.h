#ifndef _Sample_MonoScript_H_
#define _Sample_MonoScript_H_

namespace ma
{
	class SampleMonoScript : public SimpleSceneView
	{
	public:
		SampleMonoScript();

		virtual void Init(Application* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

		virtual	void OnResize(int w,int h);


	private:
	};
}

#endif
