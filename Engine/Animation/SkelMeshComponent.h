#ifndef  _SkelMeshComponent__H__
#define  _SkelMeshComponent__H__

#include "Engine/Scene/Component.h"

namespace ma
{
	class IAnimationPlay;
	class MeshComponent;

	class ENGINE_API SkelMeshComponent : public Component
	{
		DECL_OBJECT(SkelMeshComponent)
	public:
		SkelMeshComponent();

		~SkelMeshComponent();

		void Update();
			 
		void Render();

		void AddMeshComp(MeshComponent* pMeshComp);

		void PlayAnimation(const char* pszAniName);

		void Load(const char* pszAniSetPath, const char* pszSkeletonPath);

		virtual void Serialize(SerializeListener& sl, const char* pszLable = "SkelMeshComponent");

	private:
		std::vector<MeshComponent*> m_vMeshComp;	

		IAnimationPlay* m_pAnimtionPlay;
	};
}

#endif
