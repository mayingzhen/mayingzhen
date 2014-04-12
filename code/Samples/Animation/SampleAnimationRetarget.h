#ifndef _Sample_Animation_H_
#define _Sample_Animation_H_

namespace ma
{
	class AnimationComponent;
	class MeshComponent;

	class SampleAnimationRetarget : public Sample
	{
	public:
		SampleAnimationRetarget();

		virtual void	Load();

		virtual void	UnLoad();

		virtual void	Update();

	private:
		AnimationComponentPtr		m_pAnimtionObjectA;
		
		AnimationComponentPtr		m_pAnimtionObjectB;
	};
}

#endif

