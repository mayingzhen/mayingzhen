#ifndef _Sample_Animation_H_
#define _Sample_Animation_H_

namespace ma
{
	class IAnimationPlay;
	class RenderMesh;

	class SampleAnimationRetarget : public Sample
	{
	public:
		SampleAnimationRetarget();

		virtual void	Load();

		virtual void	UnLoad();

		virtual void	Update();

		virtual void	Render();

	private:
		void			OnInput();


	private:
		RenderMesh*		m_pRenderMeshA_b;
		RenderMesh*		m_pRenderMeshA_f;
		RenderMesh*		m_pRenderMeshA_h;
		IAnimationPlay*	m_pAnimtionPlayA;
		
		RenderMesh*		m_pRenderMeshB;
		IAnimationPlay*	m_pAnimtionPlayB;
	};
}

#endif

