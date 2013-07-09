#ifndef  _ParticlComponent__H__
#define  _ParticlComponent__H__

#include "Component.h"


namespace ma
{
	class RenderMesh;
	class Material;
	
	class FRAMWORK_API ParticlComponent : public Component
	{
		DECL_OBJECT(ParticlComponent)

	public:
		ParticlComponent(GameObject* pGameObj); 

		~ParticlComponent();

		void			Render();

		void			Update();

		void			Start();

		void			Stop();

		void			Load(const char* pszPath);

		void			GetBoundingAABB(Vector3& vMin,Vector3& vMax);

		virtual void	Serialize(Serializer& sl, const char* pszLable = "ParticlComponent");

	private:
		ParticleEmitter*	m_pParticleEmit;
	};
}

#endif
