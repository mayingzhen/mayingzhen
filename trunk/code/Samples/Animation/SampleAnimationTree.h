#ifndef _Sample_AnimationTree_H_
#define _Sample_AnimationTree_H_

namespace ma
{
	class IAnimationObject;
	class MeshComponent;


	class SampleAnimationTree : public Sample
	{
	public:
		SampleAnimationTree();

		virtual void Load();

		virtual void UnLoad();

		virtual bool keyPressed(const OIS::KeyEvent &arg);

	private:

		IAnimClipNode*		m_pClip602;
		IAnimClipNode*		m_pClip120;
	};
}


#endif

