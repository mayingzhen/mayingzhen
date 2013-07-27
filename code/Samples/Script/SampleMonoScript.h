#ifndef _Sample_MonoScript_H_
#define _Sample_MonoScript_H_

namespace ma
{
	class SampleMonoScript : public Sample
	{
	public:
		SampleMonoScript();

		virtual void Init();

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Update();

		virtual void Render();


	private:
		IScriptObject*	m_pTestScript;
	};

}

#endif
