#ifndef  _AnimationSet__H__
#define  _AnimationSet__H__

namespace ma
{
	class SkelAnimtion;
	class Skeleton;
	class PoseModifier;
	class AnimationComponent;

	class ANIMATION_API AnimationSet : public Serializable
	{
		DECL_OBJECT(AnimationSet)	

	public:
		AnimationSet();

		~AnimationSet();

		virtual	SkelAnimtion*		CreateSkelAnim(const char* actionName);

		virtual	void				AddAnimClip(const char* pszSkaPath, const char* actionName);

		virtual SkelAnimtion*		GetSkelAnimByName(const char* pszName);

		virtual UINT				GetSkelAnimNumber() {return m_arrSkelAnim.size();}

		virtual SkelAnimtion*		GetSkelAnimByIndex(UINT index);

		virtual void				RemoveSkelAnim(SkelAnimtion* pAction);

		void						Serialize(Serializer& sl, const char* pszLable = "AnimationSet");
		
		void						SetSkeleton(Skeleton* pSkeleton);

	private:
		RefPtr<Skeleton>				m_pSkeleton;

		std::vector<SkelAnimtion*>		m_arrSkelAnim;
		std::vector<PoseModifier*>		m_arrPoseModifier;
	};

	//ANIMATION_API RefPtr<AnimationSet> CreateAnimationSet(const char* pAniSetFile);
}


#endif
