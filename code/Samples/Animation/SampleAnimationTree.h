#ifndef _Sample_AnimationTree_H_
#define _Sample_AnimationTree_H_

namespace ma
{
	class AnimationPlay;
	class RenderMesh;


	class SampleAnimationTree : public Sample,OIS::KeyListener
	{
	public:
		SampleAnimationTree();

		virtual void Load();

		virtual void UnLoad();

		virtual void Update();

		virtual void Render();

		virtual bool keyPressed(const OIS::KeyEvent &arg);
		virtual bool keyReleased(const OIS::KeyEvent &arg);	

	private:
		AnimationPlay*	m_pAnimtionPlay;

		RenderMesh*		m_pRenderMesh;

		AnimClipNode*	m_pClip602;
		AnimClipNode*	m_pClip100;
	};
}


#endif

