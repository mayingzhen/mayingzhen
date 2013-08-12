#ifndef  _AnimComponent__H__
#define  _AnimComponent__H__

#include "Component.h"

namespace ma
{
	class AnimationObject;

	class FRAMWORK_API AnimComponent : public Component
	{
		DECL_OBJECT(AnimComponent)
	public:
		AnimComponent(GameObject* pGameObj);

		~AnimComponent();

		virtual void		Start();

		virtual void		Stop();

		void				Update();

		IAnimationObject*	GetAnimObject() {return m_pAnimtionObject;}

		void				Load(const char* pszAniSetPath, const char* pszSkeletonPath);

		virtual void		Serialize(Serializer& sl, const char* pszLable = "AnimComponent");

	private:
		std::vector<RenderMesh*>	m_arrRenderMesh;

		IAnimationObject*				m_pAnimtionObject;
	};
}

#endif // _AnimComponent__H__
