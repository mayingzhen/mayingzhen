#ifndef _Sample_Animation_H_
#define _Sample_Animation_H_

namespace ma
{
	class IAnimationObject;
	class MeshComponent;

	class SampleAnimationRetarget : public Sample
	{
	public:
		SampleAnimationRetarget();

		virtual void	Load();

		virtual void	UnLoad();

		virtual void	Update();

	private:
		IAnimationObjectPtr		m_pAnimtionObjectA;
		
		IAnimationObjectPtr		m_pAnimtionObjectB;
	};
}

#endif

