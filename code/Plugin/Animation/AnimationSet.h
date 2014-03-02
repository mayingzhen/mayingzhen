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
		AnimationSet(AnimationObject* pAniPlay,AnimationSetData* pAniSetData);

		virtual	IAction*	CreateAction(const char* actionName);

		virtual	void		AddAnimClip(const char* pszSkaPath, const char* actionName);

		virtual IAction*	GetActionByName(const char* pszName);

		virtual UINT		GetActionNumber() {return m_arrAction.size();}

		virtual IAction*	GetActionByIndex(UINT index);

		virtual void		RemoveAction(IAction* pAction);

	private:
		std::vector<Action*>		m_arrAction;

		AnimationObject*			m_pAnimObject;

		AnimationSetData*			m_pAniSetData;
	};
}


#endif
