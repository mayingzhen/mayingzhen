#ifndef _Sample_PartialAnimation_H_
#define _Sample_PartialAnimation_H_

namespace ma
{
	class AnimationPlay;
	class Skeleton;
	class IRendMesh;
	class IRendTexture;

	class SamplePartialAnimation : public SimpleSceneView
	{
	public:
		SamplePartialAnimation();

		virtual void Init(Application* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

		virtual	void OnResize(int w,int h);

	private:
		AnimationPlay*	m_pAnimtionPlay;

		Skeleton*		m_pSkeleton;

		IRendMesh*		m_pRenderMesh;

		IRendTexture*	m_pRendTexture;
	};
}


#endif
