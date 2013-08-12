#ifndef  _AnimPlayComponent__H__
#define  _AnimPlayComponent__H__

#include "Component.h"

namespace ma
{
	class AnimationPlay;

	class FRAMWORK_API AnimPlayComponent : public Component
	{
		DECL_OBJECT(AnimPlayComponent)
	public:
		AnimPlayComponent(GameObject* pGameObj);

		~AnimPlayComponent();

		virtual void	Start();

		virtual void	Stop();

		void			Update();

		IAnimationPlay*	GetAnimPlay() {return m_pAnimtionPlay;}

		void			Load(const char* pszAniSetPath, const char* pszSkeletonPath);

		virtual void	Serialize(Serializer& sl, const char* pszLable = "AnimPlayComponent");

	private:
		std::vector<RenderMesh*>	m_arrRenderMesh;

		IAnimationPlay*				m_pAnimtionPlay;
	};
}

#endif
