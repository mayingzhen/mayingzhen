#ifndef _Sample_Animation_H_
#define _Sample_Animation_H_

namespace ma
{
	class AnimationPlay;
	class Skeleton;
	class IRendMesh;
	class IRendTexture;
	class Animation;

	class SampleAnimationRetarget : public SimpleSceneView
	{
	public:
		SampleAnimationRetarget();

		virtual void Init(Application* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

		virtual	void OnResize(int w,int h);

	private:
		void OnInput();

	private:
		IRendMesh*		m_pRenderMeshA_b;
		IRendMesh*		m_pRenderMeshA_f;
		IRendMesh*		m_pRenderMeshA_h;
		IRendTexture*	m_pRendTextureA_b;
		IRendTexture*	m_pRendTextureA_f;
		IRendTexture*	m_pRendTextureA_h;

		AnimationPlay*	m_pAnimtionPlayA;
		
		Skeleton*		m_pSkeletonA;
		Animation*		m_pAnimationA120;
		Animation*		m_pAnimationA100;

		IRendMesh*		m_pRenderMeshB;
		IRendTexture*	m_pRendTextureB;
		AnimationPlay*	m_pAnimtionPlayB;
		Skeleton*		m_pSkeletonB;
		Animation*		m_pAnimationB120;
		Animation*		m_pAnimationB602;
	};
}

#endif

