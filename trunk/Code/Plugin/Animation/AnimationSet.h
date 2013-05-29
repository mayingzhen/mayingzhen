#ifndef  _AnimationSet__H__
#define  _AnimationSet__H__

namespace ma
{
	class Action;
	class Skeleton;
	class PoseModifier;

	class AnimationSet : public IAnimationSet
	{
	public:
		Action*	GetActionByName(const char* pszName);

		UINT	GetActionNumber() {return m_arrAnimation.size();}

		Action* GetActionByIndex(UINT index);

		void	AddAction(Action* pAction);

		void	RemoveAction(Action* pAction);

		void	Serialize(SerializeListener& sl, const char* pszLable = "AnimationSet");

	private:
		std::vector<BoneSet*>	m_arrBoneSet;

		std::vector<PoseModifier*> m_arrPoseModifier;
			
		std::vector<Action*>	m_arrAnimation;
	};
}


#endif
