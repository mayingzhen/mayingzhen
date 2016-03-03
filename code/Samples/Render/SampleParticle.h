#ifndef _SampleParticle_H_
#define _SampleParticle_H_

namespace ma
{
	class SampleParticle : public Sample
	{
	public:
		SampleParticle();

		virtual void Load();

		virtual void UnLoad();

		virtual void Update();
	};
}


#endif

