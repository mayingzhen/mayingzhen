#ifndef _Sample_MonoScript_H_
#define _Sample_MonoScript_H_

namespace ma
{
	class SampleMonoScript : public Sample
	{
	public:
		SampleMonoScript();

		virtual void Init(ApplicationBase* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();


	private:
	};
}

#endif
