#ifndef _Sample_AnimationTree_H_
#define _Sample_AnimationTree_H_

namespace ma
{
	class AnimationPlay;
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
		AnimationPlay*	m_pAnimtionPlay;

		RenderMesh*		m_pRenderMesh;

		AnimClipNode*	m_pClip602;
		AnimClipNode*	m_pClip100;
	};
}


#endif

