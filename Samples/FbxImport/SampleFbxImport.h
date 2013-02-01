#ifndef _Sample_FbxImport_H_
#define _Sample_FbxImport_H_

namespace ma
{
	class AnimationPlay;
	class Skeleton;
	class IRendMesh;
	class IRendTexture;

	class SampleFbxImport : public SimpleSceneView
	{
	public:
		SampleFbxImport();

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

