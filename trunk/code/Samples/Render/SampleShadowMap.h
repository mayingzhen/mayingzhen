#ifndef _Sample_ShadowMap_H_
#define _Sample_ShadowMap_H_

namespace ma
{
	class SampleShadowMap : public Sample
	{
	public:
		SampleShadowMap();

		virtual void Load();

		virtual void UnLoad();

		virtual void Update();

		virtual void Render();

	private:
		Light* m_pLigt;
	};
}


#endif


