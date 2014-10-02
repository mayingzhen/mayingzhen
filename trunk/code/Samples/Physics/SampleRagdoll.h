#ifndef _Sample_Ragdoll_H_
#define _Sample_Ragdoll_H_

namespace ma
{
	class Scene;
	class Ragdoll;

	class SampleRagdoll : public Sample
	{
	public:
		SampleRagdoll();

		virtual void	Load();

		virtual	void	UnLoad();

		virtual void	Update();

		virtual void	Render();

		virtual bool	keyPressed(const OIS::KeyEvent &arg);

	private:
		SceneNode*		CreateCapsule(float fRadius,float fHeight,const char* pBoneName);

		void			CreateJoint(int eBone,int eParentBone,Vector3 vLowerLimit,Vector3 vUpLimit);

	private:

		//GameObject*				m_arrObject[BODYPART_COUNT];

		AnimationComponent*		m_pAnimationComp;

		//Skeleton*				m_pSkeleton;
	
		//SkeletonPose*			m_pAnimPose;

		bool					m_bStart;

		Ragdoll*				m_pRagdoll;	
	};
}


#endif

