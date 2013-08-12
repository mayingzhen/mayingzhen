#ifndef _Sample_Joint_H_
#define _Sample_Joint_H_

namespace ma
{
	class Scene;

	class SampleJoint : public Sample
	{
	public:
		SampleJoint();

		virtual void Load();

		virtual	void UnLoad();

		virtual void Update();

	};
}


#endif

