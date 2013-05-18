#ifndef _Sample_Animation_H_
#define _Sample_Animation_H_

namespace ma
{
	class AnimationPlay;
	class Skeleton;
	//class RendMesh;
	class Texture;
	class Animation;

	class SampleAnimationRetarget : public Sample
	{
	public:
		SampleAnimationRetarget();

		virtual void Init(ApplicationBase* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

	private:
		void OnInput();

	private:
		RendMesh*		m_pRenderMeshA_b;
		RendMesh*		m_pRenderMeshA_f;
		RendMesh*		m_pRenderMeshA_h;
		Texture*	m_pRendTextureA_b;
		Texture*	m_pRendTextureA_f;
		Texture*	m_pRendTextureA_h;

		AnimationPlay*	m_pAnimtionPlayA;
		
		Skeleton*		m_pSkeletonA;
		Animation*		m_pAnimationA120;
		Animation*		m_pAnimationA100;

		RendMesh*		m_pRenderMeshB;
		Texture*	m_pRendTextureB;
		AnimationPlay*	m_pAnimtionPlayB;
		Skeleton*		m_pSkeletonB;
		Animation*		m_pAnimationB120;
		Animation*		m_pAnimationB602;
	};
}

#endif

