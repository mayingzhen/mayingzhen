#ifndef  _AnimationSystem__H__
#define  _AnimationSystem__H__

namespace ma
{
	class AnimationObject;

	class AnimationSystem : public IAnimationSystem
	{
	public:

		virtual IAnimationObject*	CreateAnimationObject(GameObject* pGameObj);

		virtual void				DeleteAnimationObject(IAnimationObject* pAnimObj);

		virtual	void				Update();

	private:
		std::vector<AnimationObject*>	m_arrAnimObject;
	};
}

#endif
