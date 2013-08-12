#ifndef  _AnimationObject__H__
#define  _AnimationObject__H__

namespace ma
{
	class Skeleton;
	class SkeletonPose;
	class Action;

	typedef UINT ActionID;

	class AnimationObject : public IAnimationObject
	{
	public:
		AnimationObject(const char* pszSkePath);

		~AnimationObject();

// 		virtual	void			Start();
// 
// 		virtual void			Stop();

 		virtual IAnimationSet*	GetAnimationSet() {return m_pAnimSet;}

		virtual void			PlayAnimation(const char* pszAnimName); 

		virtual void			PlayAnimation(ActionID actionID);

		virtual void			AdvanceTime(float fTimeElepse);

		virtual void			EvaluateAnimation(float fWeight);

		virtual	void			SetFrame(float fFrame);

		virtual UINT			GetSkinMatrixNumber() {return m_pSkeleton->GetBoneNumer();}

		virtual Matrix4x4*		GetSkinMatrixArray()  {return m_arrSkinMatrix;}	

		SkeletonPose*			GetAnimationPose() {return m_pose;}

		Skeleton*				GetSkeleton() {return m_pSkeleton;}

		void					AddSkinMesh(RenderMesh* pRenderMesh);

		//void					SetGameObject(GameObject* pGameObj) {m_pGameObj = pGameObj;}

	protected:
		void					CreateSkeleton(const char* pSkePath);

		void					PlayAnimation(Action* pSkelAnim);
	
	private:
		Skeleton*				m_pSkeleton;
		
		SkeletonPose*			m_pose;

		AnimationSet*			m_pAnimSet;

		Action*					m_pSkelAnim;

		Matrix4x4				m_arrSkinMatrix[256];

		//std::vector<RenderMesh*>	m_arrRenderMesh;

		//GameObject*				m_pGameObj;
	};
}

#endif

