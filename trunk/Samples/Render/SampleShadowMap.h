#ifndef _Sample_ShadowMap_H_
#define _Sample_ShadowMap_H_

namespace ma
{
	class SampleShadowMap : public Sample
	{
	public:
		SampleShadowMap();

		virtual void Init(ApplicationBase* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

	private:
		Light* m_pLigt;
	};
}


#endif


