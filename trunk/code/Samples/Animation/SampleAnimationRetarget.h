#ifndef _Sample_Animation_H_
#define _Sample_Animation_H_

namespace ma
{
	class AnimationPlay;
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
		AnimationPlay*	m_pAnimtionPlayA;
		
		RenderMesh*		m_pRenderMeshB;
		AnimationPlay*	m_pAnimtionPlayB;
	};
}

#endif

