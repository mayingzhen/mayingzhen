#ifndef  _AnimationSet__H__
#define  _AnimationSet__H__

namespace ma
{
	class Action;
	class Skeleton;
	class PoseModifier;
	class AnimationPlay;

	class AnimationSet : public IAnimationSet
	{
	public:
		AnimationSet(AnimationPlay* pAniPlay);

		virtual	void	AddAction(const char* pszSkaPath, const char* actionName);

		Action*			GetActionByName(const char* pszName);

		UINT			GetActionNumber() {return m_arrAnimation.size();}

		Action*			GetActionByIndex(UINT index);

		void			AddAction(Action* pAction);

		void			RemoveAction(Action* pAction);

		void			Serialize(Serializer& sl, const char* pszLable = "AnimationSet");

	private:
		std::vector<BoneSet*>		m_arrBoneSet;

		std::vector<PoseModifier*>	m_arrPoseModifier;
			
		std::vector<Action*>		m_arrAnimation;

		AnimationPlay*				m_pAnimPlay;
	};
}


#endif
