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
		GameObject*			m_pChargigi;
		IAnimationObject*	m_pAnimtionObjectA;
		
		GameObject*			m_pCharMagic;
		IAnimationObject*	m_pAnimtionObjectB;
	};
}

#endif

