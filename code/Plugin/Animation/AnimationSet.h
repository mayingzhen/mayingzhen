#ifndef  _AnimationSet__H__
#define  _AnimationSet__H__

namespace ma
{
	class SkelAnimtion;
	class Skeleton;
	class PoseModifier;
	class AnimationComponent;

	class ANIMATION_API AnimationSet : public Object
	{
	public:
		AnimationSet(RefPtr<Skeleton> pSkeleton, RefPtr<AnimationSetData> pAniSetData);

		~AnimationSet();

		virtual	SkelAnimtion*		CreateSkelAnim(const char* actionName);

		virtual	void				AddAnimClip(const char* pszSkaPath, const char* actionName);

		virtual SkelAnimtion*		GetSkelAnimByName(const char* pszName);

		virtual UINT				GetSkelAnimNumber() {return m_arrSkelAnim.size();}

		virtual SkelAnimtion*		GetSkelAnimByIndex(UINT index);

		virtual void				RemoveSkelAnim(SkelAnimtion* pAction);

	private:
		std::vector<SkelAnimtion*>		m_arrSkelAnim;

		RefPtr<Skeleton>				m_pSkeleton;

		RefPtr<AnimationSetData>		m_pAniSetData;
	};
}


#endif
