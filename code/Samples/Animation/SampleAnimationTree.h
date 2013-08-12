#ifndef _Sample_AnimationTree_H_
#define _Sample_AnimationTree_H_

namespace ma
{
	class IAnimationObject;
	class RenderMesh;


	class SampleAnimationTree : public Sample,OIS::KeyListener
	{
	public:
		SampleAnimationTree();

		virtual void Load();

		virtual void UnLoad();

		virtual bool keyPressed(const OIS::KeyEvent &arg);

		virtual bool keyReleased(const OIS::KeyEvent &arg);	

	private:
		GameObject*			m_pGameObj;

		IAnimClipNode*		m_pClip602;
		IAnimClipNode*		m_pClip100;
	};
}


#endif

