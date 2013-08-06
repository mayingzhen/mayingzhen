#ifndef _Sample_AnimationTree_H_
#define _Sample_AnimationTree_H_

namespace ma
{
	class IAnimationPlay;
	class RenderMesh;


	class SampleAnimationTree : public Sample
	{
	public:
		SampleAnimationTree();

		virtual void Load();

		virtual void UnLoad();

		virtual void Update();

		virtual void Render();

	private:
		IAnimationPlay*	m_pAnimtionPlay;

		RenderMesh*		m_pRenderMesh;
	};
}


#endif

