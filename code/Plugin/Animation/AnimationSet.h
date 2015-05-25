#ifndef  _AnimationSet__H__
#define  _AnimationSet__H__

namespace ma
{
	class SkelAnimtion;
	class Skeleton;
	class AnimTreeNode;

	class AnimationSet : public Serializable
	{
		DECL_OBJECT(AnimationSet)	

	public:
		AnimationSet();

		~AnimationSet();

		AnimTreeNode*		GetSkelAnimByName(const char* pszName);

		UINT				GetSkelAnimNumber() {return m_arrSkelAnim.size();}

		AnimTreeNode*		GetSkelAnimByIndex(UINT index);
		
		void				AddSkelAnim(AnimTreeNode* pAction); 

		void				RemoveSkelAnim(AnimTreeNode* pAction);

		void				Serialize(Serializer& sl, const char* pszLable = "AnimationSet");
		
		void				SetSkeleton(Skeleton* pSkeleton);

	private:
		RefPtr<Skeleton>			m_pSkeleton;

		typedef std::vector< RefPtr<AnimTreeNode> >	 VEC_SkELANIM;
		VEC_SkELANIM				m_arrSkelAnim;
	};

	RefPtr<AnimationSet> CreateAnimationSet();
	RefPtr<AnimationSet> CreateAnimationSet(const char* pszFile);
}


#endif
