#ifndef  _AnimationSet__H__
#define  _AnimationSet__H__

namespace ma
{
	class Action;
	class Skeleton;
	class PoseModifier;
	class AnimationObject;

	class AnimationSet : public IAnimationSet
	{
	public:
		AnimationSet(AnimationObject* pAniPlay);

		virtual	IAction*	CreateAction(const char* actionName);

		virtual	void		AddAnimClip(const char* pszSkaPath, const char* actionName);

		virtual IAction*	GetActionByName(const char* pszName);

		virtual UINT		GetActionNumber() {return m_arrAnimation.size();}

		virtual IAction*	GetActionByIndex(UINT index);

		virtual void		RemoveAction(IAction* pAction);

		virtual void		Serialize(Serializer& sl, const char* pszLable = "AnimationSet");

	private:
		std::vector<BoneSet*>		m_arrBoneSet;

		std::vector<PoseModifier*>	m_arrPoseModifier;

		std::vector<Action*>		m_arrAnimation;
		AnimationObject*			m_pAnimObject;
	};
}


#endif
