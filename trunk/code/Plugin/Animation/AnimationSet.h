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
		AnimationSet(ref_ptr<Skeleton> pSkeleton, ref_ptr<AnimationSetData> pAniSetData);

		~AnimationSet();

		virtual	SkelAnimtion*		CreateSkelAnim(const char* actionName);

		virtual	void				AddAnimClip(const char* pszSkaPath, const char* actionName);

		virtual SkelAnimtion*		GetSkelAnimByName(const char* pszName);

		virtual UINT				GetSkelAnimNumber() {return m_arrSkelAnim.size();}

		virtual SkelAnimtion*		GetSkelAnimByIndex(UINT index);

		virtual void				RemoveSkelAnim(SkelAnimtion* pAction);

	private:
		std::vector<SkelAnimtion*>		m_arrSkelAnim;

		ref_ptr<Skeleton>				m_pSkeleton;

		ref_ptr<AnimationSetData>		m_pAniSetData;
	};
}


#endif
