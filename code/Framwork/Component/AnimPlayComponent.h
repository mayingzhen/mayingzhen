#ifndef  _AnimPlayComponent__H__
#define  _AnimPlayComponent__H__

#include "Component.h"

namespace ma
{
	class IAnimationPlay;

	class FRAMWORK_API AnimPlayComponent : public Component
	{
		DECL_OBJECT(AnimPlayComponent)
	public:
		AnimPlayComponent(GameObject* pGameObj);

		~AnimPlayComponent();

		void			Update();

		IAnimationPlay*	GetAnimPlay() {return m_pAnimtionPlay;}

		void			Load(const char* pszAniSetPath, const char* pszSkeletonPath);

		void			SetSkinMesh(RenderMesh* pSkinMesh) {m_pRenderMesh = pSkinMesh;}

		virtual void	Serialize(Serializer& sl, const char* pszLable = "AnimPlayComponent");

	private:
		RenderMesh*		m_pRenderMesh;

		IAnimationPlay* m_pAnimtionPlay;
	};
}

#endif
