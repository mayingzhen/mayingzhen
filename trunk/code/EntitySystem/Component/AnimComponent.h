#ifndef  _AnimComponent__H__
#define  _AnimComponent__H__

#include "Component.h"

namespace ma
{
	class AnimationObject;

	class ENTITYSYSTEM_API AnimComponent : public Component
	{
		DECL_OBJECT(AnimComponent)
	public:
		AnimComponent(GameObject* pGameObj);

		~AnimComponent();

		void				BuildRenderItem();

		IAnimationObject*	GetAnimObject() {return m_pAnimtionObject;}

		void				Load(const char* pszAniSetPath, const char* pszSkeletonPath);

		virtual void		Serialize(Serializer& sl, const char* pszLable = "AnimComponent");

	private:
		IAnimationObject*				m_pAnimtionObject;

		std::string						m_strSkeletonPath;
		std::string						m_strAnimaSetPath;
	};
}

#endif // _AnimComponent__H__
