#ifndef _Sample_MonoScript_H_
#define _Sample_MonoScript_H_

namespace ma
{
	class SampleMonoScript : public Sample
	{
	public:
		SampleMonoScript();

		virtual void Load();

		virtual void UnLoad();

		virtual void Update();

	private:
		IScriptObject*	m_pTestScript;

		GameObjectPtr	m_pGameObj;
	};

}

#endif
